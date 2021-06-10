cmake_minimum_required(VERSION 3.15.3)
project(tests)

set(TEST_DIR ${CMAKE_CURRENT_LIST_DIR})
include_directories(${TEST_DIR})

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(MODELS
        ../src/model/Key.cpp ../include/model/Key.hpp
        ../src/model/Value.cpp ../include/model/Value.hpp
        ../src/model/KeyValue.cpp ../include/model/KeyValue.hpp
        ../src/model/Id.cpp ../include/model/Id.hpp
        ../src/storage/File.cpp ../include/storage/File.hpp
        ../src/model/FileOffset.cpp ../include/model/FileOffset.hpp
        ../src/model/TrieNode.cpp ../include/model/TrieNode.hpp
        ../src/model/Record.cpp ../include/model/Record.hpp
        ../src/model/InMemoryTrieNode.cpp ../include/model/InMemoryTrieNode.hpp)

set(SERIALIZERS
        ../src/serializer/TrieNodeSerializer.cpp ../include/serializer/TrieNodeSerializer.hpp
        ../src/serializer/RecordSerializer.cpp ../include/serializer/RecordSerializer.hpp)

set(STORAGES
        ../include/storage/BloomFilter.hpp ../src/storage/BloomFilter.cpp
        ../include/KeyValueStore.hpp ../src/KeyValueStore.cpp
        ../include/storage/LogicStorage.hpp
        ../src/storage/Log.cpp ../include/storage/Log.hpp
        ../include/storage/Storage.hpp
        ../src/storage/Trie.cpp ../include/storage/Trie.hpp)

set(LIBRARIES
        ../xxHash/xxhash.h ../xxHash/xxhash.c)

set(MODEL_TEST
        model/KeyTest.cpp
        model/ValueTest.cpp
        model/KeyValueTest.cpp)

set(SERIALIZER_TEST
        serializer/TrieNodeSerializerTest.cpp
        serializer/RecordSerializerTest.cpp)

set(STORAGE_TEST
        storage/RecordStorageTest.cpp
        storage/TrieNodeStorageTest.cpp
        storage/TrieTest.cpp
        storage/KeyValueStoreTest.cpp
        storage/BloomFilterTest.cpp
        storage/LogTest.cpp
        storage/FileTest.cpp)

add_executable(tests test.cpp
        ${MODELS} ${SERIALIZERS} ${STORAGES} ${LIBRARIES} ${MODEL_TEST} ${SERIALIZER_TEST} ${STORAGE_TEST})

target_link_libraries(tests
        PRIVATE "gtest_main"
        PRIVATE "gmock_main")

add_test(
        NAME "tests"
        COMMAND ${CMAKE_BINARY_DIR}/test/tests
)

enable_testing()

# Code coverage
if (ENABLE_TEST_COVERAGE)
    set(COVERAGE_LCOV_EXCLUDES "${TEST_DIR}/*" "*/googletest/*" "*/googlemock/*" "/usr/*")
    setup_target_for_coverage_lcov(
            NAME "coverage"         # New target name
            EXECUTABLE "ctest"     # Executable to run to gather coverage stats
            DEPENDENCIES "tests"    # Dependencies to build first
    )
endif ()