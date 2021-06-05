#include <cstring>
#include "model/Value.hpp"

namespace kvs {

    Value::Value(const char *value, size_t size) : _size(size) {
        _value = new char[size + 1];
        memcpy((void *) (_value), value, size);
        _value[size] = '\0';
    }

    const char *Value::getValue() const {
        return _value;
    }

    size_t Value::getSize() const {
        return _size;
    }

    Value::~Value() {
        delete[] _value;
    }

    Value &Value::operator=(const Value &value) {
        if (this == &value)
            return *this;

        _size = value._size;
        _value = new char[_size + 1];
        memcpy(value._value, _value, _size);
        _value[_size] = '\0';

        return *this;
    }

    Value::Value(const Value &value) : _size(value._size){
        _value = new char[_size + 1];
        memcpy(_value, value._value, _size);
        _value[_size] = '\0';
    }
}