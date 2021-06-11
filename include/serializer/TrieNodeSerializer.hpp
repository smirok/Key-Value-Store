#ifndef KEYVALUESTORAGE_TRIENODESERIALIZER_HPP
#define KEYVALUESTORAGE_TRIENODESERIALIZER_HPP

#include <vector>
#include <climits>
#include "model/Record.hpp"
#include "model/TrieNode.hpp"

namespace kvs {

    /**
     * \brief Класс, отвечающий за сериализацию @p TrieNode.
     */
    class TrieNodeSerializer {
    public:
        /**
         *
         * @param idSize Размер @p Id в байтах.
         */
        explicit TrieNodeSerializer(std::size_t idSize);

        /**
         * Преобразовывает @p TrieNode в байты.
         * @param trieNode Преобразуемая нода.
         * @return Массив байт после преобразования.
         */
        [[nodiscard]] char *trieNodeToBytes(const TrieNode &trieNode) const;

        /**
         * Преобразовывает байты в @p TrieNode.
         * @param bytes Массив байт, представляющий @p TrieNode.
         * @return Сама запись.
         */
        TrieNode bytesToTrieNode(const char *bytes) const;

    private:
        std::size_t _idSize;
        static constexpr int ALPHABET_SIZE = UCHAR_MAX + 1;
    };

}

#endif //KEYVALUESTORAGE_TRIENODESERIALIZER_HPP
