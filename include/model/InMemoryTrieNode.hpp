#ifndef KEYVALUESTORAGE_INMEMORYTRIENODE_HPP
#define KEYVALUESTORAGE_INMEMORYTRIENODE_HPP

#include <memory>
#include <climits>
#include "Key.hpp"
#include "Id.hpp"

namespace kvs {

    /**
     * \brief A class implementing a trie node in RAM.
     */
    class InMemoryTrieNode {
    public:
        /**
         * Initialize children with @p nullptr.
         */
        InMemoryTrieNode();

        /**
         * @param currentNode Node to which @p key with @p id is attached as a root.
         * @param key The key we want to add to the tree @p currentNode.
         * @param id Identifier of the record corresponding to the @p key.
         */
        static void add(std::shared_ptr<InMemoryTrieNode> currentNode, const Key &key, const Id &id);

        /**
         * Turning @p Log-a into a @p Trie stored in RAM.
         * @param keyIdPairs Array of pairs < @p Key, @p Id - identifier of @p Record>, from which the trie will be built.
         * @return Pointer to the root of the resulting trie.
         */
        static std::shared_ptr<InMemoryTrieNode> toInMemoryTrieNode(std::vector<std::pair<Key, Id>> &keyIdPairs);

        /**
         * @param index Node's child number.
         * @return Pointer to the @p index of the child.
         */
        [[nodiscard]] std::shared_ptr<InMemoryTrieNode> get(std::size_t index) const;

        /**
         * @return Identifier of the corresponding node record. If the node does not match the record,
         * then @p std::numeric_limits<std::size_t>::max().
         */
        [[nodiscard]] Id getId() const;

    private:
        /** 256 @p shared_ptr for children of the current node. */
        std::shared_ptr<InMemoryTrieNode> _children[256];

        /**
         * If the node is a trie leaf, then @p _id.getId() declare record corresponding this key,
         * otherwise @p _id.getId() == std::numeric_limits<std::size_t>::max().
         */
        Id _id = Id();
    };

} // kvs

#endif //KEYVALUESTORAGE_INMEMORYTRIENODE_HPP
