#include "testing.h"

#include "RecordSerializer.hpp"

namespace kvs {

    TEST(RecordSerializer, writeAndRead) {
        RecordSerializer recordSerializer = RecordSerializer(Id::getIdSize());

        std::vector<Id> nextIds;
        nextIds.reserve(256);
        for (std::size_t i = 0; i < 256; ++i) {
            nextIds.emplace_back(i);
        }
        Record record = Record(nextIds);
        char *recordInBytes = recordSerializer.recordToBytes(record);

        Record newRecord = recordSerializer.bytesToRecord(recordInBytes);

        delete[] recordInBytes;

        std::vector<Id> newNextIds = newRecord.getNextRecords();

        for (int i = 0; i < 256; ++i) {
            EXPECT_EQ(nextIds[i].getId(), newNextIds[i].getId());
        }
    }

}
