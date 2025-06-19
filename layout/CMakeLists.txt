add_library(sparrow_layout
    layout_node.cc
)

target_include_directories(sparrow_layout
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(layoutprog main.cc)

target_link_libraries(sparrow_layout
    PUBLIC sparrow_utils sparrow_style sparrow_html sparrow_css
)

target_link_libraries(layoutprog sparrow_layout)
