cmake_minimum_required(VERSION 4.0)
project(search_engine)
set(CMAKE_CXX_STANDARD 17)

# get JSON from github
include(FetchContent)
FetchContent_Declare(json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(json)

#set(JSON_BuildTests OFF CACHE INTERNAL "")
#add_subdirectory(nlohmann_json)
# копируем файлы в каталог сборки программы

# копируем файлы в каталог сборки программы
configure_file(${CMAKE_SOURCE_DIR}/config/config.json ${CMAKE_CURRENT_BINARY_DIR})
configure_file(${CMAKE_SOURCE_DIR}/config/request.json ${CMAKE_CURRENT_BINARY_DIR})


add_executable(search_engine main.cpp
        converterjson.cpp
        invertedindex.cpp
        searchserver.cpp)

target_include_directories(search_engine PUBLIC ${CMAKE_SOURCE_DIR}/include ${INCLUDE_DIR})

target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json)

