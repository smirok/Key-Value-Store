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
        Storage(File &dataFile, const RecordSerializer &recordSerializer) : _dataFile(dataFile),
                                                                            _recordSerializer(recordSerializer) {
        }

        Id add(const Record &record) {
            char *recordInBytes = _recordSerializer.recordToBytes(record);
            FileOffset recordOffset = _dataFile.write(recordInBytes,
                                                      _recordSerializer.getKeySize() + 1 +
                                                      _recordSerializer.getValueSize()); // TODO

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
        }

        void clear() {
            _dataFile.clear();
        }

    private:
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

            return Id(recordOffset.getOffset() /
                      (256 * Id::getIdSize())); // TODO how should do it
        }

        std::optional<TrieNode> get(const Id &id) {
            char *trieNodeInBytes = _trieNodeFile.read(256 * Id::getIdSize(),
                                                       FileOffset(256 * Id::getIdSize() *
                                                                  id.getId()));

            TrieNode trieNode = _trieNodeSerializer.bytesToTrieNode(trieNodeInBytes);
            return std::make_optional(trieNode);
        }

        void remove(const Id &id) = delete;

        void replace(const Id &id, const TrieNode &trieNode) {
            char *recordInBytes = _trieNodeSerializer.trieNodeToBytes(trieNode);

            _trieNodeFile.writeByOffset(FileOffset(256 * Id::getIdSize() *
                                                   id.getId()), recordInBytes, 256 * Id::getIdSize());
        }

        void clear() {
            _trieNodeFile.clear();
        }

        Id addTrieNodeSubtree(const InMemoryTrieNode *node) {
            std::vector<Id> ids(256);

            for (std::size_t i = 0; i < 256; ++i) {
                if (node->get(i)) {
                    ids[i] = addTrieNodeSubtree(node->get(i));
                }
            }

            TrieNode trieNode(ids);
            char *bytes = _trieNodeSerializer.trieNodeToBytes(trieNode);
            FileOffset trieNodeOffset = _trieNodeFile.write(bytes, 256 * Id::getIdSize());

            return Id(trieNodeOffset.getOffset() /
                      (256 * Id::getIdSize())); // TODO how should do it
        }

    private:
        File &_trieNodeFile;
        TrieNodeSerializer _trieNodeSerializer;
    };
}

#endif //KEYVALUESTORAGE_STORAGE_HPP
