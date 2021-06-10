#ifndef KEYVALUESTORAGE_FILEOFFSET_HPP
#define KEYVALUESTORAGE_FILEOFFSET_HPP

#include <cstddef>
#include <cstdlib>

namespace kvs {

    class FileOffset {
    public:
        explicit FileOffset(size_t offset);

        [[nodiscard]] off64_t getOffset() const;

    private:
        off64_t _offset;
    };

}

#endif //KEYVALUESTORAGE_FILEOFFSET_HPP
