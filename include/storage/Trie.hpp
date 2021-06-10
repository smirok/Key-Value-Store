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
        explicit Trie(Storage<TrieNode> &_storage);

        std::optional<Id> add(const Key &, const Id &) override;

        std::optional<Id> remove(const Key &) override;

        [[nodiscard]] std::optional<Id> get(const Key &) const override;

        void clear() override;

        void merge(const std::shared_ptr<InMemoryTrieNode> &smallTrieRoot);

    private:
        std::vector<Id> _recordsToDelete;
    public:
        const std::vector<Id> &getRecordsToDelete() const;

    private:

        void addRoot();

        Id merge(const Id &trieNodeId, const std::shared_ptr<InMemoryTrieNode> &smallTrieRoot);

        [[nodiscard]] std::optional<std::pair<TrieNode, Id>> traverse(const Key &key, bool shouldCreateNode) const;

        Storage<TrieNode> &_storage;
    };
}

#endif //KEYVALUESTORAGE_TRIE_HPP
