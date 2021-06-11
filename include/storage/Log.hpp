#ifndef KEYVALUESTORAGE_LOG_HPP
#define KEYVALUESTORAGE_LOG_HPP

#include "storage/LogicStorage.hpp"
#include <unordered_map>
#include <model/TrieNode.hpp>
#include <model/InMemoryTrieNode.hpp>

namespace kvs {

    /**
     * \brief Класс, представляющий хранилище данных в оперативной памяти, оборачивающий @p std::unordered_map
     * и хранящий пары < @p Key, @p Id> .
     */
    class Log : public LogicStorage {
    public:
        /**
         * \brief Итератор @p Log-а по @p _logMap.
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
         *
         * @param sizeLimit Максимальный размер хранилища.
         */
        explicit Log(std::size_t sizeLimit);

        /**
         * Добавление в @p Log по ключу @p key значение @p id.
         * Если ранее в @p Log уже был какой-то идентификатор по ключу @p key, то его @p Id обновляется
         * @param key Ключ.
         * @param id Идентификатор.
         * @return @p Id обернутый в @p std::optional, если ранее в @p Log уже был какой-то идентификатор по ключу @p key;
         * пустой @p std::optional иначе
         */
        std::optional<Id> add(const Key &key, const Id &id) override;

        /**
         * Удаление пары c ключом @p key.
         * @param key Ключ.
         * @return @p Id обернутый в @p std::optional, если ранее в @p Log уже был какой-то идентификатор по ключу @p key;
         * пустой @p std::optional иначе.
         */
        std::optional<Id> remove(const Key &key) override;

        /**
         * Получить @p Id по ключу @p key.
         * @param key Ключ.
         * @return @p Id обернутый в @p std::optional, если в @p Log есть какой-то идентификатор по ключу @p key;
         * пустой @p std::optional иначе.
         */
        std::optional<Id> get(const Key &key) const override;

        /**
         * Очищает @p Log.
         */
        void clear() override;

        /**
         * Проверяет, переполнен ли @p Log.
         * @return true, если переполнен; false иначе.
         */
        bool isFull() const;

        /**
         * Превращение @p Log-а в бор, хранящийся в оперативной памяти.
         * @return Указатель на корень получившегося бора.
         */
        std::shared_ptr<InMemoryTrieNode> toInMemoryTrieNode() const;

    private:
        std::size_t _sizeLimit;
        std::unordered_map<Key, Id> _logMap;
    };

}

#endif //KEYVALUESTORAGE_LOG_HPP
