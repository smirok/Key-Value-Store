#include "model/FileOffset.hpp"

namespace kvs {

    FileOffset::FileOffset(std::size_t offset) : _offset(offset) {
    }

    std::size_t FileOffset::getOffset() const {
        return _offset;
    }
}
