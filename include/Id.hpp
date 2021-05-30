#ifndef KEYVALUESTORAGE_ID_HPP
#define KEYVALUESTORAGE_ID_HPP

#include <cstddef>

namespace kvs {

    class Id {
    public:
        Id(std::size_t objectIdentifier);

        std::size_t getId() const;

    private:
        std::size_t _objectIdentifier;
    };

}

#endif //KEYVALUESTORAGE_ID_HPP
