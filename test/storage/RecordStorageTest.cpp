#include "testing.h"

#include "storage/Storage.hpp"

namespace kvs {

    TEST(RecordStorage, creation) {
        File file = File("data.bin");
        RecordSerializer recordSerializer = RecordSerializer(4, 8);
        Storage<Record>(file, recordSerializer);
        remove("data.bin");
    }

    TEST(RecordStorage, addAndGet) {
        File file = File("data.bin");
        RecordSerializer recordSerializer = RecordSerializer(4, 8);
        Storage<Record> storage(file, recordSerializer);

        Record record(Key("aaaa", 4), false, Value("bbbbbbbb", 8));
        Id id = storage.add(record);
        EXPECT_EQ(id.getId(), 0);

        std::optional<Record> takenRecordOptional = storage.get(id);
        EXPECT_EQ(strcmp(record.getKey().getKey(), takenRecordOptional.value().getKey().getKey()), 0);
        EXPECT_EQ(record.getIsOutdated(), takenRecordOptional.value().getIsOutdated());
        EXPECT_EQ(strcmp(record.getValue().getValue(), takenRecordOptional.value().getValue().getValue()), 0);
        remove("data.bin");
    }

    TEST(RecordStorage, autoIncrementAdd) {
        File file = File("data.bin");
        RecordSerializer recordSerializer = RecordSerializer(4, 8);
        Storage<Record> storage(file, recordSerializer);

        Record record(Key("aaaa", 4), false, Value("bbbbbbbb", 8));

        for (int i = 0; i < 100; ++i) {
            Id id = storage.add(record);
            EXPECT_EQ(id.getId(), i);
        }

        remove("data.bin");
    }

    TEST(RecordStorage, getIfNotExist) {
        File file = File("data.bin");
        RecordSerializer recordSerializer = RecordSerializer(4, 8);
        Storage<Record> storage(file, recordSerializer);

        Record record(Key("aaaa", 4), false, Value("bbbbbbbb", 8));

        Id id = storage.add(record);
        EXPECT_EQ(id.getId(), 0);
        std::optional<Record> takenRecordOptional = storage.get(Id(1));
        EXPECT_FALSE(takenRecordOptional.has_value());

        remove("data.bin");
    }

    TEST(RecordStorage, getIfWasRemoved) {
        File file = File("data.bin");
        RecordSerializer recordSerializer = RecordSerializer(4, 8);
        Storage<Record> storage(file, recordSerializer);

        Record record(Key("aaaa", 4), false, Value("bbbbbbbb", 8));

        Id id = storage.add(record);
        EXPECT_EQ(id.getId(), 0);
        storage.remove(id);
        std::optional<Record> takenRecordOptional = storage.get(Id(0));
        EXPECT_FALSE(takenRecordOptional.has_value());

        remove("data.bin");
    }

    TEST(RecordStorage, clear) {
        File file = File("data.bin");
        RecordSerializer recordSerializer = RecordSerializer(4, 8);
        Storage<Record> storage(file, recordSerializer);

        Record record(Key("aaaa", 4), false, Value("bbbbbbbb", 8));

        Id id = storage.add(record);
        EXPECT_EQ(id.getId(), 0);

        storage.clear();

        id = storage.add(record);
        EXPECT_EQ(id.getId(), 0);

        remove("data.bin");
    }
}

