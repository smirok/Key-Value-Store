#ifndef KEYVALUESTORAGE_TRIE_HPP
#define KEYVALUESTORAGE_TRIE_HPP

#include <model/Key.hpp>
#include <model/Id.hpp>
#include <model/TrieNode.hpp>
#include "storage/Storage.hpp"

namespace kvs {

    class Trie {
    public:
        Trie(Storage<TrieNode> &_storage);

        std::optional<Id> add(Key, Id); // old record id

        std::optional<Id> remove(Key);

        std::optional<Id> get(Key);

        void clear();

    private:
        void addRoot();

        std::optional<std::pair<TrieNode, Id>> traverse(Key, bool shouldCreateNode);

        Storage<TrieNode> &_storage;
    };
}

#endif //KEYVALUESTORAGE_TRIE_HPP
