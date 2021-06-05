#include "model/InMemoryTrieNode.hpp"

#include <utility>

namespace kvs {

    InMemoryTrieNode::InMemoryTrieNode() {
        for (int i = 0; i < 256; ++i) {
            _children[i] = nullptr;
        }
    }

    void InMemoryTrieNode::add(const Key &key, const Id &id) {
        std::shared_ptr<InMemoryTrieNode> current = std::make_shared<InMemoryTrieNode>(*this);

        const char *bytesKey = key.getKey();
        for (std::size_t i = 0; i < key.getSize(); ++i) {
            std::size_t index = static_cast<unsigned char>(*bytesKey);
            if (_children[index] == nullptr) {
                std::shared_ptr<InMemoryTrieNode> newNode = std::make_shared<InMemoryTrieNode>(InMemoryTrieNode());
                current->set(index, newNode);
                _children[index] = newNode;
            }

            current = _children[index];
        }

        current->_id = id;
    }

    std::shared_ptr<InMemoryTrieNode> InMemoryTrieNode::get(std::size_t index) const {
        return _children[index];
    }

    void InMemoryTrieNode::set(std::size_t index, std::shared_ptr<InMemoryTrieNode> trieNode) {
        _children[index] = std::move(trieNode);
    }
}