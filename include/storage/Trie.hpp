#ifndef KEYVALUESTORAGE_TRIE_HPP
#define KEYVALUESTORAGE_TRIE_HPP

#include <model/Key.hpp>
#include <model/Id.hpp>
#include <model/TrieNode.hpp>
#include <model/InMemoryTrieNode.hpp>
#include "storage/Storage.hpp"
#include "LogicStorage.hpp"

namespace kvs {

    /**
     * \brief Class representing trie storage on the disk, which stored pairs < @p Key, @p Id>.
     */
    class Trie : public LogicStorage {
    public:
        /**
         *
         * @param _storage Storage @p TrieNode of trie nodes, responsible only for access to nodes.
         */
        explicit Trie(Storage<TrieNode> &_storage);

        /**
         * Insert value @p id in @p Trie by @p key.
         * If there was some identifier in @p Trie by @p key, then @p Id will be updated
         * @param key Key.
         * @param id Identifier.
         * @return @p Id wrapped into @p std::optional, if there was some identifier in @p Trie by @p key;
         * empty @p std::optional otherwise.
         */
        std::optional<Id> add(const Key &, const Id &) override;

        /**
         * Remove key-value pair with @p key.
         * @param key Key.
         * @return @p Id wrapped into @p std::optional, if there was some identifier in @p Trie by @p key;
         * empty @p std::optional otherwise.
         */
        std::optional<Id> remove(const Key &) override;

        /**
        * Get @p Id by @p key.
        * @param key Key.
        * @return @p Id wrapped into @p std::optional, if there was some identifier in @p Trie by @p key;
        * empty @p std::optional otherwise.
        */
        [[nodiscard]] std::optional<Id> get(const Key &) const override;

        /**
         * Clean up @p Trie.
         */
        void clear() override;

        /**
         * Merge the RAM trie with the root @p smallTrieRoot in current trie.
         * @param smallTrieRoot Root of the RAM trie.
         * @return Array of outdated @p Id after merge.
         */
        const std::vector<Id> &merge(const std::shared_ptr<InMemoryTrieNode> &smallTrieRoot);

    private:
        std::vector<Id> _recordsToDelete;

        void addRoot();

        Id merge(const Id &trieNodeId, const std::shared_ptr<InMemoryTrieNode> &smallTrieRoot);

        [[nodiscard]] std::optional<std::pair<TrieNode, Id>> traverse(const Key &key, bool shouldCreateNode) const;

        Storage<TrieNode> &_storage;
    };
} // kvs

#endif //KEYVALUESTORAGE_TRIE_HPP
