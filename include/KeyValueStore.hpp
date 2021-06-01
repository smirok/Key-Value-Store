#ifndef KEYVALUESTORAGE_KEYVALUESTORE_HPP
#define KEYVALUESTORAGE_KEYVALUESTORE_HPP

#include <optional>
#include "model/KeyValue.hpp"
#include "storage/BloomFilter.hpp"
#include "storage/Log.hpp"
#include "storage/Storage.hpp"

namespace kvs {
    class KeyValueStore {
    public:
        KeyValueStore(std::size_t key_size,
                      std::size_t value_size,
                      const BloomFilter &bloomFilter,
                      const Log &log,
                      const Storage<Record> &_recordStorage);

        void add(const KeyValue &);

        std::optional<KeyValue> get(const Key &) const;

        void del(const Key &);

        void clear();

    private:
        bool shouldRemoveOutdatedParts() const;

        void removeOutdatedParts();

        BloomFilter _bloomFilter;

        Log _log;

        Storage<Record> _recordStorage;

        std::size_t _key_size;

        std::size_t _value_size;
    };
}

#endif //KEYVALUESTORAGE_KEYVALUESTORE_HPP
