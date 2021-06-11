#include <iostream>
#include <string>
#include <storage/BloomFilter.hpp>
#include <storage/Log.hpp>
#include <storage/File.hpp>
#include <serializer/TrieNodeSerializer.hpp>
#include <serializer/RecordSerializer.hpp>
#include <storage/Storage.hpp>
#include <storage/Trie.hpp>
#include <KeyValueStore.hpp>

int main() {
    using namespace kvs;

    std::cout << "KeyValueStorage\n"
                 "Enter the key size to use:\n$ ";

    std::size_t keySize;
    std::cin >> keySize;

    std::cout << "Enter the value size to use:\n$ ";

    std::size_t valueSize;
    std::cin >> valueSize;

    BloomFilter bloomFilter(10000000);
    Log log(100000);

    File trieFile("data/trieNodes.bin");
    File recordFile("data/records.bin");

    TrieNodeSerializer trieNodeSerializer(Id::getIdSize());
    RecordSerializer recordSerializer(keySize, valueSize);

    Storage<TrieNode> trieNodeStorage(trieFile, trieNodeSerializer);
    Storage<Record> recordStorage(recordFile, recordSerializer);

    Trie trie(trieNodeStorage);

    KeyValueStore keyValueStore(bloomFilter, log, trie, recordStorage);

    while (true) {
        std::cout << "$ ";
        std::string command;
        std::cin >> command;

        if (command == "exit") {
            return 0;
        } else if (command == "add") {
            std::string key, value;
            std::cin >> key >> value;
            if (key.size() != keySize) {
                std::cout << "Try again. Expected key of length " << keySize << "\n";
                continue;
            }
            if (value.size() != valueSize) {
                std::cout << "Try again. Expected value of length " << valueSize << "\n";
                continue;
            }
            keyValueStore.add(KeyValue(key.c_str(), keySize, value.c_str(), valueSize));
        } else if (command == "get") {
            std::string key;
            std::cin >> key;
            if (key.size() != keySize) {
                std::cout << "Try again. Expected key of length " << keySize << "\n";
                continue;
            }
            std::optional<KeyValue> keyValueOpt = keyValueStore.get(Key(key.c_str(), keySize));
            if (keyValueOpt.has_value()) {
                std::cout << keyValueOpt->getValue().getValue() << '\n';
            } else {
                std::cout << "This key is not in store\n";
            }
        } else if (command == "del") {
            std::string key;
            std::cin >> key;
            if (key.size() != keySize) {
                std::cout << "Try again. Expected key of length " << keySize << "\n";
                continue;
            }
            keyValueStore.del(Key(key.c_str(), keySize));
        } else if (command == "clear") {
            keyValueStore.clear();
            std::cout << "Store was cleared.\n";
        } else {
            getline(std::cin, command);
            std::cout << "Unknown command\n";
        }
    }

    return 0;
}