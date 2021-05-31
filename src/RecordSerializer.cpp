#include "RecordSerializer.hpp"

namespace kvs {

    RecordSerializer::RecordSerializer(std::size_t idSize) : _idSize(idSize) {
    }

    char *RecordSerializer::recordToBytes(const Record &record) {
        char *result = new char[_idSize * ALPHABET_SIZE];

        std::vector<Id> nextRecords = record.getNextRecords();
        for (auto &nextRecord : nextRecords) {
            *(reinterpret_cast<std::size_t *>(result)) = nextRecord.getId();
            result += Id::getIdSize();
        }

        return result - (_idSize * ALPHABET_SIZE);
    }

    Record RecordSerializer::bytesToRecord(const char *bytes) {
        std::vector<Id> ids;
        ids.reserve(ALPHABET_SIZE);
        for (std::size_t i = 0; i < ALPHABET_SIZE; ++i) {
            ids.emplace_back(*(reinterpret_cast<std::size_t *>(const_cast<char *>(bytes))));
            bytes += Id::getIdSize();
        }

        return Record(ids);
    }
}