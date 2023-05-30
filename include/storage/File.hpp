#ifndef KEYVALUESTORAGE_FILE_HPP
#define KEYVALUESTORAGE_FILE_HPP

#include <string>
#include <vector>
#include "model/FileOffset.hpp"
#include <fstream>

namespace kvs {

    /**
     * \brief Class responsible for read and write operations with file.
     */
    class File {
    public:
        /**
         * @param fileName File name.
         */
        explicit File(const std::string &fileName);

        /**
         * Close the file.
         */
        ~File();

        /**
         * @param length Number of reading bytes.
         * @param fileOffset First position in the file to read.
         * @return Array of read bytes.
         */
        char *read(std::size_t length, FileOffset fileOffset);

        /**
         * @param data Array of read bytes.
         * @param length Number of writing bytes.
         * @return Position in the file on written data.
         */
        FileOffset write(const char *data, std::size_t length);

        /**
         *
         * @param fileOffset Position in the file to write.
         * @param data Array of bytes to write.
         * @param length Number of written bytes.
         */
        void writeByOffset(const FileOffset &fileOffset, const char *data, std::size_t length);

        /**
         * Clean up file.
         */
        void clear();

    private:
        std::fstream fileStream;
        std::string _fileName;
    };

} // kvs

#endif //KEYVALUESTORAGE_FILE_HPP
