cmake_minimum_required(VERSION 4.0)
project(search_test)

#set(CMAKE_CXX_STANDARD 17)

# Включаем модуль FetchContent
include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)

#include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
#set(gtest_disable_pthreads on)
#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(search_test index_tests.cpp server_tests.cpp
        ${CMAKE_SOURCE_DIR}/src/inverted_index.cpp
        ${CMAKE_SOURCE_DIR}/src/search_server.cpp
)

target_include_directories(search_test PUBLIC ${gtest_SOURCE_DIR}/include ${INCLUDE_DIR})

target_link_libraries(search_test PRIVATE gtest_main)

#include(GoogleTest)
#gtest_discover_tests(search_test)