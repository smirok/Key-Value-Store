#ifndef KEYVALUESTORAGE_RECORDSERIALIZER_HPP
#define KEYVALUESTORAGE_RECORDSERIALIZER_HPP

#include <vector>
#include "TrieNode.hpp"
#include "Record.hpp"

namespace kvs {

    class RecordSerializer {
    public:
        explicit RecordSerializer(std::size_t idSize);

        char *recordToBytes(const Record &record);

        Record bytesToRecord(const char *bytes);

    private:
        std::size_t _idSize;
        static constexpr std::size_t ALPHABET_SIZE = 256;
    };

}

#endif //KEYVALUESTORAGE_RECORDSERIALIZER_HPP
