#ifndef KEYVALUESTORAGE_BLOOMFILTER_HPP
#define KEYVALUESTORAGE_BLOOMFILTER_HPP

#include <vector>
#include <bitset>
#include <deque>
#include "model/Key.hpp"

namespace kvs {

    /**
     * \brief Class implementing Bloom Filter.
     */
    class BloomFilter {
    public:
        /**
         * @param size Bloom Filter's size.
         */
        explicit BloomFilter(std::size_t size);

        /**
         * @param key Key added to filter.
         */
        void add(const Key &key);

        /**
         * @param key The key to check existence.
         * @return @p true, if the key might be in the Bloom Filter; @p false, if the key is definitely not in the filter.
         */
        [[nodiscard]] bool mightContains(const Key &key) const;

    private:
        std::vector<bool> _store;
        std::size_t _functionsCount;
    };

} // kvs

#endif //KEYVALUESTORAGE_BLOOMFILTER_HPP
