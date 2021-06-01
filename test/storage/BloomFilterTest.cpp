#include "testing.h"
#include "storage/BloomFilter.hpp"
#include <random>
#include <climits>
#include <functional>

using random_bytes_engine = std::independent_bits_engine<
        std::default_random_engine, CHAR_BIT, unsigned char>;

namespace kvs {

    TEST(BloomFilter, creation) {
        BloomFilter filter = BloomFilter(static_cast<std::size_t>(100));
    }

    TEST(BloomFilter, testAddOneAndContains) {
        BloomFilter filter = BloomFilter(static_cast<std::size_t>(100));
        std::string string_key = "somekey";
        Key key = Key(string_key.c_str(), string_key.size());

        filter.add(key);
        EXPECT_TRUE(filter.mightContains(key));
    }

    TEST(BloomFilter, testAddAndContainsStress) {
        random_bytes_engine randomBytesEngine;
        std::vector<Key> generatedKeys;

        BloomFilter filter = BloomFilter(static_cast<std::size_t>(1000000));
        for (int i = 0; i < 100000; ++i) {
            std::string string_key;
            std::generate_n(string_key.begin(), 10, std::ref(randomBytesEngine));

            generatedKeys.emplace_back(string_key.c_str(), string_key.size());
            filter.add(generatedKeys.back());
        }

        for (const Key &key : generatedKeys) {
            EXPECT_TRUE(filter.mightContains(key));
        }
    }

    TEST(BloomFilter, testContainsNotExists) {
        BloomFilter filter = BloomFilter(static_cast<std::size_t>(100));
        std::string string_key = "somekey";
        Key key = Key(string_key.c_str(), string_key.size());

        EXPECT_FALSE(filter.mightContains(key));
    }
}