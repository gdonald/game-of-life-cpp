cmake_minimum_required(VERSION 3.10.2)
project(game_of_life)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_COMPILER clang)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

include(FindPackageHandleStandardArgs)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_gfx REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(include
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_IMAGE_INCLUDE_DIRS}
        ${SDL2_GFX_INCLUDE_DIRS}
        ${SDL2_TTF_INCLUDE_DIRS})

add_executable(game_of_life
        src/Game.cpp
        src/main.cpp)

target_link_libraries(game_of_life
        ${SDL2_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_GFX_LIBRARIES}
        ${SDL2_TTF_LIBRARIES})

add_custom_command(TARGET game_of_life
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:game_of_life> ..)