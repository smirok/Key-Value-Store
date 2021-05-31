#include "RecordSerializer.hpp"
#include "Value.hpp"

#include <cstring>
#include <iostream>

namespace kvs {

    RecordSerializer::RecordSerializer(std::size_t key_size, std::size_t value_size) : _key_size(key_size),
                                                                                           _value_size(value_size) {
    }

    char *RecordSerializer::trieNodeToBytes(const Record &trieNode) {
        char *result = new char[_key_size + 1 + _value_size];

        Key key = trieNode.getKey();
        bool isOutdated = trieNode.getIsOutdated();
        Value value = trieNode.getValue();

        std::memcpy(result, key.getKey(), key.getSize());
        result += key.getSize();

        *result = isOutdated;
        result += 1;

        std::memcpy(result, value.getValue(), value.getSize());
        result += value.getSize();

        return result - (_key_size + 1 + _value_size);
    }

    Record RecordSerializer::bytesToTrieNode(const char *bytes) {
        char *keyData = new char[_key_size + 1];
        memcpy(keyData, bytes, _key_size);
        bytes += _key_size;
        keyData[_key_size] = '\0';

        bool isOutdated = *bytes;
        bytes += 1;

        char *valueData = new char[_value_size + 1];
        memcpy(valueData, bytes, _value_size);
        valueData[_value_size] = '\0';

        return Record(Key(keyData, _key_size), isOutdated, Value(valueData, _value_size));
    }

}
