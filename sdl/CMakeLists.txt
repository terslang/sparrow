find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)

add_executable(sparrow main.cc sdl_drawer.cc sdl_drawer_interface.cc)

target_link_libraries(sparrow PRIVATE SDL2::SDL2 SDL2_ttf::SDL2_ttf SDL2_image::SDL2_image layout)

target_include_directories(sparrow PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


