#include "model/KeyValue.hpp"

namespace kvs {

    KeyValue::KeyValue(const char *key, size_t key_size, const char *value, size_t value_size)
            : _key(Key(key, key_size)), _value(value, value_size) {

    }

    KeyValue::KeyValue(const Key &key, const Value &value) : _key(key), _value(value) {

    }

    Key KeyValue::getKey() const {
        return _key;
    }

    Value KeyValue::getValue() const {
        return _value;
    }
}