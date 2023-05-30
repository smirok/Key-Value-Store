#ifndef KEYVALUESTORAGE_RECORD_HPP
#define KEYVALUESTORAGE_RECORD_HPP

#include "model/Key.hpp"
#include "model/Value.hpp"
#include <vector>

namespace kvs {

    /**
     * \brief Class representing record @p KeyValue in the file : key, boolean flag, and value.
     */
    class Record {
    public:
        /**
         * @param key Stored key.
         * @param isOutdated Whether this record is outdated.
         * @param value Stored value.
         */
        Record(Key key, bool isOutdated, Value value);

        [[nodiscard]] const Key &getKey() const;

        [[nodiscard]] bool getIsOutdated() const;

        [[nodiscard]] const Value &getValue() const;

    private:
        Key _key;
        bool _isOutdated;
        Value _value;
    };

} // kvs

#endif //KEYVALUESTORAGE_RECORD_HPP
