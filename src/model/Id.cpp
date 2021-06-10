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

    bool Id::operator==(const Id &id) const {
        return _objectIdentifier == id._objectIdentifier;
    }
}
