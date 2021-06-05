#ifndef KEYVALUESTORAGE_LOG_HPP
#define KEYVALUESTORAGE_LOG_HPP

#include "storage/LogicStorage.hpp"
#include <unordered_map>
#include <model/TrieNode.hpp>
#include <model/InMemoryTrieNode.hpp>

namespace kvs {

    class Log : public LogicStorage {
    public:
        struct LogIterator {
            using iterator_category = std::forward_iterator_tag;
            using difference_type = std::ptrdiff_t;

            LogIterator(const Log &log) : _begin(log._logMap.begin()),
                                          _end(log._logMap.end()) {}

            std::unordered_map<Key, Id>::const_iterator::reference operator*() const { return *m_ptr; }

            std::unordered_map<Key, Id>::const_iterator operator->() { return m_ptr; }

            LogIterator &operator++() {
                m_ptr++;
                return *this;
            }

            // Postfix
            LogIterator operator++(int) {
                LogIterator tmp = *this;
                ++(*this);
                return tmp;
            }

            friend bool operator==(const LogIterator &a, const std::unordered_map<Key, Id>::const_iterator &b) {
                return a.m_ptr == b;
            };

            friend bool operator!=(const LogIterator &a, const std::unordered_map<Key, Id>::const_iterator &b) {
                return a.m_ptr != b;
            };

            std::unordered_map<Key, Id>::const_iterator begin() { return _begin; }

            std::unordered_map<Key, Id>::const_iterator end() { return _end; }

        private:
            std::unordered_map<Key, Id>::const_iterator _begin;
            std::unordered_map<Key, Id>::const_iterator _end;
            std::unordered_map<Key, Id>::const_iterator m_ptr = _begin;
        };

        Log(std::size_t sizeLimit);

        virtual std::optional<Id> add(const Key& key, Id id) override;

        virtual std::optional<Id> remove(const Key& key) override;

        virtual std::optional<Id> get(const Key& key) override;

        virtual void clear() override;

        bool isFull() const;

        std::shared_ptr<InMemoryTrieNode> toInMemoryTrieNode();

        static std::shared_ptr<InMemoryTrieNode> toInMemoryTrieNode(std::vector<std::pair<Key, Id>> &);

    private:
        std::size_t _sizeLimit;
        std::unordered_map<Key, Id> _logMap;
    };

}

#endif //KEYVALUESTORAGE_LOG_HPP
