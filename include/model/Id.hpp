#ifndef KEYVALUESTORAGE_ID_HPP
#define KEYVALUESTORAGE_ID_HPP

#include <cstddef>
#include <limits>

namespace kvs {

    class Id {
    public:
        Id() = default;

        explicit Id(std::size_t objectIdentifier);

        [[nodiscard]] std::size_t getId() const;

        static std::size_t getIdSize();

        bool operator==(const Id &id) const;

    private:
        std::size_t _objectIdentifier = std::numeric_limits<std::size_t>::max();
    };

}

#endif //KEYVALUESTORAGE_ID_HPP
