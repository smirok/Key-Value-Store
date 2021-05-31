#ifndef KEYVALUESTORAGE_LOGICSTORAGE_HPP
#define KEYVALUESTORAGE_LOGICSTORAGE_HPP

#include <optional>
#include "Key.hpp"
#include "Id.hpp"

namespace kvs {

    class LogicStorage {
    public:
        virtual void add(Key key, Id id) = 0;
        virtual void remove(Key key) = 0;
        virtual std::optional<Id> get(Key key) = 0;
        virtual void clear() = 0;

        virtual ~LogicStorage() {};
    };

}

#endif //KEYVALUESTORAGE_LOGICSTORAGE_HPP