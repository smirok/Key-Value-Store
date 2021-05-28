#ifndef KEYVALUESTORAGE_VALUE_HPP
#define KEYVALUESTORAGE_VALUE_HPP

#include <cstddef>

namespace kvs {

    class Value {
    public:
        Value(const char *_value, size_t _size);

        const char *getValue() const;

        size_t getSize() const;

    private:
        const char *_value;
        size_t _size;
    };

}

#endif //KEYVALUESTORAGE_VALUE_HPP
