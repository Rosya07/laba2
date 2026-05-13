cmake_minimum_required(VERSION 3.16)
project(SeaBattle)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include(FetchContent)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG master
)

FetchContent_MakeAvailable(raylib)

add_executable(SeaBattle
    src/main.cpp
    src/board.cpp
    src/bot.cpp
    src/game.cpp
    src/render.cpp
)

target_include_directories(SeaBattle PRIVATE src)
target_link_libraries(SeaBattle raylib)
