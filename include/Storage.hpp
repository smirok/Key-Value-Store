#ifndef KEYVALUESTORAGE_STORAGE_HPP
#define KEYVALUESTORAGE_STORAGE_HPP

#include "Id.hpp"
#include "KeyValue.hpp"
#include "File.hpp"
#include "RecordSerializer.hpp"
#include <optional>

namespace kvs {

    template<typename T>
    class Storage {
    public:
        Id add(const T &) {
            return Id(0);
        }

        std::optional<T> get(const Id &) {
            return std::nullopt;
        };

        void remove(const Id &) {
        }

        void clear() {

        }
    };

    template<>
    class Storage<Record> {
    public:
        Storage(File &dataFile, const RecordSerializer &recordSerializer) : _dataFile(dataFile),
                                                                            _recordSerializer(recordSerializer) {
        }

        Id add(const Record &record) {
            char *recordInBytes = _recordSerializer.recordToBytes(record);
            FileOffset recordOffset = _dataFile.write(recordInBytes,
                                                      _recordSerializer.getKeySize() + 1 +
                                                      _recordSerializer.getValueSize()); // TODO

            return Id(recordOffset.getOffset() /
                      (_recordSerializer.getKeySize() + 1 + _recordSerializer.getValueSize())); // TODO how should do it
        }

        std::optional<Record> get(const Id &id) {
            char *recordInBytes = _dataFile.read(_recordSerializer.getKeySize() + 1 + _recordSerializer.getValueSize(),
                                                 FileOffset((_recordSerializer.getKeySize() + 1 +
                                                             _recordSerializer.getValueSize()) *
                                                            id.getId()));

            Record record = _recordSerializer.bytesToRecord(recordInBytes);
            return record.getIsOutdated() ? std::nullopt : std::make_optional(record);
        }

        void remove(const Id &id) {
            FileOffset bitOutdatedOffset =
                    FileOffset(id.getId() * (_recordSerializer.getKeySize() + 1 + _recordSerializer.getValueSize()) +
                               _recordSerializer.getKeySize());
            _dataFile.writeByOffset(bitOutdatedOffset, new char[1]{'1'}, 1);
        }

        void clear() {
            _dataFile.clear();
        }

    private:
        File &_dataFile;
        RecordSerializer _recordSerializer;
    };
}

#endif //KEYVALUESTORAGE_STORAGE_HPP
