#ifndef KEYVALUESTORAGE_LOGICSTORAGE_HPP
#define KEYVALUESTORAGE_LOGICSTORAGE_HPP

#include <optional>
#include "model/Key.hpp"
#include "model/Id.hpp"

namespace kvs {

    /**
     * \brief Интерфейс хранилища, отвечающего только за логику.
     */
    class LogicStorage {
    public:
        /**
         * Добавление в @p LogicStorage по ключу @p key значение @p id.
         * Если ранее в @p LogicStorage уже был какой-то идентификатор по ключу @p key, то его @p Id обновляется
         * @param key Ключ.
         * @param id Идентификатор.
         * @return @p Id обернутый в @p std::optional, если ранее в реализации @p LogicStorage уже был какой-то идентификатор по ключу @p key;
         * пустой @p std::optional иначе
         */
        virtual std::optional<Id> add(const Key &key, const Id &id) = 0;

        /**
         * Удаление пары c ключом @p key.
         * @param key Ключ.
         * @return @p Id обернутый в @p std::optional, если ранее в реализации @p LogicStorage уже был какой-то идентификатор по ключу @p key;
         * пустой @p std::optional иначе.
         */
        virtual std::optional<Id> remove(const Key &key) = 0;

        /**
         * Получить @p Id по ключу @p key.
         * @param key Ключ.
         * @return @p Id обернутый в @p std::optional, если в реализации @p LogicStorage есть какой-то идентификатор по ключу @p key;
         * пустой @p std::optional иначе.
         */
        [[nodiscard]] virtual std::optional<Id> get(const Key &key) const = 0;

        /**
         * Очищает @p LogicStorage.
         */
        virtual void clear() = 0;

        virtual ~LogicStorage() = default;
    };

}

#endif //KEYVALUESTORAGE_LOGICSTORAGE_HPP
