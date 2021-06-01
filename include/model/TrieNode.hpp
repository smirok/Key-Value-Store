#ifndef KEYVALUESTORAGE_TRIENODE_HPP
#define KEYVALUESTORAGE_TRIENODE_HPP

#include <vector>
#include "Id.hpp"

namespace kvs {

    class TrieNode {
    public:
        TrieNode(std::vector<Id> nextRecords);

        std::vector<Id> getNextRecords() const;

    private:
        std::vector<Id> _nextRecords;
    };

}

#endif //KEYVALUESTORAGE_TRIENODE_HPP
