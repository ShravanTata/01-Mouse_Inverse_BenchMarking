add_definitions(-std=c++11)
include_directories(./)
aux_source_directory(./ SOURCES)
add_executable(${PROJECT_NAME}Demo ${SOURCES})



target_link_libraries (${PROJECT_NAME}Demo LINK_PUBLIC ${PROJECT_NAME})


pkgconfig_add_library(${PROJECT_NAME}Demo simbody)
pkgconfig_add_library(${PROJECT_NAME}Demo opensim)


install_it(${PROJECT_NAME}Demo)

install(DIRECTORY ../models DESTINATION .)
