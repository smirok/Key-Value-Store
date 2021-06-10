#include "model/FileOffset.hpp"

namespace kvs {

    FileOffset::FileOffset(size_t offset) : _offset(static_cast<off64_t >(offset)) {
    }

    off64_t FileOffset::getOffset() const {
        return _offset;
    }
}
