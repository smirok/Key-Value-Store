#ifndef KEYVALUESTORAGE_KEY_HPP
#define KEYVALUESTORAGE_KEY_HPP

#include <cstddef>
#include <functional>
#include <string>

namespace kvs {

    /**
     * \brief Класс, хранящий ключ и его размер
     */
    class Key {
    public:
        Key(const char *key, size_t size);

        Key(const Key &key);

        Key &operator=(const Key &key);

        Key(Key &&key) noexcept;

        Key &operator=(Key &&key) noexcept;

        ~Key();

        [[nodiscard]] const char *getKey() const;

        [[nodiscard]] size_t getSize() const;

        bool operator==(const Key &key) const;

    private:

        char *_key;
        size_t _size;
    };

}

namespace std {
    template<>
    struct hash<kvs::Key> {
        std::size_t operator()(kvs::Key const &s) const noexcept {
            std::size_t h1 = std::hash<std::string>{}(std::string(s.getKey(), s.getSize()));
            return h1;
        }
    };
}

#endif //KEYVALUESTORAGE_KEY_HPP
