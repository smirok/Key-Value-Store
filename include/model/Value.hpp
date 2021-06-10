#ifndef KEYVALUESTORAGE_VALUE_HPP
#define KEYVALUESTORAGE_VALUE_HPP

#include <cstddef>

namespace kvs {

    class Value {
    public:
        Value(const char *_value, size_t _size);

        Value(const Value &value);

        Value &operator=(const Value &value);

        Value(Value &&value) noexcept ;

        Value &operator=(Value &&value) noexcept ;

        ~Value();

        [[nodiscard]] const char *getValue() const;

        [[nodiscard]] size_t getSize() const;

    private:
        char *_value;
        size_t _size;
    };

}

#endif //KEYVALUESTORAGE_VALUE_HPP
