#ifndef KEYVALUESTORAGE_TRIENODESERIALIZER_HPP
#define KEYVALUESTORAGE_TRIENODESERIALIZER_HPP

#include <vector>
#include <climits>
#include "model/Record.hpp"
#include "model/TrieNode.hpp"

namespace kvs {

    class TrieNodeSerializer {
    public:
        explicit TrieNodeSerializer(std::size_t idSize);

        [[nodiscard]] char *trieNodeToBytes(const TrieNode &record) const;

        static TrieNode bytesToTrieNode(const char *bytes);

    private:
        std::size_t _idSize;
        static constexpr int ALPHABET_SIZE = UCHAR_MAX + 1;
    };

}

#endif //KEYVALUESTORAGE_TRIENODESERIALIZER_HPP
