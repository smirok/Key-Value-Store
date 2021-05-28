#include "Value.hpp"

namespace kvs {

    Value::Value(const char *value, size_t size) : _value(value), _size(size) {
    }

    const char *Value::getValue() const {
        return _value;
    }

    size_t Value::getSize() const {
        return _size;
    }
}