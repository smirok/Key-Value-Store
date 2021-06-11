#ifndef KEYVALUESTORAGE_ID_HPP
#define KEYVALUESTORAGE_ID_HPP

#include <cstddef>
#include <limits>

namespace kvs {

    /**
     * \brief Класс, представляющий идентификатор некоторой структуры.
     */
    class Id {
    public:
        Id() = default;

        /**
         *
         * @param objectIdentifier Хранимый идентификатор.
         */
        explicit Id(std::size_t objectIdentifier);

        /**
         *
         * @return Хранимый идентификатор.
         */
        [[nodiscard]] std::size_t getId() const;


        /**
         *
         * @return размер @p _objectIdentifier в байтах, sizeof(@p size_t).
         */
        static std::size_t getIdSize();

        bool operator==(const Id &id) const;

    private:
        std::size_t _objectIdentifier = std::numeric_limits<std::size_t>::max();
    };

}

#endif //KEYVALUESTORAGE_ID_HPP
