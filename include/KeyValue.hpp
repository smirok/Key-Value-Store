#ifndef KEYVALUESTORAGE_KEYVALUE_HPP
#define KEYVALUESTORAGE_KEYVALUE_HPP

#include "Value.hpp"
#include "Key.hpp"

namespace kvs {

    class KeyValue {
    public:
        KeyValue(const char *key, size_t key_size, const char *value, size_t value_size);

        Key getKey() const;

        Value getValue() const;
    };

}

#endif //KEYVALUESTORAGE_KEYVALUE_HPP
