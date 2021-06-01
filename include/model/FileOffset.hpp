#ifndef KEYVALUESTORAGE_FILEOFFSET_HPP
#define KEYVALUESTORAGE_FILEOFFSET_HPP

#include <cstddef>

namespace kvs {

    class FileOffset {
    public:
        explicit FileOffset(std::size_t offset);

        std::size_t getOffset() const;

    private:
        std::size_t _offset;
    };

}

#endif //KEYVALUESTORAGE_FILEOFFSET_HPP
