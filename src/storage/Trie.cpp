#include "storage/Trie.hpp"
#include <limits>

namespace kvs {
    // nextRecords[i] == Id(std::numeric_limits<std::size_t>::max()) IFF edge is missing

    Trie::Trie(Storage<TrieNode> &storage) : _storage(storage) {
        addRoot();
    }

    std::optional<Id> Trie::add(const Key &key, Id recordId) {
        std::optional<std::pair<TrieNode, Id>> currentNode = traverse(key, true);

        std::optional<Id> oldRecordId = std::nullopt;

        if (currentNode->first.getNextRecords()[0].getId() != std::numeric_limits<std::size_t>::max()) {
            oldRecordId = std::make_optional(currentNode->first.getNextRecords()[0]);
        }

        std::vector<Id> leafIds(256, Id(std::numeric_limits<std::size_t>::max()));
        leafIds[0] = recordId;

        _storage.replace(currentNode->second, TrieNode(leafIds));

        return oldRecordId;
    }

    std::optional<Id> Trie::remove(const Key &key) {
        std::optional<std::pair<TrieNode, Id>> currentNode = traverse(key, false);

        std::optional<Id> oldRecordId = std::nullopt;

        if (currentNode->first.getNextRecords()[0].getId() != std::numeric_limits<std::size_t>::max()) {
            oldRecordId = std::make_optional(currentNode->first.getNextRecords()[0]);
        }

        std::vector<Id> leafIds;
        leafIds.reserve(256);
        leafIds.emplace_back(std::numeric_limits<std::size_t>::max());
        for (int j = 1; j < 256; ++j) {
            leafIds.emplace_back(0);
        }

        _storage.replace(currentNode->second, TrieNode(leafIds));

        return oldRecordId;
    }

    std::optional<Id> Trie::get(const Key &key) {
        std::optional<std::pair<TrieNode, Id>> currentNodeOptional = traverse(key, false);

        if (!currentNodeOptional.has_value()) {
            return std::nullopt;
        }

        Id id = currentNodeOptional->first.getNextRecords()[0];
        if (id.getId() == std::numeric_limits<std::size_t>::max()) {
            return std::nullopt;
        }

        return std::make_optional(id);
    }

    void Trie::addRoot() {
        std::vector<Id> rootIds(256, Id(std::numeric_limits<std::size_t>::max()));

        _storage.add(TrieNode(rootIds));
    }

    std::optional<std::pair<TrieNode, Id>> Trie::traverse(Key key, bool shouldCreateNode) {
        const char *keyBytes = key.getKey();
        Id currentNodeId = Id(0);
        TrieNode currentNode = _storage.get(currentNodeId).value();

        for (std::size_t i = 0; i < key.getSize(); ++i) {
            char byte = *keyBytes;

            Id nextNodeId = currentNode.getNextRecords()[byte]; // TODO extra copy

            if (nextNodeId.getId() != std::numeric_limits<std::size_t>::max()) {
                currentNode = _storage.get(nextNodeId).value();
                currentNodeId = nextNodeId;
            } else {
                if (!shouldCreateNode) {
                    return std::nullopt;
                }

                std::vector<Id> nextNodeIds(256, Id(std::numeric_limits<std::size_t>::max()));

                TrieNode createdTrieNode = TrieNode(nextNodeIds);
                Id createdTrieNodeId = _storage.add(createdTrieNode);

                std::vector<Id> ids = currentNode.getNextRecords();
                ids[byte] = createdTrieNodeId; // TODO extra copy
                currentNode = TrieNode(ids);

                _storage.replace(currentNodeId, currentNode);

                currentNode = createdTrieNode;
                currentNodeId = createdTrieNodeId;
            }

            keyBytes += 1;

        }

        return std::make_optional(std::make_pair(currentNode, currentNodeId));
    }

    void Trie::clear() {
        _storage.clear();
    }

    void Trie::merge(std::shared_ptr<InMemoryTrieNode> smallTrieRoot) {
        merge(Id(), smallTrieRoot);
    }

    Id Trie::merge(const Id &trieNodeId, std::shared_ptr<InMemoryTrieNode> smallTrieNode) {
        if (trieNodeId.getId() == std::numeric_limits<std::size_t>::max() && !smallTrieNode) {
            return trieNodeId;
        }
        if (!smallTrieNode) {
            return trieNodeId;
        }
        if (trieNodeId.getId() == std::numeric_limits<std::size_t>::max()) {
            return _storage.addTrieNodeSubtree(smallTrieNode);
        }
        TrieNode trieNode = _storage.get(trieNodeId).value();
        std::vector<Id> ids(256);
        for (std::size_t i = 0; i < 256; ++i) {
            ids[i] = merge(trieNode.getNextRecords()[i], smallTrieNode->get(i));
        }

        _storage.replace(trieNodeId, TrieNode(ids));

        return trieNodeId;
    }
}