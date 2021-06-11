#include "model/InMemoryTrieNode.hpp"

#include <utility>

namespace kvs {

    InMemoryTrieNode::InMemoryTrieNode() {
        for (int i = 0; i <= UCHAR_MAX; ++i) {
            _children[i] = nullptr;
        }
    }

    void InMemoryTrieNode::add(std::shared_ptr<InMemoryTrieNode> currentNode,
                               const Key &key, const Id &id) {

        const char *bytesKey = key.getKey();
        for (std::size_t i = 0; i < key.getSize(); ++i) {
            std::size_t index = static_cast<unsigned char>(*bytesKey);
            if (!currentNode->_children[index]) {
                currentNode->_children[index] = std::make_shared<InMemoryTrieNode>(InMemoryTrieNode());
            }

            currentNode = currentNode->_children[index];
            bytesKey += 1;
        }

        currentNode->_id = id;
    }

    std::shared_ptr<InMemoryTrieNode> InMemoryTrieNode::get(std::size_t index) const {
        return _children[index];
    }

    Id InMemoryTrieNode::getId() const {
        return _id;
    }
}