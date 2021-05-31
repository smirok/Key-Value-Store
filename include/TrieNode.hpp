#ifndef KEYVALUESTORAGE_TRIENODE_HPP
#define KEYVALUESTORAGE_TRIENODE_HPP

#include "Key.hpp"
#include "Value.hpp"
#include <vector>

namespace kvs {

    class TrieNode {
    public:
        TrieNode(const Key& key, bool isOutdated, const Value& value);

        Key getKey() const;

        bool getIsOutdated() const;

        Value getValue() const;

    private:
        Key _key;
        bool _isOutdated;
        Value _value;
    };

}

#endif //KEYVALUESTORAGE_TRIENODE_HPP
