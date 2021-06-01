cmake_minimum_required(VERSION 3.15.3)
project(KeyValueStorage)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wextra -Werror -Wshadow -Wpedantic)
include_directories(include)

add_executable(KeyValueStorage
        src/main.cpp
        src/model/Key.cpp include/model/Key.hpp
        src/model/Value.cpp include/model/Value.hpp
        src/model/KeyValue.cpp include/model/KeyValue.hpp
        src/KeyValueStore.cpp include/KeyValueStore.hpp
        src/storage/BloomFilter.cpp include/storage/BloomFilter.hpp
        xxHash/xxhash.h xxHash/xxhash.c
        src/model/Id.cpp include/model/Id.hpp
        include/storage/LogicStorage.hpp
        src/storage/Log.cpp include/storage/Log.hpp
        include/storage/Storage.hpp
        src/storage/File.cpp include/storage/File.hpp
        src/model/FileOffset.cpp include/model/FileOffset.hpp
        src/model/TrieNode.cpp include/model/TrieNode.hpp
        src/model/Record.cpp include/model/Record.hpp
        src/serializer/TrieNodeSerializer.cpp include/serializer/TrieNodeSerializer.hpp
        src/serializer/RecordSerializer.cpp include/serializer/RecordSerializer.hpp)

enable_testing()
add_subdirectory("test")