#ifndef KEYVALUESTORAGE_INMEMORYTRIENODE_HPP
#define KEYVALUESTORAGE_INMEMORYTRIENODE_HPP

#include "Key.hpp"
#include "Id.hpp"

namespace kvs {

    class InMemoryTrieNode {
    public:
        InMemoryTrieNode();

        ~InMemoryTrieNode();

        void add(const Key &key, const Id &id);

        InMemoryTrieNode *get(std::size_t index) const;

        void set(std::size_t index, InMemoryTrieNode *trieNode);

    private:
        InMemoryTrieNode *_children[256];
        Id _id;
    };

}

#endif //KEYVALUESTORAGE_INMEMORYTRIENODE_HPP
