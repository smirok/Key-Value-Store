#include "testing.h"

#include "model/KeyValue.hpp"

namespace kvs {

    TEST(KeyValue, Creation) {
        std::string rawKey = "aaaa";
        std::string rawValue = "bbbbbbbb";

        Key key(rawKey.data(), rawKey.size());
        Value value(rawValue.data(), rawValue.size());

        KeyValue firstKeyValue(key, value);
        KeyValue secondKeyValue(rawKey.data(), rawKey.size(), rawValue.data(), rawValue.size());
    }

}
