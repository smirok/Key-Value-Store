#include <cstring>
#include "model/Value.hpp"

namespace kvs {

    Value::Value(const char *value, size_t size) : _size(size) {
        _value = new char[size + 1];
        memcpy(_value, value, size);
        _value[size] = '\0';
    }

    Value::Value(const Value &value) : _size(value._size) {
        _value = new char[_size + 1];
        memcpy(_value, value._value, _size);
        _value[_size] = '\0';
    }

    Value &Value::operator=(const Value &value) {
        if (this == &value)
            return *this;

        _size = value._size;
        _value = new char[_size + 1];
        memcpy(_value, value._value, _size);
        _value[_size] = '\0';

        return *this;
    }

    Value::Value(Value &&value) noexcept: _value(value._value), _size(value._size) {
        value._value = nullptr;
    }

    Value &Value::operator=(Value &&value) noexcept {
        if (&value == this) {
            return *this;
        }

        delete[] _value;

        _value = value._value;
        value._value = nullptr;

        _size = value._size;

        return *this;
    }

    Value::~Value() {
        delete[] _value;
    }

    const char *Value::getValue() const {
        return _value;
    }

    size_t Value::getSize() const {
        return _size;
    }
}