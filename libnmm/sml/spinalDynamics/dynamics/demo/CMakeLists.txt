add_definitions(-std=c++11)
include_directories(./)
aux_source_directory(./ SOURCES)
add_executable(${PROJECT_NAME}Demo ${SOURCES})
target_link_libraries (${PROJECT_NAME}Demo LINK_PUBLIC ${PROJECT_NAME})

