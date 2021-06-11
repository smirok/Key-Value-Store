#ifndef KEYVALUESTORAGE_BLOOMFILTER_HPP
#define KEYVALUESTORAGE_BLOOMFILTER_HPP

#include <vector>
#include <bitset>
#include <deque>
#include "model/Key.hpp"

namespace kvs {

    /**
     * \brief Класс, реализующий фильтр Блума.
     */
    class BloomFilter {
    public:
        /**
         *
         * @param size Размер фильтр блума.
         */
        explicit BloomFilter(std::size_t size);

        /**
         *
         * @param key Ключ, добавляемый в фильтр.
         */
        void add(const Key &key);

        /**
         *
         * @param key Ключ, наличие которого в фильтр проверяем.
         * @return @p true, если ключ может содержаться в фильтре; @p false, если ключа точно нет в фильтре.
         */
        [[nodiscard]] bool mightContains(const Key &key) const;

    private:
        std::vector<bool> _store;
        std::size_t _functionsCount;
    };

}

#endif //KEYVALUESTORAGE_BLOOMFILTER_HPP
