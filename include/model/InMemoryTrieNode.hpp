#ifndef KEYVALUESTORAGE_INMEMORYTRIENODE_HPP
#define KEYVALUESTORAGE_INMEMORYTRIENODE_HPP

#include <memory>
#include "Key.hpp"
#include "Id.hpp"

namespace kvs {

    class InMemoryTrieNode {
    public:
        InMemoryTrieNode();

        void add(const Key &key, const Id &id);

        std::shared_ptr<InMemoryTrieNode> get(std::size_t index) const;

        void set(std::size_t index, std::shared_ptr<InMemoryTrieNode> trieNode);

    private:
        std::shared_ptr<InMemoryTrieNode> _children[256];
        Id _id;
    };

}

#endif //KEYVALUESTORAGE_INMEMORYTRIENODE_HPP
