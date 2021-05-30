#ifndef KEYVALUESTORAGE_BLOOMFILTER_HPP
#define KEYVALUESTORAGE_BLOOMFILTER_HPP

#include <vector>
#include <bitset>
#include <deque>
#include "Key.hpp"

namespace kvs {

    class BloomFilter {
    public:
        BloomFilter(std::size_t size);

        void add(Key key);

        bool mightContains(Key key) const;

    private:
        std::vector<bool> _store;
        std::size_t _functionsCount;
    };

}

#endif //KEYVALUESTORAGE_BLOOMFILTER_HPP
