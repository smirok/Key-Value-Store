#include "model/Key.hpp"
#include <cstring>
#include <iostream>

namespace kvs {

    Key::Key(const char *key, size_t size) : _size(size) {
        _key = new char[size + 1];
        memcpy(_key, key, size);
        _key[size] = '\0';
    }

    Key::Key(const Key &key) : _size(key._size) {
        _key = new char[_size + 1];
        memcpy(_key, key._key, _size);
        _key[_size] = '\0';
    }

    Key &Key::operator=(const Key &key) {
        if (this == &key)
            return *this;

        _size = key._size;
        _key = new char[_size + 1];
        memcpy(_key, key._key, _size);
        _key[_size] = '\0';

        return *this;
    }

    Key::Key(Key &&key) : _key(key._key), _size(key._size) {
        key._key = nullptr;
    }

    Key &Key::operator=(Key &&key) {
        if (&key == this) {
            return *this;
        }

        delete[] _key;

        _key = key._key;
        key._key = nullptr;

        _size = key._size;

        return *this;
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
}