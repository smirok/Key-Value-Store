#ifndef KEYVALUESTORAGE_TRIENODE_HPP
#define KEYVALUESTORAGE_TRIENODE_HPP

#include <vector>
#include "Id.hpp"
#include "Key.hpp"

namespace kvs {

    /**
     * \brief Class representing the trie node for storing on the disk.
     */
    class TrieNode {
    public:
        TrieNode() = default;

        explicit TrieNode(std::vector<Id> nextRecords);

        [[nodiscard]] const std::vector<Id> &getNextRecords() const;

        [[nodiscard]] Id getNextRecord(std::size_t index) const;

    private:
        /** Vector of children-nodes identifiers. */
        std::vector<Id> _nextRecords;
    };

} // kvs

#endif //KEYVALUESTORAGE_TRIENODE_HPP
