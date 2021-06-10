#include "testing.h"
#include "storage/Log.hpp"

namespace kvs {

    TEST(Log, creation) {
        Log log = Log(100);
    }

    TEST(Log, addAndGet) {
        Log log = Log(100);
        Key key = Key("aaa", 3);
        log.add(key, Id(1));
        EXPECT_EQ(log.get(key).value().getId(), 1);
    }

    TEST(Log, RemoveAndGet) {
        Log log = Log(100);
        Key key = Key("aaa", 3);
        log.add(key, Id(1));
        log.remove(key);
        EXPECT_EQ(log.get(key), std::nullopt);
    }

    TEST(Log, DoubleAdd) {
        Log log = Log(100);
        Key key = Key("aaa", 3);
        EXPECT_FALSE(log.add(key, Id(2)).has_value());
        EXPECT_EQ(log.add(key, Id(3)).value(), Id(2));
    }
}

