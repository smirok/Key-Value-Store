#include "testing.h"

#include "serializer/TrieNodeSerializer.hpp"

namespace kvs {

    TEST(TrieNodeSerializer, writeAndRead) {
        TrieNodeSerializer trieNodeSerializer = TrieNodeSerializer(Id::getIdSize());

        std::vector<Id> nextIds;
        nextIds.reserve(256);
        for (std::size_t i = 0; i < 256; ++i) {
            nextIds.emplace_back(i);
        }
        TrieNode trieNode = TrieNode(nextIds);
        char *trieNodeInBytes = trieNodeSerializer.trieNodeToBytes(trieNode);

        TrieNode newTrieNode = trieNodeSerializer.bytesToTrieNode(trieNodeInBytes);

        delete[] trieNodeInBytes;

        std::vector<Id> newNextIds = newTrieNode.getNextRecords();

        for (int i = 0; i < 256; ++i) {
            EXPECT_EQ(nextIds[i].getId(), newNextIds[i].getId());
        }
    }

}
