#include "model/TrieNode.hpp"

#include <utility>

namespace kvs {

    TrieNode::TrieNode(std::vector<Id> nextRecords) : _nextRecords(std::move(nextRecords)) {
    }

    Id TrieNode::getNextRecord(std::size_t index) const {
        return _nextRecords[index];
    }

    void TrieNode::setNextRecord(std::size_t index, const Id &id) {
        _nextRecords[index] = id;
    }

    std::vector<Id> TrieNode::getNextRecords() const {
        return _nextRecords;
    }
}
