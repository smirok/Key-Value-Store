#include "model/Key.hpp"
#include <cstring>

namespace kvs {

    Key::Key(const char *key, size_t size) : _key(key), _size(size) {
    }

    const char *Key::getKey() const {
        return _key;
    }

    size_t Key::getSize() const {
        return _size;
    }

    bool Key::operator==(Key key) const {
        return std::strcmp(this->_key, key.getKey()) == 0;
    }
}