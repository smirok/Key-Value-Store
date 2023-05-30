#ifndef KEYVALUESTORAGE_KEYVALUE_HPP
#define KEYVALUESTORAGE_KEYVALUE_HPP

#include "model/Value.hpp"
#include "model/Key.hpp"

namespace kvs {

    /**
     * \brief Class representing both key and value.
     */
    class KeyValue {
    public:
        KeyValue(const char *key, size_t key_size, const char *value, size_t value_size);

        KeyValue(Key key, Value value);

        [[nodiscard]] const Key &getKey() const;

        [[nodiscard]] const Value &getValue() const;

    private:
        Key _key;
        Value _value;
    };

} // kvs

#endif //KEYVALUESTORAGE_KEYVALUE_HPP
