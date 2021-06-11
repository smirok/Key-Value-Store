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
     * \brief Главный класс, хранилище пар < @p Key, @p Value >
     */
    class KeyValueStore {
    public:
        /**
         * @param bloomFilter Фильтр Блума.
         * @param log Лог в качестве хранилища в оперативной памяти.
         * @param trie Бор в качестве хранилища на диске.
         * @param _recordStorage Хранилище записей @p Record на диске.
         */
        KeyValueStore(BloomFilter &bloomFilter,
                      Log &log,
                      Trie &trie,
                      Storage<Record> &_recordStorage);

        /**
         * Добавить @p keyValue в хранилище.
         * @param keyValue пара @p Key, @p Value.
         */
        void add(const KeyValue &keyValue);

        /**
         * Получить пару @p KeyValue по ключу @p key.
         * @param key Ключ.
         * @return @p KeyValue обернутый в @p std::optional, если в @p KeyValueStore есть @p Value по ключу @p key;
         * пустой @p std::optional иначе.
         */
        [[nodiscard]] std::optional<KeyValue> get(const Key &key) const;

        /**
         * Удалить пару @p Key, @p Value по ключу @p key.
         * @param key Ключ.
         */
        void del(const Key &key);

        /**
         * Очистить @p KeyValueStore.
         */
        void clear();

    private:

        void rebuild();

        BloomFilter &_bloomFilter;

        Log &_log;

        Trie &_trie;

        Storage<Record> &_recordStorage;
    };
}

#endif //KEYVALUESTORAGE_KEYVALUESTORE_HPP
