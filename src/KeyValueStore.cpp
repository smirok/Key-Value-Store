#include "KeyValueStore.hpp"

namespace kvs {

    KeyValueStore::KeyValueStore(BloomFilter &bloomFilter,
                                 Log &log, Trie &trie, Storage<Record> &recordStorage) :
            _bloomFilter(bloomFilter),
            _log(log),
            _trie(trie),
            _recordStorage(recordStorage) {

    }

    void KeyValueStore::add(const KeyValue &keyValue) {
        const Key &key = keyValue.getKey();
        const Value &value = keyValue.getValue();

        Record record(key, false, value);

        Id recordId = _recordStorage.add(record);

        std::optional<Id> optionalOldId = _log.add(key, recordId);
        if (optionalOldId.has_value()) {
            _recordStorage.remove(optionalOldId.value());
        }

        if (_log.isFull()) {
            std::shared_ptr<InMemoryTrieNode> smallTrie = _log.toInMemoryTrieNode();
            _trie.merge(smallTrie);
            _recordStorage.removeRecordsById(_trie.getRecordsToDelete());

            for (auto logIterator = Log::LogIterator(_log); logIterator != logIterator.end(); ++logIterator) {
                _bloomFilter.add(logIterator->first);
            }

            _log.clear();
        }

        if (_recordStorage.isFull()) {
            rebuild();
        }
    }

    std::optional<KeyValue> KeyValueStore::get(const Key &key) const {
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
        std::optional<Id> optionalRecordId = _log.remove(key);

        if (optionalRecordId.has_value()) {
            _recordStorage.remove(optionalRecordId.value());
        } else {
            if (_bloomFilter.mightContains(key)) {
                std::optional<Id> optionalOldId = _trie.remove(key);
                if (optionalOldId.has_value()) {
                    _recordStorage.remove(optionalOldId.value());
                }
            }
        }

        if (_recordStorage.isFull()) {
            rebuild();
        }
    }

    void KeyValueStore::clear() {
        _log.clear();
        _trie.clear();
        _bloomFilter = BloomFilter(_recordStorage.size() * 5);
        _recordStorage.clear();
    }

    void KeyValueStore::rebuild() {
        std::shared_ptr<InMemoryTrieNode> smallTrie = _log.toInMemoryTrieNode();
        _trie.merge(smallTrie);
        _recordStorage.removeRecordsById(_trie.getRecordsToDelete());

        for (auto logIterator = Log::LogIterator(_log); logIterator != logIterator.end(); ++logIterator) {
            _bloomFilter.add(logIterator->first);
        }

        _log.clear();

        _recordStorage.rebuild();
        _trie.clear();

        std::vector<std::pair<Key, Id>> records;
        for (auto iterator = Storage<Record>::RecordStorageIterator(_recordStorage);
             iterator != iterator.end(); ++iterator) {
            Record record = *iterator;

            records.emplace_back(record.getKey(), iterator.currentId());

            if (records.size() == 5000) {
                std::shared_ptr<InMemoryTrieNode> trieNode = Log::toInMemoryTrieNode(records);
                _trie.merge(trieNode);
                _recordStorage.removeRecordsById(_trie.getRecordsToDelete());
                records.clear();
            }
        }

        std::shared_ptr<InMemoryTrieNode> trieNode = Log::toInMemoryTrieNode(records);
        _trie.merge(trieNode);
        _recordStorage.removeRecordsById(_trie.getRecordsToDelete());
    }
}