#ifndef KEYVALUESTORAGE_ID_HPP
#define KEYVALUESTORAGE_ID_HPP

#include <cstddef>
#include <limits>

namespace kvs {

    /**
     * \brief A class representing the identifier of some object.
     */
    class Id {
    public:
        Id() = default;

        /**
         * @param objectIdentifier Stored identifier.
         */
        explicit Id(std::size_t objectIdentifier);

        /**
         * @return Stored identifier.
         */
        [[nodiscard]] std::size_t getId() const;


        /**
         * @return Size of @p _objectIdentifier in bytes, sizeof(@p size_t).
         */
        static std::size_t getIdSize();

        bool operator==(const Id &id) const;

    private:
        std::size_t _objectIdentifier = std::numeric_limits<std::size_t>::max();
    };

} // kvs

#endif //KEYVALUESTORAGE_ID_HPP
