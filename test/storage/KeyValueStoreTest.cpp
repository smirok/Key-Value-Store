#include "testing.h"
#include "KeyValueStore.hpp"
#include <string>

namespace kvs {

    TEST(KeyValueStore, creation) {
        BloomFilter bloomFilter(100);
        Log log(10000);
        File trieFile("trieNodes.bin");
        File recordFile("records.bin");
        TrieNodeSerializer trieNodeSerializer(Id::getIdSize());
        RecordSerializer recordSerializer(4, 8);
        Storage<TrieNode> trieNodeStorage(trieFile, trieNodeSerializer);
        Storage<Record> recordStorage(recordFile, recordSerializer);

        Trie trie(trieNodeStorage);

        KeyValueStore keyValueStore(bloomFilter, log, trie, recordStorage);
        remove("trieNodes.bin");
        remove("records.bin");
    }

    TEST(KeyValueStore, addAndGet) {
        BloomFilter bloomFilter(100000);
        Log log(10000);
        File trieFile("trieNodes.bin");
        File recordFile("records.bin");
        TrieNodeSerializer trieNodeSerializer(Id::getIdSize());
        RecordSerializer recordSerializer(4, 8);
        Storage<TrieNode> trieNodeStorage(trieFile, trieNodeSerializer);
        Storage<Record> recordStorage(recordFile, recordSerializer);

        Trie trie(trieNodeStorage);

        KeyValueStore keyValueStore(bloomFilter, log, trie, recordStorage);

        KeyValue keyValue(Key("aaaa", 4), Value("bbbbbbbb", 8));

        keyValueStore.add(keyValue);

        KeyValue gotKeyValue(keyValueStore.get(keyValue.getKey()).value());
        EXPECT_EQ(strcmp(keyValue.getKey().getKey(), Key("aaaa", 4).getKey()), 0);
        EXPECT_EQ(strcmp(keyValue.getValue().getValue(), gotKeyValue.getValue().getValue()), 0);
        remove("trieNodes.bin");
        remove("records.bin");
    }

    TEST(KeyValueStore, addAndDel) {
        BloomFilter bloomFilter(100000);
        Log log(100);
        File trieFile("trieNodes.bin");
        File recordFile("records.bin");
        TrieNodeSerializer trieNodeSerializer(Id::getIdSize());
        RecordSerializer recordSerializer(4, 8);
        Storage<TrieNode> trieNodeStorage(trieFile, trieNodeSerializer);
        Storage<Record> recordStorage(recordFile, recordSerializer);

        Trie trie(trieNodeStorage);

        KeyValueStore keyValueStore(bloomFilter, log, trie, recordStorage);

        KeyValue keyValue(Key("aaaa", 4), Value("bbbbbbbb", 8));

        keyValueStore.add(keyValue);
        keyValueStore.del(keyValue.getKey());

        EXPECT_FALSE(keyValueStore.get(keyValue.getKey()).has_value());
        remove("trieNodes.bin");
        remove("records.bin");
    }

    TEST(KeyValueStore, logOverflow) {
        BloomFilter bloomFilter(10000000);
        Log log(100000);
        File trieFile("trieNodes.bin");
        File recordFile("records.bin");
        TrieNodeSerializer trieNodeSerializer(Id::getIdSize());
        RecordSerializer recordSerializer(4, 8);
        Storage<TrieNode> trieNodeStorage(trieFile, trieNodeSerializer);
        Storage<Record> recordStorage(recordFile, recordSerializer);

        Trie trie(trieNodeStorage);

        KeyValueStore keyValueStore(bloomFilter, log, trie, recordStorage);

        for (char c1 = 'a'; c1 <= 'z'; ++c1) {
            for (char c2 = 'a'; c2 <= 'z'; ++c2) {
                for (char c3 = 'a'; c3 <= 'z'; ++c3) {
                    for (char c4 = 'a'; c4 <= 'z'; ++c4) {
                        std::string s;
                        s.push_back(c1);
                        s.push_back(c2);
                        s.push_back(c3);
                        s.push_back(c4);
                        keyValueStore.add(KeyValue(
                                Key(s.data(), 4),
                                Value("aaaaaaaa", 8)));
                    }
                }
            }
        }

        remove("trieNodes.bin");
        remove("records.bin");
    }

    TEST(KeyValueStore, logOverflowAndGet) {
        BloomFilter bloomFilter(10000000);
        Log log(1000000);
        File trieFile("trieNodes.bin");
        File recordFile("records.bin");
        TrieNodeSerializer trieNodeSerializer(Id::getIdSize());
        RecordSerializer recordSerializer(4, 8);
        Storage<TrieNode> trieNodeStorage(trieFile, trieNodeSerializer);
        Storage<Record> recordStorage(recordFile, recordSerializer);

        Trie trie(trieNodeStorage);

        KeyValueStore keyValueStore(bloomFilter, log, trie, recordStorage);

        for (char c1 = 'a'; c1 <= 'z'; ++c1) {
            for (char c2 = 'a'; c2 <= 'z'; ++c2) {
                for (char c3 = 'a'; c3 <= 'z'; ++c3) {
                    for (char c4 = 'a'; c4 <= 'z'; ++c4) {
                        std::string s;
                        s.push_back(c1);
                        s.push_back(c2);
                        s.push_back(c3);
                        s.push_back(c4);
                        keyValueStore.add(KeyValue(
                                Key(s.data(), 4),
                                Value("aaaaaaaa", 8)));
                    }
                }
            }
        }

        for (char c1 = 'a'; c1 <= 'z'; ++c1) {
            for (char c2 = 'a'; c2 <= 'z'; ++c2) {
                for (char c3 = 'a'; c3 <= 'z'; ++c3) {
                    for (char c4 = 'a'; c4 <= 'z'; ++c4) {
                        std::string s;
                        s.push_back(c1);
                        s.push_back(c2);
                        s.push_back(c3);
                        s.push_back(c4);
                        EXPECT_TRUE(keyValueStore.get(Key(s.data(), 4)).has_value());
                    }
                }
            }
        }


        remove("trieNodes.bin");
        remove("records.bin");
    }
}

