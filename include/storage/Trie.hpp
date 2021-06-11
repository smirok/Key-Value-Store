#ifndef KEYVALUESTORAGE_TRIE_HPP
#define KEYVALUESTORAGE_TRIE_HPP

#include <model/Key.hpp>
#include <model/Id.hpp>
#include <model/TrieNode.hpp>
#include <model/InMemoryTrieNode.hpp>
#include "storage/Storage.hpp"
#include "LogicStorage.hpp"

namespace kvs {

    /**
     * \brief Класс, представляющий хранилище данных бор на диске и хранящий пары < @p Key, @p Id>.
     */
    class Trie : public LogicStorage {
    public:
        /**
         *
         * @param _storage Хранилище @p TrieNode нод бора, отвечающее только за доступ к нодам.
         */
        explicit Trie(Storage<TrieNode> &_storage);

        /**
         * Добавление в @p Trie по ключу @p key значение @p id.
         * Если ранее в @p Trie уже был какой-то идентификатор по ключу @p key, то его @p Id обновляется
         * @param key Ключ.
         * @param id Идентификатор.
         * @return @p Id обернутый в @p std::optional, если ранее в @p Trie уже был какой-то идентификатор по ключу @p key;
         * пустой @p std::optional иначе
         */
        std::optional<Id> add(const Key &, const Id &) override;

        /**
         * Удаление пары c ключом @p key.
         * @param key Ключ.
         * @return @p Id обернутый в @p std::optional, если ранее в @p Trie уже был какой-то идентификатор по ключу @p key;
         * пустой @p std::optional иначе.
         */
        std::optional<Id> remove(const Key &) override;

        /**
        * Получить @p Id по ключу @p key.
        * @param key Ключ.
        * @return @p Id обернутый в @p std::optional, если в @p Trie есть какой-то идентификатор по ключу @p key;
        * пустой @p std::optional иначе.
        */
        [[nodiscard]] std::optional<Id> get(const Key &) const override;

        /**
         * Очищает @p Trie.
         */
        void clear() override;

        /**
         * Сливает бор в оперативной памяти с корнем @p smallTrieRoot в текущий бор.
         * @param smallTrieRoot Корень бора в оперативной памяти.
         * @return Массив @p Id устаревших в результате слияния записей.
         */
        const std::vector<Id> &merge(const std::shared_ptr<InMemoryTrieNode> &smallTrieRoot);

    private:
        std::vector<Id> _recordsToDelete;

        void addRoot();

        Id merge(const Id &trieNodeId, const std::shared_ptr<InMemoryTrieNode> &smallTrieRoot);

        [[nodiscard]] std::optional<std::pair<TrieNode, Id>> traverse(const Key &key, bool shouldCreateNode) const;

        Storage<TrieNode> &_storage;
    };
}

#endif //KEYVALUESTORAGE_TRIE_HPP
