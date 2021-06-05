#include "serializer/RecordSerializer.hpp"
#include "model/Value.hpp"

#include <cstring>
#include <iostream>

namespace kvs {

    RecordSerializer::RecordSerializer(std::size_t key_size, std::size_t value_size) : _key_size(key_size),
                                                                                       _value_size(value_size) {
    }

    char *RecordSerializer::recordToBytes(const Record &record) {
        char *result = new char[_key_size + 2 + _value_size];

        const Key &key = record.getKey();
        bool isOutdated = record.getIsOutdated();
        const Value &value = record.getValue();

        std::memcpy(result, key.getKey(), key.getSize());
        result += key.getSize();

        *result = isOutdated;
        result += 1;

        std::memcpy(result, value.getValue(), value.getSize());
        result += value.getSize();

        *result = '\0';
        result += 1;

        return result - (_key_size + 2 + _value_size);
    }

    Record RecordSerializer::bytesToRecord(const char *bytes) {
        std::string keyData(bytes, _key_size);
        bytes += _key_size;

        bool isOutdated = *bytes;
        bytes += 1;

        std::string valueData(bytes, _value_size);

        Key keyS(keyData.data(), _key_size);
        Value valueS(valueData.data(), _value_size);
        return Record(keyS, isOutdated, valueS);
    }

    std::size_t RecordSerializer::getKeySize() const {
        return _key_size;
    }

    std::size_t RecordSerializer::getValueSize() const {
        return _value_size;
    }

}
