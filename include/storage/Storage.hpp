#ifndef KEYVALUESTORAGE_STORAGE_HPP
#define KEYVALUESTORAGE_STORAGE_HPP

#include "model/Id.hpp"
#include "model/KeyValue.hpp"
#include "storage/File.hpp"
#include "serializer/RecordSerializer.hpp"
#include "model/TrieNode.hpp"
#include "serializer/TrieNodeSerializer.hpp"
#include <optional>
#include "model/InMemoryTrieNode.hpp"

namespace kvs {

    template<typename T>
    class Storage {
    public:
        Id add(const T &) {
            return Id(0);
        }

        std::optional<T> get(const Id &) {
            return std::nullopt;
        };

        void remove(const Id &) {
        }

        void clear() {

        }
    };

    template<>
    class Storage<Record> {
    public:
        struct RecordStorageIterator {
            using iterator_category = std::forward_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = Record;
            using pointer = value_type *;
            using reference = value_type &;

            RecordStorageIterator(Storage<Record> &storage, std::size_t ptr = 0) : _storage(storage),
                                                                                   _begin(0),
                                                                                   _end(storage.size()),
                                                                                   m_ptr(ptr) {}

            value_type operator*() const { return _storage.get(Id(m_ptr)).value(); }

            void operator++() {
                m_ptr++;
            }

            // Postfix
            void operator++(int) {
                m_ptr++;
            }

            Id currentId() const { return Id(m_ptr); };

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
        }

        Id add(const Record &record) {
            char *recordInBytes = _recordSerializer.recordToBytes(record);
            FileOffset recordOffset = _dataFile.write(recordInBytes,
                                                      _recordSerializer.getKeySize() + 1 +
                                                      _recordSerializer.getValueSize()); // TODO

            delete[] recordInBytes; // TODO

            countAllRecords++;

            return Id(recordOffset.getOffset() /
                      (_recordSerializer.getKeySize() + 1 + _recordSerializer.getValueSize())); // TODO how should do it
        }

        std::optional<Record> get(const Id &id) {
            char *recordInBytes = _dataFile.read(_recordSerializer.getKeySize() + 1 + _recordSerializer.getValueSize(),
                                                 FileOffset((_recordSerializer.getKeySize() + 1 +
                                                             _recordSerializer.getValueSize()) *
                                                            id.getId()));

            Record record = _recordSerializer.bytesToRecord(recordInBytes);
            return record.getIsOutdated() ? std::nullopt : std::make_optional(record);
        }

        void remove(const Id &id) {
            FileOffset bitOutdatedOffset =
                    FileOffset(id.getId() * (_recordSerializer.getKeySize() + 1 + _recordSerializer.getValueSize()) +
                               _recordSerializer.getKeySize());
            _dataFile.writeByOffset(bitOutdatedOffset, new char[1]{'1'}, 1);

            countRemovedRecords++;
        }

        void clear() {
            _dataFile.clear();
        }

        bool isFull() {
            return countAllRecords > 1000 && (countAllRecords < 2 * countRemovedRecords);
        }

        void rebuild() {
            countRemovedRecords = 0;
            countAllRecords -= countRemovedRecords;

            auto readIterator = Storage<Record>::RecordStorageIterator(*this);
            auto writeIterator = Storage<Record>::RecordStorageIterator(*this);

            for (; readIterator != readIterator.end(); readIterator++) {
                Record record = *readIterator;

                if (!record.getIsOutdated()) {
                    _dataFile.writeByOffset(FileOffset((_recordSerializer.getKeySize() + 1 +
                                                        _recordSerializer.getValueSize()) *
                                                       writeIterator.currentId().getId()),
                                            _recordSerializer.recordToBytes(record),
                                            _recordSerializer.getKeySize() + 1 +
                                            _recordSerializer.getValueSize());

                    writeIterator++;
                }
            }
        }

        std::size_t size() const {
            return countAllRecords;
        }

    private:
        std::size_t countRemovedRecords = 0;
        std::size_t countAllRecords = 0;
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
            FileOffset recordOffset = _trieNodeFile.write(recordInBytes,
                                                          256 * Id::getIdSize()); // TODO

            delete[] recordInBytes; // TODO

            return Id(recordOffset.getOffset() /
                      (256 * Id::getIdSize())); // TODO how should do it
        }

        std::optional<TrieNode> get(const Id &id) {
            char *trieNodeInBytes = _trieNodeFile.read(256 * Id::getIdSize(),
                                                       FileOffset(256 * Id::getIdSize() *
                                                                  id.getId()));

            TrieNode trieNode = _trieNodeSerializer.bytesToTrieNode(trieNodeInBytes);

            delete[] trieNodeInBytes; // TODO

            return std::make_optional(trieNode);
        }

        void remove(const Id &id) = delete;

        void replace(const Id &id, const TrieNode &trieNode) {
            char *recordInBytes = _trieNodeSerializer.trieNodeToBytes(trieNode);

            _trieNodeFile.writeByOffset(FileOffset(256 * Id::getIdSize() *
                                                   id.getId()), recordInBytes, 256 * Id::getIdSize());

            delete[] recordInBytes; // TODO
        }

        void clear() {
            _trieNodeFile.clear();
        }

        Id addTrieNodeSubtree(std::shared_ptr<InMemoryTrieNode> node) {
            std::vector<Id> ids(256);

            for (std::size_t i = 0; i < 256; ++i) {
                if (node->get(i)) {
                    ids[i] = addTrieNodeSubtree(node->get(i));
                }
            }

            TrieNode trieNode(ids);
            char *trieNodeInBytes = _trieNodeSerializer.trieNodeToBytes(trieNode);
            FileOffset trieNodeOffset = _trieNodeFile.write(trieNodeInBytes, 256 * Id::getIdSize());

            delete[] trieNodeInBytes; // TODO

            return Id(trieNodeOffset.getOffset() /
                      (256 * Id::getIdSize())); // TODO how should do it
        }

    private:
        File &_trieNodeFile;
        TrieNodeSerializer _trieNodeSerializer;
    };
}

#endif //KEYVALUESTORAGE_STORAGE_HPP
