#ifndef KEYVALUESTORAGE_KEYVALUESTORE_HPP
#define KEYVALUESTORAGE_KEYVALUESTORE_HPP

#include <optional>
#include "KeyValue.hpp"

namespace kvs {
    class KeyValueStore {
        void add(const KeyValue &);
        std::optional<KeyValue> get(const Key &) const;
        void del(const Key&);
    };
}

#endif //KEYVALUESTORAGE_KEYVALUESTORE_HPP
