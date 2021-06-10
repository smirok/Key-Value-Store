#include "testing.h"

#include "model/Key.hpp"

constexpr int KEY_SIZE = 8;

namespace kvs {

    TEST(Key, CorrectMemoryWork) {
        std::string first = "aaaabbbb";

        Key firstKey(first.data(), KEY_SIZE);
        Key secondKey(firstKey);
        Key thirdKey(std::move(firstKey));

        firstKey = secondKey;
        thirdKey = std::move(firstKey);
        EXPECT_EQ(strcmp(thirdKey.getKey(), first.data()), 0);
    }

}

