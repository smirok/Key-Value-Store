#ifndef KEYVALUESTORAGE_KEYVALUESTORE_HPP
#define KEYVALUESTORAGE_KEYVALUESTORE_HPP

#include <optional>
#include "KeyValue.hpp"
#include "BloomFilter.hpp"

namespace kvs {
    class KeyValueStore {
    public:
        KeyValueStore(const BloomFilter& bloomFilter);

        void add(const KeyValue &);

        std::optional<KeyValue> get(const Key &) const;

        void del(const Key &);

        void clear();

    private:
        bool shouldRemoveOutdatedParts() const;

        void removeOutdatedParts();

        BloomFilter _bloomFilter;
    };
}

#endif //KEYVALUESTORAGE_KEYVALUESTORE_HPP
