#include "testing.h"

#include "TrieNodeSerializer.hpp"

namespace kvs {

    TEST(TrieNodeSerializer, writeAndRead) {
        TrieNodeSerializer recordSerializer = TrieNodeSerializer(Id::getIdSize());

        std::vector<Id> nextIds;
        nextIds.reserve(256);
        for (std::size_t i = 0; i < 256; ++i) {
            nextIds.emplace_back(i);
        }
        TrieNode record = TrieNode(nextIds);
        char *recordInBytes = recordSerializer.trieNodeToBytes(record);

        TrieNode newRecord = recordSerializer.bytesToTrieNode(recordInBytes);

        delete[] recordInBytes;

        std::vector<Id> newNextIds = newRecord.getNextRecords();

        for (int i = 0; i < 256; ++i) {
            EXPECT_EQ(nextIds[i].getId(), newNextIds[i].getId());
        }
    }

}
