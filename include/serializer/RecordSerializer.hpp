#ifndef KEYVALUESTORAGE_RECORDSERIALIZER_HPP
#define KEYVALUESTORAGE_RECORDSERIALIZER_HPP

#include <cstddef>
#include "model/Record.hpp"

namespace kvs {
    /**
     * \brief Class responsible for @p Record serialization and deserialization.
     */
    class RecordSerializer {
    public:
        /**
         * @param key_size Key size (in bytes).
         * @param value_size Value size (in bytes).
         */
        RecordSerializer(std::size_t key_size, std::size_t value_size);

        /**
         * Transform @p Record in sequence of bytes.
         * @param record Record to transform.
         * @return Array of bytes after serialization.
         */
        [[nodiscard]] char *recordToBytes(const Record &record) const;

        /**
         * Transform byte sequence to @p Record.
         * @param bytes Array of bytes representing @p Record.
         * @return The record.
         */
        Record bytesToRecord(const char *bytes) const;

        [[nodiscard]] std::size_t getKeySize() const;

        [[nodiscard]] std::size_t getValueSize() const;

    private:
        std::size_t _key_size;
        std::size_t _value_size;
    };

} // kvs

#endif //KEYVALUESTORAGE_RECORDSERIALIZER_HPP
