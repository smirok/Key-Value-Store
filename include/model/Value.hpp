#ifndef KEYVALUESTORAGE_VALUE_HPP
#define KEYVALUESTORAGE_VALUE_HPP

#include <cstddef>

namespace kvs {

    /**
     * \brief Class representing key and its size.
     */
    class Value {
    public:
        Value(const char *_value, size_t _size);

        Value(const Value &value);

        Value &operator=(const Value &value);

        Value(Value &&value) noexcept;

        Value &operator=(Value &&value) noexcept;

        ~Value();

        [[nodiscard]] const char *getValue() const;

        [[nodiscard]] size_t getSize() const;

    private:
        char *_value;
        size_t _size;
    };

} // kvs

#endif //KEYVALUESTORAGE_VALUE_HPP
