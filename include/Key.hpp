#ifndef KEYVALUESTORAGE_KEY_HPP
#define KEYVALUESTORAGE_KEY_HPP

#include <cstddef>

namespace kvs {

    class Key {
        Key(const char *value, size_t size);

        const char *getKey() const;

        size_t getSize() const;
    };

}

#endif //KEYVALUESTORAGE_KEY_HPP
