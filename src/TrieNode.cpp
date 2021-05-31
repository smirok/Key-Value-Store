#include "TrieNode.hpp"

namespace kvs {

    TrieNode::TrieNode(const Key &key, bool isOutdated, const Value &value) : _key(key),
                                                                              _isOutdated(isOutdated),
                                                                              _value(value) {
    }

    Key TrieNode::getKey() const {
        return _key;
    }

    bool TrieNode::getIsOutdated() const {
        return _isOutdated;
    }

    Value TrieNode::getValue() const {
        return _value;
    }
}