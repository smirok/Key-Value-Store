cmake_minimum_required(VERSION 3.19)
project(KeyValueStorage)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(KeyValueStorage
        src/main.cpp
        src/Key.cpp include/Key.hpp
        src/Value.cpp include/Value.hpp
        src/KeyValue.cpp include/KeyValue.hpp
        src/KeyValueStore.cpp include/KeyValueStore.hpp)