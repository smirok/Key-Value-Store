#include <iostream>
#include "storage/Log.hpp"
#include <cstring>

namespace kvs {

    Log::Log(std::size_t sizeLimit) : _sizeLimit(sizeLimit) {
        _logMap.reserve(sizeLimit);
    }

    std::optional<Id> Log::add(const Key &key, const Id &id) {
        std::optional<Id> replacedRecordId = get(key);

        if (replacedRecordId.has_value()) {
            _logMap[key] = id;
            return replacedRecordId;
        } else {
            _logMap[key] = id;
            return std::nullopt;
        }
    }

    std::optional<Id> Log::remove(const Key &key) {
        std::optional<Id> recordToRemoveId = get(key);
        if (recordToRemoveId == std::nullopt) {
            return std::nullopt;
        }

        _logMap.erase(key);
        return recordToRemoveId;
    }

    std::optional<Id> Log::get(const Key &key) const {
        if (_logMap.find(key) != _logMap.end()) {
            return std::optional<Id>(_logMap.at(key));
        }

        return std::nullopt;
    }

    void Log::clear() {
        _logMap.clear();
        _logMap.reserve(_sizeLimit);
    }

    bool Log::isFull() const {
        return _logMap.size() >= _sizeLimit;
    }

    std::shared_ptr<InMemoryTrieNode> Log::toInMemoryTrieNode() const {
        std::shared_ptr<InMemoryTrieNode> root = std::make_shared<InMemoryTrieNode>(InMemoryTrieNode());

        for (const auto &pair : _logMap) {
            InMemoryTrieNode::add(root, pair.first, pair.second);
        }

        return root;
    }

    std::shared_ptr<InMemoryTrieNode> Log::toInMemoryTrieNode(std::vector<std::pair<Key, Id>> &keyIdPairs) {
        std::shared_ptr<InMemoryTrieNode> root = std::make_shared<InMemoryTrieNode>(InMemoryTrieNode());

        for (const auto &pair : keyIdPairs) {
            InMemoryTrieNode::add(root, pair.first, pair.second);
        }

        return root;
    }
}