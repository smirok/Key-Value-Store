#ifndef KEYVALUESTORAGE_TRIE_HPP
#define KEYVALUESTORAGE_TRIE_HPP

#include <model/Key.hpp>
#include <model/Id.hpp>
#include <model/TrieNode.hpp>
#include <model/InMemoryTrieNode.hpp>
#include "storage/Storage.hpp"
#include "LogicStorage.hpp"

namespace kvs {

    class Trie : public LogicStorage {
    public:
        Trie(Storage<TrieNode> &_storage);

        std::optional<Id> add(const Key &, Id); // old record id

        std::optional<Id> remove(const Key &);

        std::optional<Id> get(const Key &);

        void clear();

        void merge(std::shared_ptr<InMemoryTrieNode> smallTrieRoot);

    private:
        void addRoot();

        Id merge(const Id &trieNodeId, std::shared_ptr<InMemoryTrieNode> smallTrieRoot);

        std::optional<std::pair<TrieNode, Id>> traverse(Key, bool shouldCreateNode);

        Storage<TrieNode> &_storage;
    };
}

#endif //KEYVALUESTORAGE_TRIE_HPP
