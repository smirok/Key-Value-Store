#ifndef KEYVALUESTORAGE_RECORD_HPP
#define KEYVALUESTORAGE_RECORD_HPP

#include "Key.hpp"
#include "Value.hpp"
#include <vector>

namespace kvs {

    class Record {
    public:
        Record(const Key& key, bool isOutdated, const Value& value);

        Key getKey() const;

        bool getIsOutdated() const;

        Value getValue() const;

    private:
        Key _key;
        bool _isOutdated;
        Value _value;
    };

}

#endif //KEYVALUESTORAGE_RECORD_HPP
