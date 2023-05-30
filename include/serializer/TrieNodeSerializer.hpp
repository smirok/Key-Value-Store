#ifndef KEYVALUESTORAGE_TRIENODESERIALIZER_HPP
#define KEYVALUESTORAGE_TRIENODESERIALIZER_HPP

#include <vector>
#include <climits>
#include "model/Record.hpp"
#include "model/TrieNode.hpp"

namespace kvs {

    /**
     * \brief Class responsible for @p TrieNode serialization and deserialization.
     */
    class TrieNodeSerializer {
    public:
        /**
         * @param idSize Size of @p Id (in bytes).
         */
        explicit TrieNodeSerializer(std::size_t idSize);

        /**
         * Transform @p TrieNode in sequence of bytes.
         * @param trieNode Transformable node.
         * @return Array of bytes after transforming.
         */
        [[nodiscard]] char *trieNodeToBytes(const TrieNode &trieNode) const;

        /**
         * Transform byte sequence to @p TrieNode.
         * @param bytes Array of bytes representing @p TrieNode.
         * @return The record.
         */
        TrieNode bytesToTrieNode(const char *bytes) const;

    private:
        std::size_t _idSize;
        static constexpr int ALPHABET_SIZE = UCHAR_MAX + 1;
    };

} // kvs

#endif //KEYVALUESTORAGE_TRIENODESERIALIZER_HPP
