#ifndef KEYVALUESTORAGE_TRIENODESERIALIZER_HPP
#define KEYVALUESTORAGE_TRIENODESERIALIZER_HPP

#include <vector>
#include "model/Record.hpp"
#include "model/TrieNode.hpp"

namespace kvs {

    class TrieNodeSerializer {
    public:
        explicit TrieNodeSerializer(std::size_t idSize);

        char *trieNodeToBytes(const TrieNode &record);

        TrieNode bytesToTrieNode(const char *bytes);

    private:
        std::size_t _idSize;
        static constexpr std::size_t ALPHABET_SIZE = 256;
    };

}

#endif //KEYVALUESTORAGE_TRIENODESERIALIZER_HPP
