cmake_minimum_required(VERSION 2.6)
project(timer)

include("${CMAKE_SOURCE_DIR}/cmake_helper_functions/cmake_functions.txt")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})


add_subdirectory (lib)
add_subdirectory (demo)
add_subdirectory (test)


target_include_directories (${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/include/")
create_include_dir()

