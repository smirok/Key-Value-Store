#include "KeyValueStore.hpp"

namespace kvs {

    KeyValueStore::KeyValueStore(const BloomFilter &bloomFilter,
                                 const Log &log, const Trie &trie, const Storage<Record> &recordStorage) :
            _bloomFilter(bloomFilter),
            _log(log),
            _trie(trie),
            _recordStorage(recordStorage) {

    }

    void KeyValueStore::add(const KeyValue &keyValue) {
        Key key = keyValue.getKey();
        Value value = keyValue.getValue();

        Record record(key, false, value);

        Id recordId = _recordStorage.add(record);

        std::optional<Id> optionalOldId = _log.get(key);
        if (!optionalOldId.has_value()) {
            _recordStorage.remove(optionalOldId.value());
        }

        _log.add(key, recordId);

        if (_log.isFull()) {
            // TODO превращаемся в бор
            // _trie.merge(smallTrie);
            // добавить log в фильтр;
        }
    }

    std::optional<KeyValue> KeyValueStore::get(const Key &key) {
        std::optional<Id> optionalRecordId = _log.get(key);

        if (optionalRecordId.has_value()) {
            Record record = _recordStorage.get(optionalRecordId.value()).value();
            return std::make_optional(KeyValue(record.getKey(), record.getValue()));
        } else {
            if (!_bloomFilter.mightContains(key)) {
                return std::nullopt;
            } else {
                optionalRecordId = _trie.get(key);
                if (!optionalRecordId.has_value()) {
                    return std::nullopt;
                } else {
                    Record record = _recordStorage.get(optionalRecordId.value()).value();
                    return std::make_optional(KeyValue(record.getKey(), record.getValue()));
                }
            }
        }
    }

    void KeyValueStore::del(const Key &key) {
        std::optional<Id> optionalRecordId = _log.get(key);

        if (optionalRecordId.has_value()) {
            _recordStorage.remove(optionalRecordId.value());
            _log.remove(key);
        } else {
            if (_bloomFilter.mightContains(key)) {
                std::optional<Id> optionalOldId = _trie.remove(key);
                if (optionalOldId.has_value()) {
                    _recordStorage.remove(optionalOldId.value());
                }
            }
        }

        if (shouldRemoveOutdatedParts()) {
            // TODO
            removeOutdatedParts();
        }
    }

    void KeyValueStore::clear() {
        _log.clear();
        _trie.clear();
        // TODO clear filter
        _recordStorage.clear();
    }

    bool KeyValueStore::shouldRemoveOutdatedParts() {
        return false;
    }

    void KeyValueStore::removeOutdatedParts() {

    }
}