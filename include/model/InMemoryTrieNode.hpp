#ifndef KEYVALUESTORAGE_INMEMORYTRIENODE_HPP
#define KEYVALUESTORAGE_INMEMORYTRIENODE_HPP

#include <memory>
#include "Key.hpp"
#include "Id.hpp"

namespace kvs {

    class InMemoryTrieNode {
    public:
        InMemoryTrieNode();

        static void add(std::shared_ptr<InMemoryTrieNode> currentNode, const Key &key, const Id &id);

        std::shared_ptr<InMemoryTrieNode> get(std::size_t index) const;

        void set(std::size_t index, const std::shared_ptr<InMemoryTrieNode> &trieNode);

        Id getId() const;

    private:
        std::shared_ptr<InMemoryTrieNode> _children[256];
        Id _id = Id();
    };

}

#endif //KEYVALUESTORAGE_INMEMORYTRIENODE_HPP
