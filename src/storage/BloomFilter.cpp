#include "storage/BloomFilter.hpp"
#include "../../extern/xxHash/xxhash.h"
#include <cmath>

namespace kvs {
    BloomFilter::BloomFilter(std::size_t size) {
        _store.assign(size, false);
        _functionsCount = static_cast<std::size_t>(ceil(log2(static_cast<double>(size))));
    }

    void BloomFilter::add(const Key &key) {
        for (std::size_t i = 1; i <= _functionsCount; ++i) {
            std::size_t hash = XXH64(key.getKey(), key.getSize(), i);

            _store[hash % _store.size()] = true;
        }
    }

    bool BloomFilter::mightContains(const Key &key) const {
        for (std::size_t i = 1; i <= _functionsCount; ++i) {
            std::size_t hash = XXH64(key.getKey(), key.getSize(), i);

            if (!_store[hash % _store.size()]) {
                return false;
            }
        }

        return true;
    }
}