#include <iostream>
#include <random>
#include <chrono>
#include <storage/BloomFilter.hpp>
#include <serializer/RecordSerializer.hpp>
#include <serializer/TrieNodeSerializer.hpp>
#include <storage/File.hpp>
#include <storage/Log.hpp>
#include <storage/Storage.hpp>
#include <storage/Trie.hpp>
#include <KeyValueStore.hpp>

std::mt19937 random_data(2);

int main() {
    using namespace kvs;

    std::size_t N; // количество элементов в хранилище на момент начала теста
    std::size_t M; // количество запросов, обращенных к хранилищу в процессе тестирования
    std::size_t p; // целое число от 0 до 100 -- процент запросов на чтение

    std::cout << "Введите кол-во элементов в хранилище на момент начала теста\n";
    std::cin >> N;
    std::cout << "Введите количество запросов, обращенных к хранилищу в процессе тестирования\n";
    std::cin >> M;
    std::cout << "Введите целое число от 0 до 100 -- процент запросов на чтение\n";
    std::cin >> p;

    std::size_t bloomFilterSize = N * 8;
    std::size_t logSize = N / 20;
    std::size_t keySize = 2;
    std::size_t valueSize = 1;

    BloomFilter bloomFilter(bloomFilterSize);
    Log log(logSize);

    File trieFile("data/trieNodes.bin");
    File recordFile("data/records.bin");

    TrieNodeSerializer trieNodeSerializer(Id::getIdSize());
    RecordSerializer recordSerializer(keySize, valueSize);

    Storage<TrieNode> trieNodeStorage(trieFile, trieNodeSerializer);
    Storage<Record> recordStorage(recordFile, recordSerializer);

    Trie trie(trieNodeStorage);

    KeyValueStore keyValueStore(bloomFilter, log, trie, recordStorage);

    std::size_t storeSize = 0;
    int cnt = 0;
    while (storeSize < N) {
        cnt++;
        std::string keyString, valueString;

        for (std::size_t j = 0; j < keySize; ++j) {
            char sym = static_cast<char>((((random_data() % 128) + 128) % 128));
            keyString.push_back(sym);
        }

        for (std::size_t j = 0; j < valueSize; ++j) {
            char sym = static_cast<char>((((random_data() % 128) + 128) % 128));
            valueString.push_back(sym);
        }

        Key key(keyString.c_str(), keyString.size());
        Value value(valueString.c_str(), valueString.size());

        if (!keyValueStore.get(key).has_value()) {
            storeSize++;
            keyValueStore.add(KeyValue(key, value));
        }
    }

    std::size_t readOperations = (M * p) / 100;
    std::size_t writeOperations = (M * (100 - p)) / 100;

    long double summaryWriteTime = 0;
    long double summaryReadTime = 0;

    long double maxWriteTime = 0;
    long double minWriteTime = 1e9;

    long double maxReadTime = 0;
    long double minReadTime = 1e9;

    for (std::size_t i = 0; i < writeOperations; i++) {
        std::string keyString, valueString;

        for (std::size_t j = 0; j < keySize; ++j) {
            char sym = static_cast<char>((((random_data() % 128) + 128) % 128));
            keyString.push_back(sym);
        }

        for (std::size_t j = 0; j < valueSize; ++j) {
            char sym = static_cast<char>((((random_data() % 128) + 128) % 128));
            valueString.push_back(sym);
        }

        Key key(keyString.c_str(), keyString.size());
        Value value(valueString.c_str(), valueString.size());
        KeyValue keyValue(key, value);

        auto startTime = std::chrono::steady_clock::now();

        keyValueStore.add(keyValue);

        auto finishTime = std::chrono::steady_clock::now();

        long double operationTime = std::chrono::duration_cast<std::chrono::microseconds>(
                finishTime - startTime).count();

        summaryWriteTime += operationTime;
        maxWriteTime = std::max(maxWriteTime, operationTime);
        minWriteTime = std::min(minWriteTime, operationTime);
    }

    for (std::size_t i = 0; i < readOperations; i++) {
        std::string keyString;

        for (std::size_t j = 0; j < keySize; ++j) {
            char sym = static_cast<char>((((random_data() % 128) + 128) % 128));
            keyString.push_back(sym);
        }

        Key key(keyString.c_str(), keyString.size());

        auto startTime = std::chrono::steady_clock::now();

        (void) keyValueStore.get(key);

        auto finishTime = std::chrono::steady_clock::now();

        long double operationTime = std::chrono::duration_cast<std::chrono::microseconds>(
                finishTime - startTime).count();

        summaryReadTime += operationTime;
        maxReadTime = std::max(maxReadTime, operationTime);
        minReadTime = std::min(minReadTime, operationTime);
    }

    std::cout << "Average query time : " << (summaryReadTime + summaryWriteTime) / (readOperations + writeOperations)
              << " microseconds \n";
    if (readOperations > 0) {
        std::cout << "Average read query time : " << summaryReadTime / readOperations << " microseconds \n";
        std::cout << "Max read query time : " << maxReadTime << " microseconds \n";
        std::cout << "Min read query time : " << minReadTime << " microseconds \n";
    }
    if (writeOperations > 0) {
        std::cout << "Average write query time : " << summaryWriteTime / writeOperations << " microseconds \n";
        std::cout << "Max write query time : " << maxWriteTime << " microseconds \n";
        std::cout << "Min write query time : " << minWriteTime << " microseconds \n";
    }

    remove("data/trieNodes.bin");
    remove("data/records.bin");
    return 0;
}
