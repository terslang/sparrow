add_library(sparrow_utils
    font_manager.cc
)

target_include_directories(sparrow_utils
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

find_package(SDL2 REQUIRED)
find_package(Freetype REQUIRED)

target_link_libraries(sparrow_utils
    PUBLIC SDL2::SDL2 SDL2::SDL2main Freetype::Freetype
)
