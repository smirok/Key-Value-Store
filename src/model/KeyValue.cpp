#include "model/KeyValue.hpp"
#include <utility>

namespace kvs {

    KeyValue::KeyValue(const char *key, size_t key_size, const char *value, size_t value_size)
            : _key(Key(key, key_size)), _value(value, value_size) {

    }

    KeyValue::KeyValue(Key key, Value value) : _key(std::move(key)), _value(std::move(value)) {

    }

    const Key &KeyValue::getKey() const {
        return _key;
    }

    const Value &KeyValue::getValue() const {
        return _value;
    }
}