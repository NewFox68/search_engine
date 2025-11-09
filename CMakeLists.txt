cmake_minimum_required(VERSION 4.0)
project(search_engine)

set(CMAKE_CXX_STANDARD 17)

set(JSON_BuildTests OFF CACHE INTERNAL "")

add_subdirectory(nlohmann_json)

add_executable(search_engine main.cpp
        converterjson.h
        converterjson.cpp
        invertedindex.h
        invertedindex.cpp
        searchserver.h
        searchserver.cpp)

target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json)