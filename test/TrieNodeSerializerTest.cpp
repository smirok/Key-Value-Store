#include "testing.h"

#include "RecordSerializer.hpp"

namespace kvs {

    TEST(RecordSerializer, writeAndRead) {
        RecordSerializer trieNodeSerializer = RecordSerializer(3, 11);

        std::string key = "key";
        std::string value = "abracadabra";
        Record trieNode = Record(Key(key.c_str(), 3), false, Value(value.c_str(), 11));
        char *trieNodeInBytes = trieNodeSerializer.recordToBytes(trieNode);

        Record newTrieNode = trieNodeSerializer.bytesToRecord(trieNodeInBytes);

        delete[] trieNodeInBytes;
        EXPECT_EQ(strcmp(key.data(), newTrieNode.getKey().getKey()), 0);
        EXPECT_EQ(trieNode.getIsOutdated(), newTrieNode.getIsOutdated());
        EXPECT_EQ(strcmp(value.data(), newTrieNode.getValue().getValue()), 0);
        delete[] newTrieNode.getKey().getKey();
        delete[] newTrieNode.getValue().getValue();
    }
}