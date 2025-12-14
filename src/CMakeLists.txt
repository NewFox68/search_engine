cmake_minimum_required(VERSION 4.0)
project(search_engine)

# get JSON from github
include(FetchContent)
FetchContent_Declare(json
        URL https://github.com/nlohmann/json/archive/refs/heads/develop.zip
        GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(json)

# копируем файлы в каталог сборки программы
configure_file(${CMAKE_SOURCE_DIR}/config/config.json ${CMAKE_CURRENT_BINARY_DIR})
configure_file(${CMAKE_SOURCE_DIR}/config/requests.json ${CMAKE_CURRENT_BINARY_DIR})

add_executable(search_engine main.cpp
        ${INCLUDE_DIR}/converterjson.h
        ${INCLUDE_DIR}/invertedindex.h
        ${INCLUDE_DIR}/searchserver.h
        converterjson.cpp
        invertedindex.cpp
        searchserver.cpp)

target_include_directories(search_engine PUBLIC ${INCLUDE_DIR})

target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json)

