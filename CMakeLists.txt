cmake_minimum_required(VERSION 3.15.3)
project(KeyValueStorage)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wextra -Werror -Wshadow -Wpedantic)
include_directories(include)

add_executable(KeyValueStorage
        src/main.cpp
        src/Key.cpp include/Key.hpp
        src/Value.cpp include/Value.hpp
        src/KeyValue.cpp include/KeyValue.hpp
        src/KeyValueStore.cpp include/KeyValueStore.hpp
        src/BloomFilter.cpp include/BloomFilter.hpp
        xxHash/xxhash.h xxHash/xxhash.c
        src/Id.cpp include/Id.hpp
        include/LogicStorage.hpp
        src/Log.cpp include/Log.hpp
        include/Storage.hpp
        src/File.cpp include/File.hpp
        src/FileOffset.cpp include/FileOffset.hpp
        src/Record.cpp include/Record.hpp
        src/TrieNode.cpp include/TrieNode.hpp
        src/RecordSerializer.cpp include/RecordSerializer.hpp
        src/TrieNodeSerializer.cpp include/TrieNodeSerializer.hpp)

enable_testing()
add_subdirectory("test")