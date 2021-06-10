cmake_minimum_required(VERSION 3.15.3)
project(KeyValueStorage)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wextra -Werror -Wshadow -Wpedantic)
include_directories(include)

set(MODELS
        src/model/Key.cpp include/model/Key.hpp
        src/model/Value.cpp include/model/Value.hpp
        src/model/KeyValue.cpp include/model/KeyValue.hpp
        src/model/Id.cpp include/model/Id.hpp
        src/storage/File.cpp include/storage/File.hpp
        src/model/FileOffset.cpp include/model/FileOffset.hpp
        src/model/TrieNode.cpp include/model/TrieNode.hpp
        src/model/Record.cpp include/model/Record.hpp
        src/model/InMemoryTrieNode.cpp include/model/InMemoryTrieNode.hpp)

set(SERIALIZERS
        src/serializer/TrieNodeSerializer.cpp include/serializer/TrieNodeSerializer.hpp
        src/serializer/RecordSerializer.cpp include/serializer/RecordSerializer.hpp)

set(STORAGES
        src/KeyValueStore.cpp include/KeyValueStore.hpp
        src/storage/BloomFilter.cpp include/storage/BloomFilter.hpp
        include/storage/LogicStorage.hpp
        src/storage/Log.cpp include/storage/Log.hpp
        include/storage/Storage.hpp
        src/storage/Trie.cpp include/storage/Trie.hpp)

set(LIBRARIES
        xxHash/xxhash.h xxHash/xxhash.c)

add_executable(benchmark
        src/benchmark.cpp
        ${MODELS} ${SERIALIZERS} ${STORAGES} ${LIBRARIES})

add_executable(kvs
        src/main.cpp
        ${MODELS} ${SERIALIZERS} ${STORAGES} ${LIBRARIES})

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

option(ENABLE_TEST_COVERAGE "Enable test coverage" OFF)

if (ENABLE_TEST_COVERAGE)
    include(CodeCoverage)
    append_coverage_compiler_flags()
endif ()

enable_testing()
add_subdirectory("test")
