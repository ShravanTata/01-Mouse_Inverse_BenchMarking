add_definitions(-std=c++11)
include_directories(./)
aux_source_directory(./ SOURCES)
add_library(timer ${SOURCES})
target_include_directories (${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.hh")

#add_local_library(dynamics)


foreach(header ${headers})
    get_filename_component(header ${header} NAME)
    add_include_dir_header(${header})  
endforeach()

foreach(header ${headers})
    get_filename_component(header ${header} NAME)
    rename_dir_header_include(${header})  
endforeach()


install_it(${PROJECT_NAME})