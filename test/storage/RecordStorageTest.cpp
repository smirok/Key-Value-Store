#include "testing.h"

#include "storage/Storage.hpp"

const std::string testFileName = "records.bin";

namespace kvs {

    TEST(RecordStorage, creation) {
        File file = File(testFileName);
        RecordSerializer recordSerializer = RecordSerializer(4, 8);
        Storage<Record>(file, recordSerializer);
        remove(testFileName.c_str());
    }

    TEST(RecordStorage, addAndGet) {
        File file = File(testFileName);
        RecordSerializer recordSerializer = RecordSerializer(4, 8);
        Storage<Record> storage(file, recordSerializer);

        Record record(Key("aaaa", 4), false, Value("bbbbbbbb", 8));
        Id id = storage.add(record);
        EXPECT_EQ(id.getId(), 0);

        std::optional<Record> takenRecordOptional = storage.get(id);
        EXPECT_EQ(strcmp(record.getKey().getKey(), takenRecordOptional.value().getKey().getKey()), 0);
        EXPECT_EQ(record.getIsOutdated(), takenRecordOptional.value().getIsOutdated());
        EXPECT_EQ(strcmp(record.getValue().getValue(), takenRecordOptional.value().getValue().getValue()), 0);
        remove(testFileName.c_str());
    }

    TEST(RecordStorage, autoIncrementAdd) {
        File file = File(testFileName);
        RecordSerializer recordSerializer = RecordSerializer(4, 8);
        Storage<Record> storage(file, recordSerializer);

        Record record(Key("aaaa", 4), false, Value("bbbbbbbb", 8));

        for (int i = 0; i < 100; ++i) {
            Id id = storage.add(record);
            EXPECT_EQ(id.getId(), i);
        }

        remove(testFileName.c_str());
    }

    TEST(RecordStorage, getIfWasRemoved) {
        File file = File(testFileName);
        RecordSerializer recordSerializer = RecordSerializer(4, 8);
        Storage<Record> storage(file, recordSerializer);

        Record record(Key("aaaa", 4), false, Value("bbbbbbbb", 8));

        Id id = storage.add(record);
        EXPECT_EQ(id.getId(), 0);
        storage.remove(id);
        std::optional<Record> takenRecordOptional = storage.get(Id(0));
        EXPECT_FALSE(takenRecordOptional.has_value());

        remove(testFileName.c_str());
    }

    TEST(RecordStorage, clear) {
        File file = File(testFileName);
        RecordSerializer recordSerializer = RecordSerializer(4, 8);
        Storage<Record> storage(file, recordSerializer);

        Record record(Key("aaaa", 4), false, Value("bbbbbbbb", 8));

        Id id = storage.add(record);
        EXPECT_EQ(id.getId(), 0);

        storage.clear();

        id = storage.add(record);
        EXPECT_EQ(id.getId(), 0);

        remove(testFileName.c_str());
    }
}

