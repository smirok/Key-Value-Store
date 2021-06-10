#ifndef KEYVALUESTORAGE_RECORDSERIALIZER_HPP
#define KEYVALUESTORAGE_RECORDSERIALIZER_HPP

#include <cstddef>
#include "model/Record.hpp"

namespace kvs {

    class RecordSerializer {
    public:
        RecordSerializer(std::size_t key_size, std::size_t value_size);

        [[nodiscard]] char *recordToBytes(const Record &trieNode) const;

        Record bytesToRecord(const char *bytes) const;

        [[nodiscard]] std::size_t getKeySize() const;

        [[nodiscard]] std::size_t getValueSize() const;

    private:
        std::size_t _key_size;
        std::size_t _value_size;
    };

}

#endif //KEYVALUESTORAGE_RECORDSERIALIZER_HPP
