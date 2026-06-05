cmake_minimum_required(VERSION 3.16)
project(RogueliteDemo)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR}/..)

add_executable(demo
    main.cpp
    ../src/core/stats.cpp
    ../src/core/characters.cpp
    ../src/core/player.cpp
    ../src/core/consumable.cpp
    ../src/core/entity.cpp
    ../src/core/game_manager.cpp
)
