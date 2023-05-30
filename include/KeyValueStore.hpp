#ifndef KEYVALUESTORAGE_KEYVALUESTORE_HPP
#define KEYVALUESTORAGE_KEYVALUESTORE_HPP

#include <optional>
#include <storage/Trie.hpp>
#include "model/KeyValue.hpp"
#include "storage/BloomFilter.hpp"
#include "storage/Log.hpp"
#include "storage/Storage.hpp"

namespace kvs {
    /**
     * \brief Main class stored pairs < @p Key, @p Value >
     */
    class KeyValueStore {
    public:
        /**
         * @param bloomFilter Bloom Filter.
         * @param log Log as a RAM storage.
         * @param trie Trie as a disk storage.
         * @param _recordStorage Storage of @p Record on the disk.
         */
        KeyValueStore(BloomFilter &bloomFilter,
                      Log &log,
                      Trie &trie,
                      Storage<Record> &_recordStorage);

        /**
         * Add @p keyValue in storage.
         * @param keyValue Pair of @p Key, @p Value.
         */
        void add(const KeyValue &keyValue);

        /**
         * Get a @p KeyValue pair by @p key.
         * @param key Key.
         * @return @p KeyValue wrapped into @p std::optional, if there is @p Value in @p KeyValueStore by @p key;
         * empty @p std::optional otherwise.
         */
        [[nodiscard]] std::optional<KeyValue> get(const Key &key) const;

        /**
         * Remove pair @p Key, @p Value by @p key.
         * @param key Key.
         */
        void del(const Key &key);

        /**
         * Clean up @p KeyValueStore.
         */
        void clear();

    private:

        void rebuild();

        BloomFilter &_bloomFilter;

        Log &_log;

        Trie &_trie;

        Storage<Record> &_recordStorage;
    };
} // kvs

#endif //KEYVALUESTORAGE_KEYVALUESTORE_HPP
