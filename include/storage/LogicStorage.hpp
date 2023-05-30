#ifndef KEYVALUESTORAGE_LOGICSTORAGE_HPP
#define KEYVALUESTORAGE_LOGICSTORAGE_HPP

#include <optional>
#include "model/Key.hpp"
#include "model/Id.hpp"

namespace kvs {

    /**
     * \brief Storage interface responsible for the storage logic.
     */
    class LogicStorage {
    public:
        /**
         * Insert value @p id in @p LogicStorage by @p key.
         * If there was some identifier in @p LogicStorage by @p key, then @p Id will be updated.
         * @param key Key.
         * @param id Identifier.
         * @return @p Id wrapped into @p std::optional, if there was some identifier in @p LogicStorage implementation by @p key;
         * empty @p std::optional otherwise.
         */
        virtual std::optional<Id> add(const Key &key, const Id &id) = 0;

        /**
         * Remove key-value pair by @p key.
         * @param key Key.
         * @return @p Id wrapped into @p std::optional, if there was some identifier in @p LogicStorage implementation by @p key;
         * empty @p std::optional otherwise.
         */
        virtual std::optional<Id> remove(const Key &key) = 0;

        /**
         * Get @p Id by @p key.
         * @param key Ключ.
         * @return @p Id wrapped into @p std::optional, if there was some identifier in @p LogicStorage implementation by @p key;
         * empty @p std::optional otherwise.
         */
        [[nodiscard]] virtual std::optional<Id> get(const Key &key) const = 0;

        /**
         * Clean up @p LogicStorage.
         */
        virtual void clear() = 0;

        virtual ~LogicStorage() = default;
    };

} // kvs

#endif //KEYVALUESTORAGE_LOGICSTORAGE_HPP
