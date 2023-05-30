#ifndef KEYVALUESTORAGE_LOG_HPP
#define KEYVALUESTORAGE_LOG_HPP

#include "storage/LogicStorage.hpp"
#include <unordered_map>
#include <model/TrieNode.hpp>
#include <model/InMemoryTrieNode.hpp>

namespace kvs {

    /**
     * \brief Class representing data storage in RAM, wrapping @p std::unordered_map and storing pairs < @p Key, @p Id>.
     */
    class Log : public LogicStorage {
    public:
        /**
         * \brief @p Log iterator over @p _logMap.
         */
        struct LogIterator {
            explicit LogIterator(const Log &log) : _begin(log._logMap.begin()),
                                                   _end(log._logMap.end()) {}

            std::unordered_map<Key, Id>::const_iterator::reference operator*() const { return *m_ptr; }

            std::unordered_map<Key, Id>::const_iterator operator->() { return m_ptr; }

            LogIterator &operator++() {
                m_ptr++;
                return *this;
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

        /**
         * @param sizeLimit Maximal storage size.
         */
        explicit Log(std::size_t sizeLimit);

        /**
         * Insert value @p id in @p Log by @p key.
         * If there is an identifier by @p key, then @p Id will be updated.
         * @param key Key.
         * @param id Identifier.
         * @return @p Id wrapped into @p std::optional, if there was some identifier by @p key in @p Log;
         * empty @p std::optional otherwise.
         */
        std::optional<Id> add(const Key &key, const Id &id) override;

        /**
         * Remove key-value pair by @p key.
         * @param key Key.
         * @return @p Id wrapped into @p std::optional, if there was some identifier by @p key in @p Log;
         * empty @p std::optional otherwise.
         */
        std::optional<Id> remove(const Key &key) override;

        /**
         * Get @p Id by @p key.
         * @param key Key.
         * @return @p Id wrapped into @p std::optional, if there was some identifier by @p key in @p Log;
         * empty @p std::optional otherwise.
         */
        std::optional<Id> get(const Key &key) const override;

        /**
         * Clean up @p Log.
         */
        void clear() override;

        /**
         * Check whether @p Log is full.
         * @return true if log is full; false otherwise.
         */
        bool isFull() const;

        /**
         * Turn @p Log-а into trie storing in RAM.
         * @return Pointer on the root of resulting trie.
         */
        std::shared_ptr<InMemoryTrieNode> toInMemoryTrieNode() const;

    private:
        std::size_t _sizeLimit;
        std::unordered_map<Key, Id> _logMap;
    };

} // kvs

#endif //KEYVALUESTORAGE_LOG_HPP
