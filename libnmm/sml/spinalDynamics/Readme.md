
# Project usage guideline

1. The presentation used in the project : [Presentation](https://gitlab.com/srill-fb99/spinaldynamics/blob/master/doc/Presentation.pdf)
2. The class diagrams :
	[Diagram](https://gitlab.com/srill-fb99/spinaldynamics/blob/master/doc/class_diagram.pdf)


## Dependencies

### Catch test suit
The project use the catchTestSuit. You can found it here :  https://github.com/philsquared/Catch.git

### OpenSim & Simbody
In order to use all the developped code you will need to install `simbody` and `opensim`.
You can follow the tutorial [HowTo/OpenSim](https://gitlab.com/srill-fb99/spinaldynamics/blob/master/doc/HowToOpenSim.md) guide (for linux).

[Dependencies](https://gitlab.com/srill-fb99/spinaldynamics/blob/master/doc/dependencies.png)



## Installation
### Automatic installation

See [install.txt](https://gitlab.com/srill-fb99/spinaldynamics/blob/master/install.txt)


## Usage

### For all libraries
The _dynamics_ and _setting-manager_ library comes with a test suits that provides most of the use case for the different library.
The tests are located in the `build/bin` folder.

Note that the test executable provides many useful method to run only some tests are to control the output (see `--help` for more information).

### 1. OpenSim-modeller

A test case can be launched from the bin folder as follow :

```bash
./src/opensim-model/demo/opensim-modelDemo ../src/opensim-model/models/10dof_18musc_config.yaml
```

> The demo code make use of the Parameter & Settings manager library to set the parameter associated to a model. 
For example : `../src/opensim-model/models/10dof_18musc_config.yaml` contains 

```yaml
# All boolean parameters
- parameters : bool
  visualize : true # Turns visualization on/off
  save_simulation_results : true # Save the simulation results
  save_model : false # Save the model
  load_from_file : true # Loads the model from file
  load_parameters : true # Load extra parameters 
  contact : false # Enable contact
  coordinate : true # Enable coordinate visualization
  controller : true # Enable controller
  controller/hasDuration : true # Controller provides duration ?
# All string parameters
- parameters : string
  save_simulation_results/filename : /tmp/simulation_results_10dof_18musc.sto
  save_model/filename : 10dof_18musc.osim
  load_from_file/filename : ../src/opensim-model/models/10dof_18musc.osim
  load_parameters/filename : ../src/opensim-model/models/10dof_18musc_param.txt
  controller/type : TwitchingController
# A table parameters. Here for contact geometry
- parameters : table
  headers_type : string string string
  headers_name : contact/geom_name contact/mesh_file contact/body_name
  content : foot1 foot_rectangle.obj calcn_l
            foot2 foot_rectangle.obj calcn_r
# A table parameters. For settings coordinate states (if it should be locked or not)
- parameters : table
  headers_type : string bool
  headers_name : coordinate/name coordinate/locked
  # Fixed to the world
  ####################
  content : pelvis_tilt true
            pelvis_tx true
            pelvis_ty true
            lumbar_extension true
  # In the ground
  ###############
  # content : pelvis_tilt false
  #           pelvis_tx false
  #           pelvis_ty false
  #           lumbar_extension true
  # In the ground fully locked
  ############################
  # content :  pelvis_tilt true
  #            pelvis_tx true
  #            pelvis_ty true
  #            hip_flexion_r true
  #            knee_angle_r true
  #            ankle_angle_r true
  #            hip_flexion_l true
  #            knee_angle_l true
  #            ankle_angle_l true
  #            lumbar_extension true
# A parameters table to set the muscle to be controlled
- parameters : table
  headers_type : string
  headers_name : controller/muscles_name
  content : hamstrings_r
            bifemsh_r
            glut_max_r
            iliopsoas_r
            rect_fem_r
            vasti_r
            gastroc_r
            soleus_r
            tib_ant_r
            hamstrings_l
            bifemsh_l
            glut_max_l
            iliopsoas_l
            rect_fem_l
            vasti_l
            gastroc_l
            soleus_l
            tib_ant_l
```
