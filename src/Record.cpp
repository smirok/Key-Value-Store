#include "Record.hpp"

namespace kvs {

    Record::Record(const Key &key, bool isOutdated, const Value &value) : _key(key),
                                                                              _isOutdated(isOutdated),
                                                                              _value(value) {
    }

    Key Record::getKey() const {
        return _key;
    }

    bool Record::getIsOutdated() const {
        return _isOutdated;
    }

    Value Record::getValue() const {
        return _value;
    }
}