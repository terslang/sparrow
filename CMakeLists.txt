cmake_minimum_required(VERSION 3.15)
project(sparrow LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(utils)
add_subdirectory(html)
add_subdirectory(css)
add_subdirectory(style)
add_subdirectory(layout)
add_subdirectory(sdl)
