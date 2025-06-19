add_library(sparrow_drawer
    sdl_drawer.cc
    sdl_drawer_interface.cc
)

target_include_directories(sparrow_drawer
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(Freetype REQUIRED)

add_executable(drawerprog main.cc)

target_link_libraries(sparrow_drawer
    PUBLIC sparrow_layout SDL2::SDL2 SDL2_image::SDL2_image Freetype::Freetype
)

target_link_libraries(drawerprog sparrow_drawer)
