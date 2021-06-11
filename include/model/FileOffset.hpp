#ifndef KEYVALUESTORAGE_FILEOFFSET_HPP
#define KEYVALUESTORAGE_FILEOFFSET_HPP

#include <cstddef>
#include <cstdlib>

namespace kvs {

    /**
     * \brief Класс, представляющий позицию в файле.
     */
    class FileOffset {
    public:
        /**
         *
         * @param offset Хранимая позиция.
         */
        explicit FileOffset(size_t offset);

        /**
         *
         * @return Хранимая позиция.
         */
        [[nodiscard]] off64_t getOffset() const;

    private:
        off64_t _offset;
    };

}

#endif //KEYVALUESTORAGE_FILEOFFSET_HPP
