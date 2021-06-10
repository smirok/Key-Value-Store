#ifndef KEYVALUESTORAGE_BLOOMFILTER_HPP
#define KEYVALUESTORAGE_BLOOMFILTER_HPP

#include <vector>
#include <bitset>
#include <deque>
#include "model/Key.hpp"

namespace kvs {

    class BloomFilter {
    public:
        explicit BloomFilter(std::size_t size);

        void add(const Key &key);

        [[nodiscard]] bool mightContains(const Key &key) const;

    private:
        std::vector<bool> _store;
        std::size_t _functionsCount;
    };

}

#endif //KEYVALUESTORAGE_BLOOMFILTER_HPP
