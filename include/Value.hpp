#ifndef KEYVALUESTORAGE_VALUE_HPP
#define KEYVALUESTORAGE_VALUE_HPP

#include <cstddef>

namespace kvs {

    class Value {
    public:
        Value(const char *value, size_t size);

        const char *getValue() const;

        size_t getSize() const;
    };

}

#endif //KEYVALUESTORAGE_VALUE_HPP
