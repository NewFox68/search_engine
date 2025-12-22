cmake_minimum_required(VERSION 4.0)
project(search_test)

# Включаем модуль FetchContent
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
        GIT_TAG release-1.12.1
)

# Включаем googletest в проект
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(search_test index_tests.cpp server_tests.cpp
        ${INCLUDE_DIR}/converterjson.h
        ${RESOURCES}/converterjson.cpp

        ${INCLUDE_DIR}/invertedindex.h
        ${RESOURCES}/invertedindex.cpp

        ${INCLUDE_DIR}/searchserver.h
        ${RESOURCES}/searchserver.cpp
)

target_include_directories(search_test PUBLIC ${gtest_SOURCE_DIR}/include ${INCLUDE_DIR})

target_link_libraries(search_test PRIVATE gtest_main nlohmann_json::nlohmann_json)
