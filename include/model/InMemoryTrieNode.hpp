#ifndef KEYVALUESTORAGE_INMEMORYTRIENODE_HPP
#define KEYVALUESTORAGE_INMEMORYTRIENODE_HPP

#include <memory>
#include <climits>
#include "Key.hpp"
#include "Id.hpp"

namespace kvs {

    /**
     * \brief Класс, реализующий ноду бора в оперативной памяти.
     */
    class InMemoryTrieNode {
    public:
        /**
         * Инициализируем детей @p nullptr-ами.
         */
        InMemoryTrieNode();

        /**
         *
         * @param currentNode Нода, к которой как корню подвешивается @p key с @p id.
         * @param key Ключ, который хотим добавить в дерево @p currentNode.
         * @param id Идентификатор записи, соотвествующей ключу @p key.
         */
        static void add(std::shared_ptr<InMemoryTrieNode> currentNode, const Key &key, const Id &id);

        /**
         * Превращение @p Log-а в бор, хранящийся в оперативной памяти.
         * @param keyIdPairs Массив пар < @p Key, @p Id - идентификатор @p Record-a>, из которого будет строиться бор.
         * @return Указатель на корень получившегося бора.
         */
        static std::shared_ptr<InMemoryTrieNode> toInMemoryTrieNode(std::vector<std::pair<Key, Id>> &keyIdPairs);

        /**
         *
         * @param index Номер ребёнка ноды.
         * @return Указатель на @p index-ового ребёнка.
         */
        [[nodiscard]] std::shared_ptr<InMemoryTrieNode> get(std::size_t index) const;

        /**
         *
         * @return Идентификатор соответствующей ноде записи. Если ноде не соответствует запись,
         * то @p std::numeric_limits<std::size_t>::max().
         */
        [[nodiscard]] Id getId() const;

    private:
        /** 256 @p shared_ptr указателей на детей текущей ноды. */
        std::shared_ptr<InMemoryTrieNode> _children[256];

        /** Если нода - лист бора, то @p _id.getId() задаёт запись, соответствующую этому ключу,
         *  иначе @p _id.getId() == std::numeric_limits<std::size_t>::max().
         */
        Id _id = Id();
    };

}

#endif //KEYVALUESTORAGE_INMEMORYTRIENODE_HPP
