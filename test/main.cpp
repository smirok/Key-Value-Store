#include "gtest/gtest.h"
#include "testing.h"

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}