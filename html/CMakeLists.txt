add_library(sparrow_html
    html_parser.cc
    dom.cc
)

target_include_directories(sparrow_html
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(htmlprog main.cc)
target_link_libraries(htmlprog sparrow_html)
