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

    /**
     * \brief Storage interface of pairs < @p T, @p Id>. The middleware between logic storage and disk interaction.
     * @tparam T Type of stored objects.
     */
    template<typename T>
    class Storage {
    public:
        /**
         * Insert @p obj into a storage and assign it an inner identifier.
         * @param obj Inserted object.
         * @return Assigned identifier.
         */
        virtual Id add(const T &obj) = 0;

        /**
         * Get object by @p id.
         * @param id Object identifier.
         * @return @p T wrapped into @p std::optional, if there is some object in @p Storage with @p id;
         * empty @p std::optional otherwise.
         */
        virtual std::optional<T> get(const Id &id) const = 0;

        /**
         * Remove object with @p id. Do nothing if the object with @p id doesn't exist.
         * @param id Identifier of removed object.
         */
        virtual void remove(const Id &id) = 0;

        /**
         * Clean up storage.
         */
        virtual void clear() = 0;
    };

    /**
     * \brief Class storing @p Record on the disk.
     */
    template<>
    class Storage<Record> {
    public:
        /**
         * Iterator over @p Storage<Record>.
         */
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

        /**
         *
         * @param dataFile File for @p Record writing.
         * @param recordSerializer @p Record serializer.
         */
        Storage(File &dataFile, const RecordSerializer &recordSerializer) : _dataFile(dataFile),
                                                                            _recordSerializer(recordSerializer) {
            _recordSize = _recordSerializer.getKeySize() + 1 + _recordSerializer.getValueSize();
        }

        /**
         * Add @p record in storage and assign it identifier.
         * @param record Inserted record.
         * @return Assigned identifier.
         */
        Id add(const Record &record) {
            char *recordInBytes = _recordSerializer.recordToBytes(record);
            FileOffset recordOffset = _dataFile.write(recordInBytes, _recordSize);

            delete[] recordInBytes;

            _countAllRecords++;

            return Id(recordOffset.getOffset() / _recordSize);
        }

        /**
         * Get @p Record by @p id.
         * @param id Object identifier.
         * @return @p Record wrapper into @p std::optional, if there is some object in @p Storage with @p id;
         * empty @p std::optional otherwise.
         */
        [[nodiscard]] std::optional<Record> get(const Id &id) const {
            char *recordInBytes = _dataFile.read(_recordSize, FileOffset(_recordSize * id.getId()));

            Record record = _recordSerializer.bytesToRecord(recordInBytes);
            return record.getIsOutdated() ? std::nullopt : std::make_optional(record);
        }

        /**
         * Get @p Record by @p id.
         * Contract : object with the given @p id always exists, but probably outdated.
         * @param id Object identifier.
         * @return @p Record wrapped into @p std::optional.
         */
        std::optional<Record> getAnyway(const Id &id) {
            char *recordInBytes = _dataFile.read(_recordSize, FileOffset(_recordSize * id.getId()));

            Record record = _recordSerializer.bytesToRecord(recordInBytes);
            return record;
        }

        /**
         * Remove object with @p id. Do nothing if there is no object.
         * @param id Identifier of removed object.
         */
        void remove(const Id &id) {
            auto bitOutdatedOffset = FileOffset(id.getId() * _recordSize + _recordSerializer.getKeySize());
            _dataFile.writeByOffset(bitOutdatedOffset, new char[1]{'1'}, 1);

            _countRemovedRecords++;
        }

        /**
         * Clean up storage.
         */
        void clear() {
            _dataFile.clear();
        }

        /**
        * Check whether @p Log is full.
        * @return true if log is full; false otherwise.
        */
        [[nodiscard]] bool isFull() const {
            return _countAllRecords > 1000 && (_countAllRecords < 2 * _countRemovedRecords);
        }

        /**
         * Remove all the records with the given ids.
         * @param idsToRemove Array of objects @p Id to remove.
         */
        void removeRecordsById(const std::vector<Id> &idsToRemove) {
            for (const Id &id : idsToRemove) {
                remove(id);
            }
        }

        /**
         * Rebuild storage via deleting outdated records.
         */
        void rebuild() {
            auto readIterator = Storage<Record>::RecordStorageIterator(*this);
            auto writeIterator = Storage<Record>::RecordStorageIterator(*this);

            for (; readIterator != readIterator.end(); ++readIterator) {
                Record record = *readIterator;

                if (!record.getIsOutdated()) {
                    _dataFile.writeByOffset(FileOffset(_recordSize * writeIterator.currentId().getId()),
                                            _recordSerializer.recordToBytes(record),
                                            _recordSize);

                    ++writeIterator;
                }
            }

            _countAllRecords -= _countRemovedRecords;
            _countRemovedRecords = 0;
        }

        /**
         * @return Storage size.
         */
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

    /**
     * \brief Class storing @p Record on the disk.
     */
    template<>
    class Storage<TrieNode> {
    public:
        /**
         *
         * @param trieNodeFile File to writing @p TrieNode.
         * @param trieNodeSerializer Class serializing @p TrieNode.
         */
        Storage(File &trieNodeFile, const TrieNodeSerializer &trieNodeSerializer) : _trieNodeFile(trieNodeFile),
                                                                                    _trieNodeSerializer(
                                                                                            trieNodeSerializer) {
        }

        /**
         * Add @p record in storage and assign it identifier.
         * @param trieNode Inserted trie node.
         * @return Assigned identifier.
         */
        Id add(const TrieNode &trieNode) {
            char *recordInBytes = _trieNodeSerializer.trieNodeToBytes(trieNode);
            FileOffset recordOffset = _trieNodeFile.write(recordInBytes, _trieNodeSize);

            delete[] recordInBytes;

            return Id(recordOffset.getOffset() / _trieNodeSize);
        }

        /**
         * Get @p TrieNode by @p id.
         * Contract : there is @p TrieNode by @p id.
         * @param id Object identifier.
         * @return @p TrieNode wrapped into @p std::optional.
         */
        [[nodiscard]] std::optional<TrieNode> get(const Id &id) const {
            char *trieNodeInBytes = _trieNodeFile.read(_trieNodeSize,
                                                       FileOffset(_trieNodeSize * id.getId()));

            TrieNode trieNode = _trieNodeSerializer.bytesToTrieNode(trieNodeInBytes);

            delete[] trieNodeInBytes;

            return std::make_optional(trieNode);
        }

        void remove(const Id &id) = delete;

        /**
         * Write by @p id a new @p trieNode.
         * @param id Identifier.
         * @param trieNode TrieNode.
         */
        void replace(const Id &id, const TrieNode &trieNode) {
            char *recordInBytes = _trieNodeSerializer.trieNodeToBytes(trieNode);

            _trieNodeFile.writeByOffset(FileOffset(_trieNodeSize * id.getId()),
                                        recordInBytes,
                                        _trieNodeSize);

            delete[] recordInBytes;
        }

        /**
         * Clean up storage.
         */
        void clear() {
            _trieNodeFile.clear();
        }

        /**
         * Add all nodes of the given trie to storage.
         * @param node Root of the given trie.
         * @return Root identifier @p of the node in storage.
         */
        Id addTrieNodeSubtree(const std::shared_ptr<InMemoryTrieNode> &node) {
            std::vector<Id> ids(UCHAR_MAX + 1, Id(std::numeric_limits<std::size_t>::max()));

            for (std::size_t i = 0; i <= UCHAR_MAX; ++i) {
                if (node->get(i)) {
                    ids[i] = addTrieNodeSubtree(node->get(i));
                }
            }

            if (node->getId().getId() != std::numeric_limits<std::size_t>::max()) { // leaf
                std::vector<Id> leafIds(256, Id(std::numeric_limits<std::size_t>::max() - 1));
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
} // kvs

#endif //KEYVALUESTORAGE_STORAGE_HPP
