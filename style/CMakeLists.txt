add_library(sparrow_style
    style_node_parser.cc
    style_node.cc
)

target_include_directories(sparrow_style
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(styleprog main.cc)

target_link_libraries(sparrow_style
    PUBLIC sparrow_utils sparrow_html sparrow_css
)

target_link_libraries(styleprog sparrow_style)
