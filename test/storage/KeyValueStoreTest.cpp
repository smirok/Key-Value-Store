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
        Log log(5000);
        File trieFile("trieNodes.bin");
        File recordFile("records.bin");
        TrieNodeSerializer trieNodeSerializer(Id::getIdSize());
        RecordSerializer recordSerializer(3, 8);
        Storage<TrieNode> trieNodeStorage(trieFile, trieNodeSerializer);
        Storage<Record> recordStorage(recordFile, recordSerializer);

        Trie trie(trieNodeStorage);

        KeyValueStore keyValueStore(bloomFilter, log, trie, recordStorage);

        for (char c1 = 'a'; c1 <= 'z'; ++c1) {
            for (char c2 = 'a'; c2 <= 'z'; ++c2) {
                for (char c3 = 'a'; c3 <= 'z'; ++c3) {
                    std::string s;
                    s.push_back(c1);
                    s.push_back(c2);
                    s.push_back(c3);
                    keyValueStore.add(KeyValue(
                            Key(s.data(), 3),
                            Value("aaaaaaaa", 8)));
                }
            }
        }

        remove("trieNodes.bin");
        remove("records.bin");
    }

    TEST(KeyValueStore, logOverflowAndGet) {
        BloomFilter bloomFilter(100000);
        Log log(5000);
        File trieFile("trieNodes.bin");
        File recordFile("records.bin");
        TrieNodeSerializer trieNodeSerializer(Id::getIdSize());
        RecordSerializer recordSerializer(3, 4);
        Storage<TrieNode> trieNodeStorage(trieFile, trieNodeSerializer);
        Storage<Record> recordStorage(recordFile, recordSerializer);

        Trie trie(trieNodeStorage);

        KeyValueStore keyValueStore(bloomFilter, log, trie, recordStorage);

        for (char c1 = 'a'; c1 <= 'z'; ++c1) {
            for (char c2 = 'a'; c2 <= 'z'; ++c2) {
                for (char c3 = 'a'; c3 <= 'z'; ++c3) {
                    std::string s;
                    s.push_back(c1);
                    s.push_back(c2);
                    s.push_back(c3);
                    keyValueStore.add(KeyValue(
                            Key(s.data(), 3),
                            Value(s.data(), 4)));
                }
            }
        }


        for (char c1 = 'a'; c1 <= 'z'; ++c1) {
            for (char c2 = 'a'; c2 <= 'z'; ++c2) {
                for (char c3 = 'a'; c3 <= 'z'; ++c3) {
                    std::string s;
                    s.push_back(c1);
                    s.push_back(c2);
                    s.push_back(c3);
                    EXPECT_EQ(strcmp(keyValueStore.get(Key(s.data(), 3))->getValue().getValue(), s.data()), 0);
                }
            }
        }

        remove("trieNodes.bin");
        remove("records.bin");
    }

    TEST(KeyValueStore, clear) {
        BloomFilter bloomFilter(100000);
        Log log(5000);
        File trieFile("trieNodes.bin");
        File recordFile("records.bin");
        TrieNodeSerializer trieNodeSerializer(Id::getIdSize());
        RecordSerializer recordSerializer(3, 8);
        Storage<TrieNode> trieNodeStorage(trieFile, trieNodeSerializer);
        Storage<Record> recordStorage(recordFile, recordSerializer);

        Trie trie(trieNodeStorage);

        KeyValueStore keyValueStore(bloomFilter, log, trie, recordStorage);

        for (char c1 = 'a'; c1 <= 'z'; ++c1) {
            for (char c2 = 'a'; c2 <= 'z'; ++c2) {
                for (char c3 = 'a'; c3 <= 'z'; ++c3) {
                    std::string s;
                    s.push_back(c1);
                    s.push_back(c2);
                    s.push_back(c3);
                    keyValueStore.add(KeyValue(
                            Key(s.data(), 3),
                            Value("aaaaaaaa", 8)));
                }
            }
        }

        keyValueStore.clear();

        for (char c1 = 'a'; c1 <= 'z'; ++c1) {
            for (char c2 = 'a'; c2 <= 'z'; ++c2) {
                for (char c3 = 'a'; c3 <= 'z'; ++c3) {
                    std::string s;
                    s.push_back(c1);
                    s.push_back(c2);
                    s.push_back(c3);
                    EXPECT_FALSE(keyValueStore.get(Key(s.data(), 3)).has_value());
                }
            }
        }

        remove("trieNodes.bin");
        remove("records.bin");
    }

    TEST(KeyValueStore, Stress) {
        BloomFilter bloomFilter(10000000);
        Log log(5000);
        File trieFile("trieNodes.bin");
        File recordFile("records.bin");
        TrieNodeSerializer trieNodeSerializer(Id::getIdSize());
        RecordSerializer recordSerializer(3, 3);
        Storage<TrieNode> trieNodeStorage(trieFile, trieNodeSerializer);
        Storage<Record> recordStorage(recordFile, recordSerializer);

        Trie trie(trieNodeStorage);

        KeyValueStore keyValueStore(bloomFilter, log, trie, recordStorage);

        for (char c1 = 'a'; c1 <= 'z'; ++c1) {
            for (char c2 = 'a'; c2 <= 'z'; ++c2) {
                for (char c3 = 'a'; c3 <= 'z'; ++c3) {
                    std::string s;
                    s.push_back(c1);
                    s.push_back(c2);
                    s.push_back(c3);
                    keyValueStore.add(KeyValue(
                            Key(s.data(), 3),
                            Value(s.data(), 3)));
                }
            }
        }


        for (char c1 = 'a'; c1 <= 'z'; ++c1) {
            for (char c2 = 'a'; c2 <= 'z'; ++c2) {
                for (char c3 = 'a'; c3 <= 'z'; ++c3) {
                    std::string s;
                    s.push_back(c1);
                    s.push_back(c2);
                    s.push_back(c3);
                    EXPECT_TRUE(keyValueStore.get(Key(s.data(), 3)).has_value());
                }
            }
        }

        for (char c1 = 'a'; c1 <= 'v'; ++c1) {
            for (char c2 = 'a'; c2 <= 'v'; ++c2) {
                for (char c3 = 'a'; c3 <= 'v'; ++c3) {
                    std::string s;
                    s.push_back(c1);
                    s.push_back(c2);
                    s.push_back(c3);
                    keyValueStore.del(Key(s.data(), 3));
                }
            }
        }

        for (char c1 = 'w'; c1 <= 'z'; ++c1) {
            for (char c2 = 'w'; c2 <= 'z'; ++c2) {
                for (char c3 = 'w'; c3 <= 'z'; ++c3) {
                    std::string s;
                    s.push_back(c1);
                    s.push_back(c2);
                    s.push_back(c3);
                    EXPECT_TRUE(keyValueStore.get(Key(s.data(), 3)).has_value());
                }
            }
        }

        for (char c1 = 'a'; c1 <= 'v'; ++c1) {
            for (char c2 = 'a'; c2 <= 'v'; ++c2) {
                for (char c3 = 'a'; c3 <= 'v'; ++c3) {
                    std::string s;
                    s.push_back(c1);
                    s.push_back(c2);
                    s.push_back(c3);
                    EXPECT_FALSE(keyValueStore.get(Key(s.data(), 3)).has_value());
                }
            }
        }

        remove("trieNodes.bin");
        remove("records.bin");
    }

}

