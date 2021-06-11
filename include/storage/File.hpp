#ifndef KEYVALUESTORAGE_FILE_HPP
#define KEYVALUESTORAGE_FILE_HPP

#include <string>
#include <vector>
#include "model/FileOffset.hpp"
#include <fstream>

namespace kvs {

    /**
     * \brief Класс, отвечающий за запись и чтение в файл.
     */
    class File {
    public:
        /**
         *
         * @param fileName Название оборачиваемого файла.
         */
        explicit File(const std::string &fileName);

        /**
         * Закрываем файл.
         */
        ~File();

        /**
         *
         * @param length Количество читаемых байт.
         * @param fileOffset Позиция в файле, с которой начинается чтение.
         * @return Массив прочитанных байт.
         */
        char *read(std::size_t length, FileOffset fileOffset);

        /**
         *
         * @param data Массив записываемых байт.
         * @param length Количество записываемых байт.
         * @return Позиция в файле на только что записанные данные.
         */
        FileOffset write(const char *data, std::size_t length);

        /**
         *
         * @param fileOffset Позиция в файле, начиная с которой произойдет запись.
         * @param data Массив записываемых байт.
         * @param length Количество записываемых байт.
         */
        void writeByOffset(const FileOffset &fileOffset, const char *data, std::size_t length);

        /**
         * Очищает файл.
         */
        void clear();

    private:
        std::fstream fileStream;
        std::string _fileName;
    };

}

#endif //KEYVALUESTORAGE_FILE_HPP
