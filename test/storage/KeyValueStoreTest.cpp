#include "testing.h"
#include "KeyValueStore.hpp"

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
    }

}

