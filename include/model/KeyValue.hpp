#ifndef KEYVALUESTORAGE_KEYVALUE_HPP
#define KEYVALUESTORAGE_KEYVALUE_HPP

#include "model/Value.hpp"
#include "model/Key.hpp"

namespace kvs {

    class KeyValue {
    public:
        KeyValue(const char *key, size_t key_size, const char *value, size_t value_size);

        KeyValue(const Key &key, const Value &value);

        Key getKey() const;

        Value getValue() const;

    private:
        Key _key;
        Value _value;
    };

}

#endif //KEYVALUESTORAGE_KEYVALUE_HPP
