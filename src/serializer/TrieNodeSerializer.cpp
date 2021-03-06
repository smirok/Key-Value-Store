#include "serializer/TrieNodeSerializer.hpp"
#include "climits"

namespace kvs {

    TrieNodeSerializer::TrieNodeSerializer(std::size_t idSize) : _idSize(idSize) {
    }

    char *TrieNodeSerializer::trieNodeToBytes(const TrieNode &trieNode) const {
        char *result = new char[_idSize * ALPHABET_SIZE + 1];

        std::vector<Id> nextRecords = trieNode.getNextRecords();
        for (auto &nextRecord : nextRecords) {
            *(reinterpret_cast<std::size_t *>(result)) = nextRecord.getId();
            result += Id::getIdSize();
        }

        *result = '\0';

        return result - _idSize * ALPHABET_SIZE;
    }

    TrieNode TrieNodeSerializer::bytesToTrieNode(const char *bytes) const {
        std::vector<Id> ids;
        ids.reserve(ALPHABET_SIZE);
        for (std::size_t i = 0; i < ALPHABET_SIZE; ++i) {
            ids.emplace_back(*(reinterpret_cast<std::size_t *>(const_cast<char *>(bytes))));
            bytes += Id::getIdSize();
        }

        return TrieNode(ids);
    }
}