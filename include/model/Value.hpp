#ifndef KEYVALUESTORAGE_VALUE_HPP
#define KEYVALUESTORAGE_VALUE_HPP

#include <cstddef>

namespace kvs {

    class Value {
    public:
        Value(const char *_value, size_t _size);

        Value &operator=(const Value &value);

        Value(const Value& value);

        ~Value();

        const char *getValue() const;

        size_t getSize() const;

    private:
        char *_value;
        size_t _size;
    };

}

#endif //KEYVALUESTORAGE_VALUE_HPP
