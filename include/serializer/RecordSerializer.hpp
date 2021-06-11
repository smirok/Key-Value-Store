#ifndef KEYVALUESTORAGE_RECORDSERIALIZER_HPP
#define KEYVALUESTORAGE_RECORDSERIALIZER_HPP

#include <cstddef>
#include "model/Record.hpp"

namespace kvs {
    /**
     * \brief Класс, отвечающий за сериализацию @p Record.
     */
    class RecordSerializer {
    public:
        /**
         *
         * @param key_size Размер ключа в байтах.
         * @param value_size Размер значения в байтах.
         */
        RecordSerializer(std::size_t key_size, std::size_t value_size);

        /**
         * Преобразовывает @p Record в байты.
         * @param record Преобразуемая запись.
         * @return Массив байт после преобразования.
         */
        [[nodiscard]] char *recordToBytes(const Record &record) const;

        /**
         * Преобразовывает байты в @p Record.
         * @param bytes Массив байт, представляющий @p Record.
         * @return Сама запись.
         */
        Record bytesToRecord(const char *bytes) const;

        [[nodiscard]] std::size_t getKeySize() const;

        [[nodiscard]] std::size_t getValueSize() const;

    private:
        std::size_t _key_size;
        std::size_t _value_size;
    };

}

#endif //KEYVALUESTORAGE_RECORDSERIALIZER_HPP
