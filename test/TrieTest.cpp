#include <storage/Trie.hpp>
#include "testing.h"

namespace kvs {

    TEST(Trie, creation) {
        File file = File("data.bin");
        TrieNodeSerializer trieNodeSerializer = TrieNodeSerializer(Id::getIdSize());
        Storage<TrieNode> storage(file, trieNodeSerializer);

        Trie trie(storage);
    }

    TEST(Trie, addAndGet) {
        File file = File("data.bin");
        TrieNodeSerializer trieNodeSerializer = TrieNodeSerializer(Id::getIdSize());
        Storage<TrieNode> storage(file, trieNodeSerializer);

        Trie trie(storage);
        Key key("aaaa", 4);
        std::optional<Id> emptyOldRecord = trie.add(key, 0);

        EXPECT_FALSE(emptyOldRecord.has_value());

        Key key2("bbbb", 4);
        std::optional<Id> oldRecordId = trie.add(key2, 1);
        EXPECT_FALSE(oldRecordId.has_value());

        EXPECT_EQ(trie.get(key2)->getId(), 1);
        remove("data.bin");
    }

    TEST(Trie, addTwoValuesToOneKey) {
        File file = File("data.bin");
        TrieNodeSerializer trieNodeSerializer = TrieNodeSerializer(Id::getIdSize());
        Storage<TrieNode> storage(file, trieNodeSerializer);

        Trie trie(storage);

        Key key("aaaa", 4);
        std::optional<Id> emptyOldRecord = trie.add(key, 322);

        EXPECT_FALSE(emptyOldRecord.has_value());

        std::optional<Id> oldRecordId = trie.add(key, 323);
        EXPECT_EQ(oldRecordId->getId(), 322);

        remove("data.bin");
    }

    TEST(Trie, getIfNotExist) {
        File file = File("data.bin");
        TrieNodeSerializer trieNodeSerializer = TrieNodeSerializer(Id::getIdSize());
        Storage<TrieNode> storage(file, trieNodeSerializer);

        Trie trie(storage);

        Key key("aaaa", 4);

        std::optional<Id> optionalId = trie.get(key);
        EXPECT_FALSE(optionalId.has_value());

        remove("data.bin");
    }

    TEST(Trie, getIfWasRemoved) {
        File file = File("data.bin");
        TrieNodeSerializer trieNodeSerializer = TrieNodeSerializer(Id::getIdSize());
        Storage<TrieNode> storage(file, trieNodeSerializer);

        Trie trie(storage);

        Key key("aaaa", 4);

        std::optional<Id> emptyRecordId = trie.add(key, Id(2));
        EXPECT_FALSE(emptyRecordId.has_value());
        std::optional<Id> presentRecordId = trie.remove(key);
        EXPECT_EQ(presentRecordId->getId(), 2);

        EXPECT_FALSE(trie.get(key).has_value());

        remove("data.bin");
    }

    TEST(Trie, clear) {
        File file = File("data.bin");
        TrieNodeSerializer trieNodeSerializer = TrieNodeSerializer(Id::getIdSize());
        Storage<TrieNode> storage(file, trieNodeSerializer);

        Trie trie(storage);

        Key key("aaaa", 4);

        trie.add(key, Id(2));

        EXPECT_EQ(trie.get(key)->getId(), 2);

        trie.clear();

        EXPECT_FALSE(trie.get(key).has_value());
    }
}