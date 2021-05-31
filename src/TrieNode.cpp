#include "TrieNode.hpp"

#include <utility>

namespace kvs {

    TrieNode::TrieNode(std::vector<Id> nextRecords) : _nextRecords(std::move(nextRecords)) {
    }

    std::vector<Id> TrieNode::getNextRecords() const {
        return _nextRecords;
    }

}
