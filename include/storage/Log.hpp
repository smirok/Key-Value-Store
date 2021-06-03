#ifndef KEYVALUESTORAGE_LOG_HPP
#define KEYVALUESTORAGE_LOG_HPP

#include "storage/LogicStorage.hpp"
#include <unordered_map>
#include <model/TrieNode.hpp>
#include <model/InMemoryTrieNode.hpp>

namespace kvs {

    class Log : public LogicStorage {
    public:
        Log(std::size_t sizeLimit);

        virtual std::optional<Id> add(Key key, Id id) override;

        virtual std::optional<Id> remove(Key key) override;

        virtual std::optional<Id> get(Key key) override;

        virtual void clear() override;

        bool isFull() const;

        InMemoryTrieNode *toInMemoryTrieNode();

        static InMemoryTrieNode *toInMemoryTrieNode(std::vector<std::pair<Key, Id>> &);

    private:
        std::size_t _sizeLimit;
        std::unordered_map<Key, Id> _logMap;
    };

}

#endif //KEYVALUESTORAGE_LOG_HPP
