#ifndef KEYVALUESTORAGE_STORAGE_HPP
#define KEYVALUESTORAGE_STORAGE_HPP

#include "Id.hpp"
#include "KeyValue.hpp"
#include "TrieNode.hpp"
#include "File.hpp"
#include "TrieNodeSerializer.hpp"
#include "RecordSerializer.hpp"
#include <optional>

namespace kvs {

    template<typename T>
    class Storage {
    public:
        virtual Id add(const T &) = 0;

        virtual std::optional<T> get(const Id &) = 0;

        virtual void remove(const Id &) = 0;

        virtual void clear() = 0;
    };

    template<>
    class Storage<Record> {
    public:
        Storage(File &dataFile, const RecordSerializer &recordSerializer);

        Id add(const Record &record);

        std::optional<Record> get(const Id &id);

        void remove(const Id &id);

        void clear();

    private:
        File &_dataFile;
        RecordSerializer _recordSerializer;
    };

    Storage<Record>::Storage(File &dataFile, const RecordSerializer &recordSerializer)
            : _dataFile(dataFile),
              _recordSerializer(recordSerializer) {
    }

    Id Storage<Record>::add(const Record &record) {
        char *recordInBytes = _recordSerializer.recordToBytes(record);
        FileOffset recordOffset = _dataFile.write(recordInBytes,
                                                  _recordSerializer.getKeySize() + 1 +
                                                  _recordSerializer.getValueSize()); // TODO

        return Id(recordOffset.getOffset() /
                  (_recordSerializer.getKeySize() + 1 + _recordSerializer.getValueSize())); // TODO how should do it
    }

    std::optional<Record> Storage<Record>::get(const Id &id) {
        char *recordInBytes = _dataFile.read(_recordSerializer.getKeySize() + 1 + _recordSerializer.getValueSize(),
                                             FileOffset((_recordSerializer.getKeySize() + 1 +
                                                         _recordSerializer.getValueSize()) *
                                                        id.getId()));

        Record record = _recordSerializer.bytesToRecord(recordInBytes);
        return record.getIsOutdated() ? std::nullopt : std::make_optional(record);
    }

    void Storage<Record>::remove(const Id &id) {
        FileOffset bitOutdatedOffset =
                FileOffset(id.getId() * (_recordSerializer.getKeySize() + 1 + _recordSerializer.getValueSize()) +
                           _recordSerializer.getKeySize());
        _dataFile.writeByOffset(bitOutdatedOffset, new char[1]{'1'}, 1);
    }

    void Storage<Record>::clear() {
        _dataFile.clear();
    }
}


#endif //KEYVALUESTORAGE_STORAGE_HPP
