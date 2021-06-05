#include <iostream>
#include <storage/BloomFilter.hpp>
#include <serializer/RecordSerializer.hpp>
#include <serializer/TrieNodeSerializer.hpp>
#include <storage/File.hpp>
#include <storage/Log.hpp>
#include <storage/Storage.hpp>
#include <storage/Trie.hpp>
#include <KeyValueStore.hpp>

int main() {
    using namespace kvs;
    BloomFilter bloomFilter(1000000);
    Log log(10000);
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

    return 0;
}
