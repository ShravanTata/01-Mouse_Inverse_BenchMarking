cmake_minimum_required(VERSION 2.8)

include(cmake_helper_functions/cmake_functions.txt)



add_subproject(timer)
add_subproject(yaml-cpp timer)
add_subproject(setting-manager yaml-cpp)
add_subproject(dynamics setting-manager)


install (DIRECTORY build/include/ DESTINATION include/spinalDynamics)
install (DIRECTORY build/lib/ DESTINATION lib/spinalDynamics)
configure_file( spinalDynamics.pc.in spinalDynamics.pc @ONLY)
install (FILES build/spinalDynamics.pc DESTINATION lib/pkgconfig/)

pkgconfig_check(simbody found_simbody)
pkgconfig_check(opensim found_opensim)


if (
	NOT "${found_simbody}" STREQUAL ""
	AND
	NOT "${found_opensim}" STREQUAL ""
	)
	add_subproject(opensim-modeller dynamics)
else()
	message("-- Simbody and/or OpenSim library not fonud")
	message("--> opensim-modeller will not be built")
endif()
