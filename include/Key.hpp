#ifndef KEYVALUESTORAGE_KEY_HPP
#define KEYVALUESTORAGE_KEY_HPP

#include <cstddef>

namespace kvs {

    class Key {
    public:
        Key(const char *key, size_t size);

        const char *getKey() const;

        size_t getSize() const;

    private:

        const char *_key;
        size_t _size;
    };

}

#endif //KEYVALUESTORAGE_KEY_HPP
