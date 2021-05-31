#ifndef KEYVALUESTORAGE_RECORD_HPP
#define KEYVALUESTORAGE_RECORD_HPP

#include <vector>
#include "Id.hpp"

namespace kvs {

    class Record {
    public:
        Record(std::vector<Id> nextRecords);

        std::vector<Id> getNextRecords() const;

    private:
        std::vector<Id> _nextRecords;
    };

}

#endif //KEYVALUESTORAGE_RECORD_HPP
