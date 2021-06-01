#include <storage/File.hpp>
#include <serializer/TrieNodeSerializer.hpp>
#include <storage/Storage.hpp>
#include "testing.h"

namespace kvs {

    TEST(TrieNodeStorage, creation) {
        File file = File("data.bin");
        TrieNodeSerializer trieNodeSerializer = TrieNodeSerializer(sizeof(std::size_t));
        Storage<TrieNode>(file, trieNodeSerializer);
        remove("data.bin");
    }

    TEST(TrieNodeStorage, addAndGet) {
        File file = File("data.bin");
        TrieNodeSerializer trieNodeSerializer = TrieNodeSerializer(sizeof(std::size_t));
        Storage<TrieNode> storage(file, trieNodeSerializer);

        std::vector<Id> ids;
        ids.reserve(256);
        for (int i = 0; i < 256; ++i) {
            ids.emplace_back(i);
        }

        TrieNode trieNode(ids);
        Id id = storage.add(trieNode);
        EXPECT_EQ(id.getId(), 0);

        std::optional<TrieNode> takenTrieNodeOptional = storage.get(id);
        std::vector<Id> newIds = takenTrieNodeOptional->getNextRecords();
        for (int i = 0; i < 256; ++i) {
            EXPECT_EQ(ids[i].getId(), newIds[i].getId());
        }
        remove("data.bin");
    }

    TEST(TrieNodeStorage, autoIncrementAdd) {
        File file = File("data.bin");
        TrieNodeSerializer trieNodeSerializer = TrieNodeSerializer(Id::getIdSize());
        Storage<TrieNode> storage(file, trieNodeSerializer);

        std::vector<Id> ids;
        ids.reserve(256);
        for (int i = 0; i < 256; ++i) {
            ids.emplace_back(i);
        }

        TrieNode trieNode(ids);

        for (int i = 0; i < 100; ++i) {
            Id id = storage.add(trieNode);
            EXPECT_EQ(id.getId(), i);
        }

        remove("data.bin");
    }

    TEST(TrieNodeStorage, clear) {
        File file = File("data.bin");
        TrieNodeSerializer trieNodeSerializer = TrieNodeSerializer(Id::getIdSize());
        Storage<TrieNode> storage(file, trieNodeSerializer);

        std::vector<Id> ids;
        ids.reserve(256);
        for (int i = 0; i < 256; ++i) {
            ids.emplace_back(i);
        }

        TrieNode trieNode(ids);

        Id id = storage.add(trieNode);
        EXPECT_EQ(id.getId(), 0);
        storage.clear();

        id = storage.add(trieNode);
        EXPECT_EQ(id.getId(), 0);

        remove("data.bin");
    }
}
