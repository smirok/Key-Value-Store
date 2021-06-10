#ifndef KEYVALUESTORAGE_FILE_HPP
#define KEYVALUESTORAGE_FILE_HPP

#include <string>
#include <vector>
#include "model/FileOffset.hpp"
#include <fstream>

namespace kvs {

    class File {
    public:
        explicit File(const std::string &fileName);

        ~File();

        char *read(std::size_t length, FileOffset fileOffset);

        FileOffset write(const char *data, std::size_t length);

        void writeByOffset(const FileOffset &fileOffset, const char *data, std::size_t length);

        void clear();

    private:
        std::fstream fileStream;
        std::string _fileName;
    };

}

#endif //KEYVALUESTORAGE_FILE_HPP
