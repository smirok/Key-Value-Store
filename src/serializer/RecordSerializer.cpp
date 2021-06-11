#include "serializer/RecordSerializer.hpp"
#include "model/Value.hpp"

#include <cstring>

namespace kvs {

    RecordSerializer::RecordSerializer(std::size_t key_size, std::size_t value_size) : _key_size(key_size),
                                                                                       _value_size(value_size) {
    }

    char *RecordSerializer::recordToBytes(const Record &record) const {
        std::size_t buffer_size = _key_size + 1 + _value_size + 1;

        char *result = new char[buffer_size];

        const Key &key = record.getKey();
        bool isOutdated = record.getIsOutdated();
        const Value &value = record.getValue();

        std::memcpy(result, key.getKey(), key.getSize());
        result += key.getSize();

        *result = static_cast<char>(isOutdated);
        result += 1;

        std::memcpy(result, value.getValue(), value.getSize());
        result += value.getSize();

        *result = '\0';
        result += 1;

        return result - buffer_size;
    }

    Record RecordSerializer::bytesToRecord(const char *bytes) const {
        std::string keyData(bytes, _key_size);
        bytes += _key_size;

        bool isOutdated = *bytes;
        bytes += 1;

        std::string valueData(bytes, _value_size);

        Key key(keyData.c_str(), _key_size);
        Value value(valueData.c_str(), _value_size);
        return Record(key, isOutdated, value);
    }

    std::size_t RecordSerializer::getKeySize() const {
        return _key_size;
    }

    std::size_t RecordSerializer::getValueSize() const {
        return _value_size;
    }

}
