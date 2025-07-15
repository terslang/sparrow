add_library(html html_parser.cc dom.cc)

target_include_directories(html PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(html utils)
