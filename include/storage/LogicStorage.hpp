#ifndef KEYVALUESTORAGE_LOGICSTORAGE_HPP
#define KEYVALUESTORAGE_LOGICSTORAGE_HPP

#include <optional>
#include "model/Key.hpp"
#include "model/Id.hpp"

namespace kvs {

    class LogicStorage {
    public:
        virtual std::optional<Id> add(const Key &key, const Id &id) = 0;

        virtual std::optional<Id> remove(const Key &key) = 0;

        [[nodiscard]] virtual std::optional<Id> get(const Key &key) const = 0;

        virtual void clear() = 0;

        virtual ~LogicStorage() = default;
    };

}

#endif //KEYVALUESTORAGE_LOGICSTORAGE_HPP
