find_package(Freetype REQUIRED)

add_library(style style_node.cc style_node_parser.cc)

target_include_directories(style PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(style utils html css Freetype::Freetype)
