#ifndef KEYVALUESTORAGE_RECORDSERIALIZER_HPP
#define KEYVALUESTORAGE_RECORDSERIALIZER_HPP

#include <cstddef>
#include "Record.hpp"

namespace kvs {

    class RecordSerializer {
    public:
        RecordSerializer(std::size_t key_size, std::size_t value_size);

        char *trieNodeToBytes(const Record &trieNode);

        Record bytesToTrieNode(const char *bytes);

    private:
        std::size_t _key_size;
        std::size_t _value_size;
    };

}

#endif //KEYVALUESTORAGE_RECORDSERIALIZER_HPP
