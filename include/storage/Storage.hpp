#ifndef KEYVALUESTORAGE_STORAGE_HPP
#define KEYVALUESTORAGE_STORAGE_HPP

#include "model/Id.hpp"
#include "model/KeyValue.hpp"
#include "storage/File.hpp"
#include "serializer/RecordSerializer.hpp"
#include "model/TrieNode.hpp"
#include "serializer/TrieNodeSerializer.hpp"
#include <optional>
#include <iostream>
#include "model/InMemoryTrieNode.hpp"

namespace kvs {

    template<typename T>
    class Storage {
    public:
        virtual Id add(const T &) = 0;

        virtual std::optional<T> get(const Id &) const = 0;

        virtual void remove(const Id &) = 0;

        virtual void clear() = 0;
    };

    template<>
    class Storage<Record> {
    public:
        struct RecordStorageIterator {
            using value_type = Record;
            using pointer = value_type *;
            using reference = value_type &;

            explicit RecordStorageIterator(Storage<Record> &storage, std::size_t ptr = 0) : _storage(storage),
                                                                                            _begin(0),
                                                                                            _end(storage.size()),
                                                                                            m_ptr(ptr) {}

            value_type operator*() const { return _storage.getAnyway(Id(m_ptr)).value(); }

            void operator++() {
                m_ptr++;
            }

            [[nodiscard]] Id currentId() const { return Id(m_ptr); };

            friend bool
            operator==(const RecordStorageIterator &a, const RecordStorageIterator &b) {
                return a.m_ptr == b.m_ptr;
            };

            friend bool
            operator!=(const RecordStorageIterator &a, const RecordStorageIterator &b) {
                return a.m_ptr != b.m_ptr;
            };

            RecordStorageIterator begin() { return RecordStorageIterator(_storage, _begin); }

            RecordStorageIterator end() { return RecordStorageIterator(_storage, _end); }

        private:
            Storage<Record> &_storage;
            std::size_t _begin;
            std::size_t _end;
            std::size_t m_ptr = _begin;
        };

        Storage(File &dataFile, const RecordSerializer &recordSerializer) : _dataFile(dataFile),
                                                                            _recordSerializer(recordSerializer) {
            _recordSize = _recordSerializer.getKeySize() + 1 + _recordSerializer.getValueSize();
        }

        Id add(const Record &record) {
            char *recordInBytes = _recordSerializer.recordToBytes(record);
            FileOffset recordOffset = _dataFile.write(recordInBytes, _recordSize);

            delete[] recordInBytes;

            _countAllRecords++;

            return Id(recordOffset.getOffset() / _recordSize);
        }

        [[nodiscard]] std::optional<Record> get(const Id &id) const {
            char *recordInBytes = _dataFile.read(_recordSize, FileOffset(_recordSize * id.getId()));

            Record record = _recordSerializer.bytesToRecord(recordInBytes);
            return record.getIsOutdated() ? std::nullopt : std::make_optional(record);
        }

        std::optional<Record> getAnyway(const Id &id) {
            char *recordInBytes = _dataFile.read(_recordSize, FileOffset(_recordSize * id.getId()));

            Record record = _recordSerializer.bytesToRecord(recordInBytes);
            return record;
        }

        void remove(const Id &id) {
            auto bitOutdatedOffset = FileOffset(id.getId() * _recordSize + _recordSerializer.getKeySize());
            _dataFile.writeByOffset(bitOutdatedOffset, new char[1]{'1'}, 1);

            _countRemovedRecords++;
        }

        void clear() {
            _dataFile.clear();
        }

        [[nodiscard]] bool isFull() const {
            return _countAllRecords > 1000 && (_countAllRecords < 2 * _countRemovedRecords);
        }

        void rebuild() {
            auto readIterator = Storage<Record>::RecordStorageIterator(*this);
            auto writeIterator = Storage<Record>::RecordStorageIterator(*this);

            int cnt = 0;
            for (; readIterator != readIterator.end(); ++readIterator) {
                Record record = *readIterator;

                if (!record.getIsOutdated()) {
                    _dataFile.writeByOffset(FileOffset(_recordSize * writeIterator.currentId().getId()),
                                            _recordSerializer.recordToBytes(record),
                                            _recordSize);

                    ++writeIterator;
                    cnt++;
                }
            }

            _countAllRecords -= _countRemovedRecords;
            _countRemovedRecords = 0;
        }

        [[nodiscard]] std::size_t size() const {
            return _countAllRecords;
        }

    private:
        std::size_t _countRemovedRecords = 0;
        std::size_t _countAllRecords = 0;
        std::size_t _recordSize;
        File &_dataFile;
        RecordSerializer _recordSerializer;
    };

    template<>
    class Storage<TrieNode> {
    public:
        Storage(File &trieNodeFile, const TrieNodeSerializer &trieNodeSerializer) : _trieNodeFile(trieNodeFile),
                                                                                    _trieNodeSerializer(
                                                                                            trieNodeSerializer) {
        }

        Id add(const TrieNode &trieNode) {
            char *recordInBytes = _trieNodeSerializer.trieNodeToBytes(trieNode);
            FileOffset recordOffset = _trieNodeFile.write(recordInBytes, _trieNodeSize);

            delete[] recordInBytes;

            return Id(recordOffset.getOffset() / _trieNodeSize);
        }

        [[nodiscard]] std::optional<TrieNode> get(const Id &id) const {
            char *trieNodeInBytes = _trieNodeFile.read(_trieNodeSize,
                                                       FileOffset(_trieNodeSize * id.getId()));

            TrieNode trieNode = kvs::TrieNodeSerializer::bytesToTrieNode(trieNodeInBytes);

            delete[] trieNodeInBytes;

            return std::make_optional(trieNode);
        }

        void remove(const Id &id) = delete;

        void replace(const Id &id, const TrieNode &trieNode) {
            char *recordInBytes = _trieNodeSerializer.trieNodeToBytes(trieNode);

            _trieNodeFile.writeByOffset(FileOffset(_trieNodeSize * id.getId()),
                                        recordInBytes,
                                        _trieNodeSize);

            delete[] recordInBytes;
        }

        void clear() {
            _trieNodeFile.clear();
        }

        Id addTrieNodeSubtree(const std::shared_ptr<InMemoryTrieNode> &node) {
            std::vector<Id> ids(UCHAR_MAX + 1);

            for (std::size_t i = 0; i <= UCHAR_MAX; ++i) {
                if (node->get(i)) {
                    ids[i] = addTrieNodeSubtree(node->get(i));
                }
            }

            if (node->getId().getId() != std::numeric_limits<std::size_t>::max()) {
                std::vector<Id> leafIds(256, Id(std::numeric_limits<std::size_t>::max()));
                leafIds[0] = node->getId();

                return this->add(TrieNode(leafIds));
            }

            TrieNode trieNode(ids);
            char *trieNodeInBytes = _trieNodeSerializer.trieNodeToBytes(trieNode);
            FileOffset trieNodeOffset = _trieNodeFile.write(trieNodeInBytes, _trieNodeSize);

            delete[] trieNodeInBytes;

            return Id(trieNodeOffset.getOffset() / _trieNodeSize);
        }

    private:
        std::size_t _trieNodeSize = (UCHAR_MAX + 1) * Id::getIdSize();
        File &_trieNodeFile;
        TrieNodeSerializer _trieNodeSerializer;
    };
}

#endif //KEYVALUESTORAGE_STORAGE_HPP
