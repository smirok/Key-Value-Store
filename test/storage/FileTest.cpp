#include "testing.h"
#include "storage/File.hpp"

namespace kvs {

    TEST(File, creation) {
        std::string fileName = "testFile1.txt";

        File file(fileName);
        remove(fileName.c_str());
    }

    TEST(File, writeAndRead) {
        std::string fileName = "testFile2.txt";
        {
            File file(fileName);
            file.write("abcdef", 6);
            char *buffer = file.read(3, FileOffset(2));
            EXPECT_EQ(strcmp(buffer, "cde"), 0);
            delete[] buffer;
        }
        remove(fileName.c_str());
    }

    TEST(File, rewrite) {
        std::string fileName = "testFile3.txt";
        {
            File file(fileName);
            file.write("aaaaaa", 6);
            file.write("bbbbbb", 6);
            file.writeByOffset(FileOffset(3), "cccccc", 6);
            char *buffer = file.read(12, FileOffset(0));
            EXPECT_EQ(strcmp(buffer, "aaaccccccbbb"), 0);
            delete[] buffer;
        }
        remove(fileName.c_str());
    }
}