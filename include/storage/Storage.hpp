#ifndef KEYVALUESTORAGE_STORAGE_HPP
#define KEYVALUESTORAGE_STORAGE_HPP

#include "model/Id.hpp"
#include "model/KeyValue.hpp"
#include "storage/File.hpp"
#include "serializer/RecordSerializer.hpp"
#include "model/TrieNode.hpp"
#include "serializer/TrieNodeSerializer.hpp"
#include <optional>
#include <iostream>
#include "model/InMemoryTrieNode.hpp"

namespace kvs {

    /**
     * \brief Интерфейс хранилища пар < @p T, @p Id>. Прослойка между хранилищем-логикой и записью на диск.
     * @tparam T Тип хранимых объектов.
     */
    template<typename T>
    class Storage {
    public:
        /**
         * Добавить @p obj в хранилище и присвоить ему идентификатор внутри хранилища.
         * @param obj Добавляемый объект.
         * @return Присвоенный идентификатор.
         */
        virtual Id add(const T &obj) = 0;

        /**
         * Получить объект по @p id.
         * @param id Идентификатор объекта.
         * @return @p T обернутый в @p std::optional, если в @p Storage есть какой-то объект с @p id;
         * пустой @p std::optional иначе.
         */
        virtual std::optional<T> get(const Id &id) const = 0;

        /**
         * Удаляет объект с @p id. Если объекта нет, ничего не делает.
         * @param id Идентификатор удаляемого объекта.
         */
        virtual void remove(const Id &id) = 0;

        /**
         * Очищает хранилище.
         */
        virtual void clear() = 0;
    };

    /**
     * \brief Класс для хранения @p Record на диске.
     */
    template<>
    class Storage<Record> {
    public:
        /**
         * Итератор для итерации по @p Storage<Record>
         */
        struct RecordStorageIterator {
            using value_type = Record;
            using pointer = value_type *;
            using reference = value_type &;

            explicit RecordStorageIterator(Storage<Record> &storage, std::size_t ptr = 0) : _storage(storage),
                                                                                            _begin(0),
                                                                                            _end(storage.size()),
                                                                                            m_ptr(ptr) {}

            value_type operator*() const { return _storage.getAnyway(Id(m_ptr)).value(); }

            void operator++() {
                m_ptr++;
            }

            [[nodiscard]] Id currentId() const { return Id(m_ptr); };

            friend bool
            operator==(const RecordStorageIterator &a, const RecordStorageIterator &b) {
                return a.m_ptr == b.m_ptr;
            };

            friend bool
            operator!=(const RecordStorageIterator &a, const RecordStorageIterator &b) {
                return a.m_ptr != b.m_ptr;
            };

            RecordStorageIterator begin() { return RecordStorageIterator(_storage, _begin); }

            RecordStorageIterator end() { return RecordStorageIterator(_storage, _end); }

        private:
            Storage<Record> &_storage;
            std::size_t _begin;
            std::size_t _end;
            std::size_t m_ptr = _begin;
        };

        /**
         *
         * @param dataFile Файл для записи @p Record-ов.
         * @param recordSerializer Класс, сериализующий @p Record-ы.
         */
        Storage(File &dataFile, const RecordSerializer &recordSerializer) : _dataFile(dataFile),
                                                                            _recordSerializer(recordSerializer) {
            _recordSize = _recordSerializer.getKeySize() + 1 + _recordSerializer.getValueSize();
        }

        /**
         * Добавить @p record в хранилище и присвоить ему идентификатор внутри хранилища.
         * @param record Добавляемая запись.
         * @return Присвоенный идентификатор.
         */
        Id add(const Record &record) {
            char *recordInBytes = _recordSerializer.recordToBytes(record);
            FileOffset recordOffset = _dataFile.write(recordInBytes, _recordSize);

            delete[] recordInBytes;

            _countAllRecords++;

            return Id(recordOffset.getOffset() / _recordSize);
        }

        /**
         * Получить @p Record по @p id.
         * @param id Идентификатор объекта.
         * @return @p Record обернутый в @p std::optional, если в @p Storage есть какой-то объект с @p id;
         * пустой @p std::optional иначе.
         */
        [[nodiscard]] std::optional<Record> get(const Id &id) const {
            char *recordInBytes = _dataFile.read(_recordSize, FileOffset(_recordSize * id.getId()));

            Record record = _recordSerializer.bytesToRecord(recordInBytes);
            return record.getIsOutdated() ? std::nullopt : std::make_optional(record);
        }

        /**
         * Получить @p Record по @p id.
         * Контракт : объект с таким @p id всегда существует, но возможно он устарел.
         * @param id Идентификатор объекта.
         * @return @p Record обернутый в @p std::optional.
         */
        std::optional<Record> getAnyway(const Id &id) {
            char *recordInBytes = _dataFile.read(_recordSize, FileOffset(_recordSize * id.getId()));

            Record record = _recordSerializer.bytesToRecord(recordInBytes);
            return record;
        }

        /**
         * Удаляет объект с @p id. Если объекта нет, ничего не делает.
         * @param id Идентификатор удаляемого объекта.
         */
        void remove(const Id &id) {
            auto bitOutdatedOffset = FileOffset(id.getId() * _recordSize + _recordSerializer.getKeySize());
            _dataFile.writeByOffset(bitOutdatedOffset, new char[1]{'1'}, 1);

            _countRemovedRecords++;
        }

        /**
         * Очищает хранилище.
         */
        void clear() {
            _dataFile.clear();
        }

