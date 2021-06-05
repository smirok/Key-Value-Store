#include "testing.h"

#include "serializer/RecordSerializer.hpp"

namespace kvs {

    TEST(RecordSerializer, writeAndRead) {
        RecordSerializer recordSerializer = RecordSerializer(3, 11);

        std::string key = "key";
        std::string value = "abracadabra";
        Key keyS(key.data(), 3);
        Value valueS(value.data(), 11);
        Record record = Record(keyS, false, valueS);
        char *recordInBytes = recordSerializer.recordToBytes(record);

        Record newRecord = recordSerializer.bytesToRecord(recordInBytes);

        delete[] recordInBytes;
        EXPECT_EQ(strcmp(key.data(), newRecord.getKey().getKey()), 0);
        EXPECT_EQ(record.getIsOutdated(), newRecord.getIsOutdated());
        EXPECT_EQ(strcmp(value.data(), newRecord.getValue().getValue()), 0);
    }
}