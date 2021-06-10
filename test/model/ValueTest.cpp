#include "testing.h"

#include "model/Value.hpp"

constexpr int VALUE_SIZE = 8;

namespace kvs {

    TEST(Value, CorrectMemoryWork) {
        std::string first = "aaaabbbb";

        Value firstValue(first.data(), VALUE_SIZE);
        Value secondValue(firstValue);
        Value thirdValue(std::move(firstValue));

        firstValue = secondValue;
        thirdValue = std::move(firstValue);
        EXPECT_EQ(strcmp(thirdValue.getValue(), first.data()), 0);
    }

}

