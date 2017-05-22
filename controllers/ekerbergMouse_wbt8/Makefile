include ../../mf/optimization.mf
include ../../mf/webots8-version.mf
MAIN_FOLDER = mainEkerbergMouse


ifeq ($(Optimization),"0")
        pkgs = 'sml-5.0 spinalDynamics'
else
        pkgs = 'sml-5.0 spinalDynamics liboptimization-2.0'
endif
include ../../mf/webots-common.mf
include ../../mf/webots-controller.mf

CFLAGS += -DWEBOTS8
CFLAGS +=  -Wno-overloaded-virtual
