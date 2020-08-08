cmake_minimum_required(VERSION 3.11)
project(tomb_of_darius)

set(CMAKE_C_STANDARD 90)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake_sdl2")

include_directories( src
		${SDL2_INCLUDE_DIR}
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DSDLSW -std=c90 -W -ansi -g -pedantic -Wall")

find_package(SDL2 REQUIRED)

add_executable(tomb_of_darius src/demo.c src/sdl2.c src/maps.c src/maps.h)

include(FindPkgConfig)

target_include_directories(tomb_of_darius PUBLIC ${SDL2_INCLUDE_DIR})
target_link_libraries(tomb_of_darius LINK_PUBLIC ${SDL2_LIBRARY} m)

