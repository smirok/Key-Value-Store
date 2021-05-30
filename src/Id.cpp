#include "Id.hpp"

namespace kvs {

    Id::Id(std::size_t objectIdentifier) : _objectIdentifier(objectIdentifier) {
    }

    std::size_t Id::getId() const {
        return _objectIdentifier;
    }
}
