#ifndef KEYVALUESTORAGE_RECORD_HPP
#define KEYVALUESTORAGE_RECORD_HPP

#include "model/Key.hpp"
#include "model/Value.hpp"
#include <vector>

namespace kvs {

    /**
     * \brief Класс, представляющий запись @p KeyValue в файле : ключ, булев флаг, значение
     */
    class Record {
    public:
        /**
         *
         * @param key Ключ
         * @param isOutdated Является ли данная запись устаревшей
         * @param value Значение
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

}

#endif //KEYVALUESTORAGE_RECORD_HPP
