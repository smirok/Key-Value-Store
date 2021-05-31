#include "Record.hpp"

#include <utility>

namespace kvs {

    Record::Record(std::vector<Id> nextRecords) : _nextRecords(std::move(nextRecords)) {
    }

    std::vector<Id> Record::getNextRecords() const {
        return _nextRecords;
    }

}
