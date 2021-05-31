#include "testing.h"

#include "TrieNodeSerializer.hpp"

namespace kvs {

    TEST(TrieNodeSerializer, writeAndRead) {
        TrieNodeSerializer trieNodeSerializer = TrieNodeSerializer(3, 11);

        std::string key = "key";
        std::string value = "abracadabra";
        TrieNode trieNode = TrieNode(Key(key.c_str(), 3), false, Value(value.c_str(), 11));
        char *trieNodeInBytes = trieNodeSerializer.trieNodeToBytes(trieNode);

        TrieNode newTrieNode = trieNodeSerializer.bytesToTrieNode(trieNodeInBytes);

        delete[] trieNodeInBytes;
        EXPECT_EQ(strcmp(key.data(), newTrieNode.getKey().getKey()), 0);
        EXPECT_EQ(trieNode.getIsOutdated(), newTrieNode.getIsOutdated());
        EXPECT_EQ(strcmp(value.data(), newTrieNode.getValue().getValue()), 0);
        delete[] newTrieNode.getKey().getKey();
        delete[] newTrieNode.getValue().getValue();
    }
}