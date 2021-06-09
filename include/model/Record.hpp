#ifndef KEYVALUESTORAGE_RECORD_HPP
#define KEYVALUESTORAGE_RECORD_HPP

#include "model/Key.hpp"
#include "model/Value.hpp"
#include <vector>

namespace kvs {

    class Record {
    public:
        Record(Key key, bool isOutdated, Value value);

        const Key &getKey() const;

        bool getIsOutdated() const;

        const Value &getValue() const;

    private:
        Key _key;
        bool _isOutdated;
        Value _value;
    };

}

#endif //KEYVALUESTORAGE_RECORD_HPP
