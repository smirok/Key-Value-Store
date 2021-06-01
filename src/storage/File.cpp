#include <array>
#include "storage/File.hpp"
#include "iostream"

namespace kvs {

    File::File(const std::string &fileName) : _fileName(fileName) {
        fileStream.open(fileName, std::fstream::in | std::fstream::out | std::fstream::trunc);
    }

    char *File::read(std::size_t length, FileOffset fileOffset) {
        char *buffer = new char[length];

        fileStream.seekg(fileOffset.getOffset(), std::ios_base::beg);
        fileStream.read(buffer, length);
        fileStream.seekg(0, std::ios_base::end);

        return buffer;
    }

    FileOffset File::write(const char *data, std::size_t length) {
        fileStream.seekg(0, std::ios_base::end);
        FileOffset resultOffset(fileStream.tellg());

        fileStream.write(data, length);
        fileStream.seekg(0, std::ios_base::end);

        return resultOffset;
    }

    void File::writeByOffset(const FileOffset &fileOffset, const char *data, std::size_t length) {
        fileStream.seekg(fileOffset.getOffset(), std::ios_base::beg);
        fileStream.write(data, length);

        fileStream.seekg(0, std::ios_base::end);
    }

    File::~File() {
        fileStream.close();
    }

    void File::clear() {
        fileStream.close();
        fileStream.open(_fileName, std::fstream::in | std::fstream::out | std::fstream::trunc);
    }
}