#ifndef KEYVALUESTORAGE_STORAGE_HPP
#define KEYVALUESTORAGE_STORAGE_HPP

#include "Id.hpp"
#include "KeyValue.hpp"
#include "Record.hpp"
#include <optional>

namespace kvs {

    template<typename T>
    class Storage {
    public:
        virtual Id add(T) = 0;

        virtual std::optional<T> get(Id) = 0;

        virtual void remove(Id) = 0;

        virtual void clear() = 0;

        virtual ~Storage() {};

    private:
    };

    template<>
    class Storage<Record> {
    public:
        Id add(Record record);

        std::optional<Record> get(Id id);

        void remove(Id id);

        void clear();

    private:
    };

}

#endif //KEYVALUESTORAGE_STORAGE_HPP
