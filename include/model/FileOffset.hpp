#ifndef KEYVALUESTORAGE_FILEOFFSET_HPP
#define KEYVALUESTORAGE_FILEOFFSET_HPP

#include <cstddef>
#include <cstdlib>

namespace kvs {

    /**
     * \brief A class representing a position in a file.
     */
    class FileOffset {
    public:
        /**
         * @param offset Stored position.
         */
        explicit FileOffset(size_t offset);

        /**
         * @return Stored position.
         */
        [[nodiscard]] off64_t getOffset() const;

    private:
        off64_t _offset;
    };

} // kvs

#endif //KEYVALUESTORAGE_FILEOFFSET_HPP
