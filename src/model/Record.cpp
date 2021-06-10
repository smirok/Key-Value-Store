#include <utility>
#include "model/Record.hpp"

namespace kvs {

    Record::Record(Key key, bool isOutdated, Value value) : _key(std::move(key)),
                                                            _isOutdated(isOutdated),
                                                            _value(std::move(value)) {
    }

    const Key &Record::getKey() const {
        return _key;
    }

    bool Record::getIsOutdated() const {
        return _isOutdated;
    }

    const Value &Record::getValue() const {
        return _value;
    }
}