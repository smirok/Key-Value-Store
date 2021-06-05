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
    RecordSerializer recordSerializer = RecordSerializer(3, 11);

    std::string key = "key";
    std::string value = "abracadabra";
    Key keyS(key.data(), 3);
    Value valueS(value.data(), 11);
    Record record = Record(keyS, false, valueS);
    char *trieNodeInBytes = recordSerializer.recordToBytes(record);

    Record newRecord = recordSerializer.bytesToRecord(trieNodeInBytes);
    (void)newRecord;

    delete[] trieNodeInBytes;
    return 0;
}
