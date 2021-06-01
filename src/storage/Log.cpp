#include "storage/Log.hpp"

namespace kvs {

    Log::Log(std::size_t sizeLimit) : _sizeLimit(sizeLimit) {
        _logMap.reserve(sizeLimit);
    }

    void Log::add(Key key, Id id) {
        if (_logMap.find(key) != _logMap.end()) {
            // TODO : старая запись устарела
        } else {
            _logMap.insert({key, id});
        }

        if (isFull()) {
            // TODO : сбросить в бор
            clear();
        }
    }

    void Log::remove(Key key) {
        if (get(key) == std::nullopt) {
            return;
        }

        _logMap.erase(key);
        // TODO : удалить запись
    }

    std::optional<Id> Log::get(Key key) {
        if (_logMap.find(key) != _logMap.end()) {
            return std::optional<Id>(_logMap.at(key));
        }

        return std::nullopt;
    }

    void Log::clear() {
        _logMap.clear();
        _logMap.reserve(_sizeLimit);
    }

    bool Log::isFull() const {
        return _logMap.size() >= _sizeLimit;
    }
}