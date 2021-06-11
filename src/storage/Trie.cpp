#include "storage/Trie.hpp"
#include <limits>
#include <utility>

namespace kvs {
    // nextRecords[i] == Id(std::numeric_limits<std::size_t>::max()) IFF edge is missing
    // nextRecords[i] == Id(std::numeric_limits<std::size_t>::max() - 1 for i >= 1 IFF currentNode is leaf
    // EMPTY LEAF = {std::numeric_limits<std::size_t>::max() , std::numeric_limits<std::size_t>::max() -1,
    // std::numeric_limits<std::size_t>::max() - 1, ... , std::numeric_limits<std::size_t>::max()  - 1}
    // NOT EMPTY LEAF = {id, std::numeric_limits<std::size_t>::max() -1,
    // std::numeric_limits<std::size_t>::max() -1, ..., std::numeric_limits<std::size_t>::max() -1}

    Trie::Trie(Storage<TrieNode> &storage) : _storage(storage) {
        addRoot();
    }

    std::optional<Id> Trie::add(const Key &key, const Id &recordId) {
        std::optional<std::pair<TrieNode, Id>> currentNode = traverse(key, true);

        std::optional<Id> oldRecordId = std::nullopt;

        if (currentNode->first.getNextRecord(0).getId() != std::numeric_limits<std::size_t>::max()) {
            oldRecordId = std::make_optional(currentNode->first.getNextRecord(0));
        }

        std::vector<Id> leafIds(UCHAR_MAX + 1, Id(std::numeric_limits<std::size_t>::max() - 1)); // LEAF
        leafIds[0] = recordId;

        _storage.replace(currentNode->second, TrieNode(leafIds));

        return oldRecordId;
    }

    std::optional<Id> Trie::remove(const Key &key) {
        std::optional<std::pair<TrieNode, Id>> currentNode = traverse(key, false);

        std::optional<Id> oldRecordId = std::nullopt;

        if (currentNode->first.getNextRecord(0).getId() != std::numeric_limits<std::size_t>::max()) {
            oldRecordId = std::make_optional(currentNode->first.getNextRecord(0));
        }

        std::vector<Id> leafIds(UCHAR_MAX + 1, Id(std::numeric_limits<std::size_t>::max() - 1));
        leafIds[0] = Id(std::numeric_limits<std::size_t>::max());

        _storage.replace(currentNode->second, TrieNode(leafIds));

        return oldRecordId;
    }

    std::optional<Id> Trie::get(const Key &key) const {
        std::optional<std::pair<TrieNode, Id>> currentNodeOptional = traverse(key, false);

        if (!currentNodeOptional.has_value()) {
            return std::nullopt;
        }

        Id id = currentNodeOptional->first.getNextRecord(0);
        if (id.getId() == std::numeric_limits<std::size_t>::max()) {
            return std::nullopt;
        }

        return std::make_optional(id);
    }

    void Trie::addRoot() {
        std::vector<Id> rootIds(UCHAR_MAX + 1, Id(std::numeric_limits<std::size_t>::max()));

        _storage.add(TrieNode(rootIds));
    }

    std::optional<std::pair<TrieNode, Id>> Trie::traverse(const Key &key, bool shouldCreateNode) const {
        const char *keyBytes = key.getKey();
        Id currentNodeId = Id(0);
        TrieNode currentNode = _storage.get(currentNodeId).value();

        for (std::size_t i = 0; i < key.getSize(); ++i) {
            char byte = *keyBytes;

            Id nextNodeId = currentNode.getNextRecord(byte);

            if (nextNodeId.getId() != std::numeric_limits<std::size_t>::max()) {
                currentNode = _storage.get(nextNodeId).value();
                currentNodeId = nextNodeId;
            } else {
                if (!shouldCreateNode) {
                    return std::nullopt;
                }

                std::vector<Id> nextNodeIds(UCHAR_MAX + 1, Id(std::numeric_limits<std::size_t>::max()));

                TrieNode createdTrieNode = TrieNode(nextNodeIds);
                Id createdTrieNodeId = _storage.add(createdTrieNode);

                std::vector<Id> ids = currentNode.getNextRecords();
                ids[byte] = createdTrieNodeId;
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
        addRoot();
    }

    const std::vector<Id> &Trie::merge(const std::shared_ptr<InMemoryTrieNode> &smallTrieRoot) {
        _recordsToDelete.clear();
        merge(Id(0), smallTrieRoot);
        return _recordsToDelete;
    }

    Id Trie::merge(const Id &trieNodeId, const std::shared_ptr<InMemoryTrieNode> &smallTrieNode) {
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

        if (trieNode.getNextRecord(1).getId() == std::numeric_limits<std::size_t>::max() - 1) { // trieNode == leaf
            if (trieNode.getNextRecord(0).getId() != std::numeric_limits<std::size_t>::max()) {
                _recordsToDelete.push_back(trieNode.getNextRecord(0));

                std::vector<Id> ids(UCHAR_MAX + 1, Id(std::numeric_limits<std::size_t>::max() - 1));
                ids[0] = smallTrieNode->getId();

                _storage.replace(trieNodeId, TrieNode(ids));
                return trieNodeId;
            }
        }

        std::vector<Id> ids(UCHAR_MAX + 1);
        for (std::size_t i = 0; i <= UCHAR_MAX; ++i) {
            ids[i] = merge(trieNode.getNextRecord(i), smallTrieNode->get(i));
        }

        _storage.replace(trieNodeId, TrieNode(ids));

        return trieNodeId;
    }
}