#include "model/InMemoryTrieNode.hpp"

namespace kvs {

    InMemoryTrieNode::InMemoryTrieNode() {
        for (int i = 0; i < 256; ++i) {
            _children[i] = nullptr;
        }
    }

    InMemoryTrieNode::~InMemoryTrieNode() {
        for (auto &i : _children) {
            delete i;
        }
        delete this;
    }

    void InMemoryTrieNode::add(const Key &key, const Id &id) {
        InMemoryTrieNode *current = this;

        const char *bytesKey = key.getKey();
        for (std::size_t i = 0; i < key.getSize(); ++i) {
            std::size_t index = static_cast<unsigned char>(*bytesKey);
            if (_children[index] == nullptr) {
                InMemoryTrieNode *newNode = new InMemoryTrieNode();
                current->set(index, newNode);
                _children[index] = newNode;
            }

            current = _children[index];
        }

        current->_id = id;
    }

    InMemoryTrieNode *InMemoryTrieNode::get(std::size_t index) const {
        return _children[index];
    }

    void InMemoryTrieNode::set(std::size_t index, InMemoryTrieNode *trieNode) {
        _children[index] = trieNode;
    }
}