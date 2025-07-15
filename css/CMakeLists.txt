add_library(css css_parser.cc css_element.cc)

target_include_directories(css PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(css utils)
