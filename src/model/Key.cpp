#include "model/Key.hpp"
#include <cstring>
#include <iostream>

namespace kvs {

    Key::Key(const char *key, size_t size) : _size(size) {
        _key = new char[size + 1];
        memcpy((void *) (_key), key, size);
        _key[size] = '\0';
    }

    Key::~Key() {
        delete[] _key;
    }

    const char *Key::getKey() const {
        return _key;
    }

    size_t Key::getSize() const {
        return _size;
    }

    bool Key::operator==(const Key &key) const {
        return (strcmp(this->_key, key.getKey()) == 0);
    }

    Key &Key::operator=(const Key &key) {
        if (this == &key)
            return *this;

        _size = key._size;
        _key = new char[_size + 1];
        memcpy(key._key, _key, _size);
        _key[_size] = '\0';

        return *this;
    }

    Key::Key(const Key &key) : _size(key._size) {
        _key = new char[_size + 1];
        memcpy(_key, key._key, _size);
        _key[_size] = '\0';
    }
}