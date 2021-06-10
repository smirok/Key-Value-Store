#ifndef KEYVALUESTORAGE_TRIENODE_HPP
#define KEYVALUESTORAGE_TRIENODE_HPP

#include <vector>
#include "Id.hpp"
#include "Key.hpp"

namespace kvs {

    class TrieNode {
    public:
        TrieNode() = default;

        explicit TrieNode(std::vector<Id> nextRecords);

        [[nodiscard]] const std::vector<Id> &getNextRecords() const;

        [[nodiscard]] Id getNextRecord(std::size_t index) const;

    private:
        std::vector<Id> _nextRecords;
    };

}

#endif //KEYVALUESTORAGE_TRIENODE_HPP
