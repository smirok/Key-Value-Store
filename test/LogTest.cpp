#include "testing.h"
#include "Log.hpp"

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
}

