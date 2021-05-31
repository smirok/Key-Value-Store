#ifndef KEYVALUESTORAGE_TRIENODESERIALIZER_HPP
#define KEYVALUESTORAGE_TRIENODESERIALIZER_HPP

#include <cstddef>
#include "TrieNode.hpp"

namespace kvs {

    class TrieNodeSerializer {
    public:
        TrieNodeSerializer(std::size_t key_size, std::size_t value_size);

        char *trieNodeToBytes(const TrieNode &trieNode);

        TrieNode bytesToTrieNode(const char *bytes);

    private:
        std::size_t _key_size;
        std::size_t _value_size;
    };

}

#endif //KEYVALUESTORAGE_TRIENODESERIALIZER_HPP
