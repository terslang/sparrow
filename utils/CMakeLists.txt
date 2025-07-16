find_package(Freetype REQUIRED)

add_library(utils font_manager.cc exceptions.h)

target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(utils PRIVATE Freetype::Freetype)
target_include_directories(utils PUBLIC ${CMAKE_BINARY_DIR})


