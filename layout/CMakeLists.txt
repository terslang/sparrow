add_library(layout layout_node.cc)

target_include_directories(layout PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(layout utils style)
