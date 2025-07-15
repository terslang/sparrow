cmake_minimum_required(VERSION 3.10)
project(SparrowBrowser CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(ASSETS_DIR assets)

# Set paths for assets
if(CMAKE_BUILD_TYPE STREQUAL "Release")
  # Release build: set path to installed assets
  set(APP_ASSETS_PATH
      "${CMAKE_INSTALL_PREFIX}/share/${PROJECT_NAME}/${ASSETS_DIR}"
  )
else()
  # Debug build: copy assets to build directory
  file(COPY ${ASSETS_DIR} DESTINATION ${CMAKE_BINARY_DIR})
  set(APP_ASSETS_PATH ${CMAKE_BINARY_DIR}/${ASSETS_DIR})
endif()

# Convert paths to CMake style paths for cross-platform compatibility
file(TO_CMAKE_PATH "${APP_ASSETS_PATH}" APP_ASSETS_PATH)

# Print the asset path
message(STATUS "Sparrow assets path: ${APP_ASSETS_PATH}")

configure_file(config.h.in config.h)

add_subdirectory(utils)
add_subdirectory(html)
add_subdirectory(css)
add_subdirectory(style)
add_subdirectory(layout)
add_subdirectory(sdl)

install(
  DIRECTORY ${ASSETS_DIR}
  DESTINATION share/${PROJECT_NAME}
  COMPONENT Application)
