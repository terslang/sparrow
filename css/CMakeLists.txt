add_library(sparrow_css
    css_parser.cc
    css_element.cc
)

target_include_directories(sparrow_css
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(cssprog main.cc)
target_link_libraries(cssprog sparrow_css)
