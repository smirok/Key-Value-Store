#include "model/Id.hpp"
#include <limits>

namespace kvs {

    Id::Id(std::size_t objectIdentifier) : _objectIdentifier(objectIdentifier) {
    }

    std::size_t Id::getId() const {
        return _objectIdentifier;
    }

    std::size_t Id::getIdSize() {
        return sizeof(_objectIdentifier);
    }

    Id::Id() : _objectIdentifier(std::numeric_limits<std::size_t>::max()) {
    }
}