        /**
         * Проверяет, заполнено ли хранилище.
         * @return @p true, если заполнено; @p false иначе.
         */
        [[nodiscard]] bool isFull() const {
            return _countAllRecords > 1000 && (_countAllRecords < 2 * _countRemovedRecords);
        }

        /**
         * Удаляет из хранилища все записи с переданными @p Id
         * @param idsToRemove Массив @p Id, по которым надо удалить записи.
         */
        void removeRecordsById(const std::vector<Id> &idsToRemove) {
            for (const Id &id : idsToRemove) {
                remove(id);
            }
        }

        /**
         * Перестраивает хранилище, удаляя устаревшие записи и оставляя остальные.
         */
        void rebuild() {
            auto readIterator = Storage<Record>::RecordStorageIterator(*this);
            auto writeIterator = Storage<Record>::RecordStorageIterator(*this);

            int cnt = 0;
            for (; readIterator != readIterator.end(); ++readIterator) {
                Record record = *readIterator;

                if (!record.getIsOutdated()) {
                    _dataFile.writeByOffset(FileOffset(_recordSize * writeIterator.currentId().getId()),
                                            _recordSerializer.recordToBytes(record),
                                            _recordSize);

                    ++writeIterator;
                    cnt++;
                }
            }

            _countAllRecords -= _countRemovedRecords;
            _countRemovedRecords = 0;
        }

        /**
         * @return Размер хранилища.
         */
        [[nodiscard]] std::size_t size() const {
            return _countAllRecords;
        }

    private:
        std::size_t _countRemovedRecords = 0;
        std::size_t _countAllRecords = 0;
        std::size_t _recordSize;
        File &_dataFile;
        RecordSerializer _recordSerializer;
    };

    /**
     * \brief Класс для хранения @p Record на диске.
     */
    template<>
    class Storage<TrieNode> {
    public:
        /**
         *
         * @param trieNodeFile Файл для записи @p TrieNode.
         * @param trieNodeSerializer Класс, сериализующий @p TrieNode-ы.
         */
        Storage(File &trieNodeFile, const TrieNodeSerializer &trieNodeSerializer) : _trieNodeFile(trieNodeFile),
                                                                                    _trieNodeSerializer(
                                                                                            trieNodeSerializer) {
        }

        /**
         * Добавить @p trieNode в хранилище и присвоить ей идентификатор внутри хранилища.
         * @param trieNode Добавляемая нода.
         * @return Присвоенный идентификатор.
         */
        Id add(const TrieNode &trieNode) {
            char *recordInBytes = _trieNodeSerializer.trieNodeToBytes(trieNode);
            FileOffset recordOffset = _trieNodeFile.write(recordInBytes, _trieNodeSize);

            delete[] recordInBytes;

            return Id(recordOffset.getOffset() / _trieNodeSize);
        }

        /**
         * Получить @p TrieNode по @p id. Контракт : по @p id всегда есть @p TrieNode.
         * @param id Идентификатор объекта.
         * @return @p TrieNode обернутый в @p std::optional.
         */
        [[nodiscard]] std::optional<TrieNode> get(const Id &id) const {
            char *trieNodeInBytes = _trieNodeFile.read(_trieNodeSize,
                                                       FileOffset(_trieNodeSize * id.getId()));

            TrieNode trieNode = _trieNodeSerializer.bytesToTrieNode(trieNodeInBytes);

            delete[] trieNodeInBytes;

            return std::make_optional(trieNode);
        }

        void remove(const Id &id) = delete;

        /**
         * Записать по @p id новую @p trieNode.
         * @param id Идентификатор.
         * @param trieNode Нода.
         */
        void replace(const Id &id, const TrieNode &trieNode) {
            char *recordInBytes = _trieNodeSerializer.trieNodeToBytes(trieNode);

            _trieNodeFile.writeByOffset(FileOffset(_trieNodeSize * id.getId()),
                                        recordInBytes,
                                        _trieNodeSize);

            delete[] recordInBytes;
        }

        /**
         * Очищает хранилище.
         */
        void clear() {
            _trieNodeFile.clear();
        }

        /**
         * Добавляет все ноды переданного бора в хранилище
         * @param node Корень переданного бора.
         * @return Идентификатор корня @p node в хранилище.
         */
        Id addTrieNodeSubtree(const std::shared_ptr<InMemoryTrieNode> &node) {
            std::vector<Id> ids(UCHAR_MAX + 1, Id(std::numeric_limits<std::size_t>::max()));

            for (std::size_t i = 0; i <= UCHAR_MAX; ++i) {
                if (node->get(i)) {
                    ids[i] = addTrieNodeSubtree(node->get(i));
                }
            }

            if (node->getId().getId() != std::numeric_limits<std::size_t>::max()) { // leaf
                std::vector<Id> leafIds(256, Id(std::numeric_limits<std::size_t>::max() - 1));
                leafIds[0] = node->getId();

                return this->add(TrieNode(leafIds));
            }

            TrieNode trieNode(ids);
            char *trieNodeInBytes = _trieNodeSerializer.trieNodeToBytes(trieNode);
            FileOffset trieNodeOffset = _trieNodeFile.write(trieNodeInBytes, _trieNodeSize);

            delete[] trieNodeInBytes;

            return Id(trieNodeOffset.getOffset() / _trieNodeSize);
        }

    private:
        std::size_t _trieNodeSize = (UCHAR_MAX + 1) * Id::getIdSize();
        File &_trieNodeFile;
        TrieNodeSerializer _trieNodeSerializer;
    };
}

#endif //KEYVALUESTORAGE_STORAGE_HPP
