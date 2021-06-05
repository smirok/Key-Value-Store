#include "storage/Log.hpp"

namespace kvs {

    Log::Log(std::size_t sizeLimit) : _sizeLimit(sizeLimit) {
        _logMap.reserve(sizeLimit);
    }

    std::optional<Id> Log::add(const Key &key, Id id) {
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

    std::optional<Id> Log::get(const Key &key) {
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

    InMemoryTrieNode *Log::toInMemoryTrieNode() {
        InMemoryTrieNode *root = new InMemoryTrieNode();

        for (const auto &pair : _logMap) {
            root->add(pair.first, pair.second);
        }

        return root;
    }

    InMemoryTrieNode *Log::toInMemoryTrieNode(std::vector<std::pair<Key, Id>> &keyIdPairs) {
        InMemoryTrieNode *root = new InMemoryTrieNode();

        for (const auto &pair : keyIdPairs) {
            root->add(pair.first, pair.second);
        }

        return root;
    }
}