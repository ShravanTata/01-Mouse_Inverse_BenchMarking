## Installation

### Get the sources
Both symbody and opensim-core have open read access to their repository. 
We can thus just clone them up in our working folder.

	git clone https://github.com/simbody/simbody.git
	git clone https://github.com/opensim-org/opensim-core.git 

### Compilation
Both software can be compiled using cmake. We can therefore use helpers such as ccmake to set the compilation options. 

#### Simbody 

* Dependencies

```bash
        sudo apt-get install liblapack-dev libblas-dev doxygen freeglut3-dev
```

* Important Compilation options
```cpp
		BUILD_EXAMPLES ON
		BUILD_TESTING OFF
		CMAKE_BUILD_TYPE Release
		CMAKE_INSTALL_PREFIX /opt/Simbody
		SIMBODY_STANDARD_11 ON
```
* Compilation
Assuming we are in the simbody repository
```bash
		$ mkdir build
		$ cd build
		$ ./cmake
		$ ./ccmake #(to set the compilation option)
		$ make -j8 && sudo make install
```
* Post installation 
To make the usage of the library possible we should make sure __pkg-config__ and __ldconfig__ find the library.
	1. pkg-config makes use of .pc file to locate libraries. The compilation should have created one and the installation put it in /opt/Simbody/lib/*/pkgconfig/simbody.pc. The file should look like this :
	
				prefix=/usr/local/Simbody
				libdir=${prefix}/lib/x86_64-linux-gnu
				includedir=${prefix}/include
				
				Name: simbody
				Description: Simbody Libraries
				Version: 3.5
				Requires:
				Cflags: -I${includedir}/simbody -I${includedir}
				Libs: -L${libdir} -lSimTKsimbody -lSimTKmath -lSimTKcommon -llapack -lblas -lpthread -lrt -ldl -lm

		Make sure that the before last line starts with a __Cflags__ and not __CFlags__. Now we should make pkg-config find our .pc file. This can be done by adding the folder where the the simbody.pc file is located to the PKG_CONFIG_PATH environment variable. You can put a line like this one in your ~/.bashrc file :

			export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/path/to/folder/containing/simbody.pc
			
		> Make sur to replace __/path/to/folder/containing/simbody.pc__ with the correct path.
				
		Run the last command in the terminal to update the PKG_CONFIG_PATH environment variable in your current shell. You should see something like this when searching for simbody with pkg-config : 

			$ pkg-config --cflags --libs simbody 
			 -L/opt/Simbody/lib/x86_64-linux-gnu -lSimTKsimbody -lSimTKmath -lSimTKcommon -llapack -lblas -lpthread -lrt -ldl -lm
	 
	2. ld-config looks for library using configuration file present in the /etc/ld.so.conf.d folder. Create a new file in this folder called simbody.conf that should contain the path to the libraries. On my computer the content of the file is : 

			/opt/Simbody/lib/x86_64-linux-gnu/

		Now we should update the ld cache by runnnig _sudo ldconfig_. If everything went well the following command should show something : 

			ldconfig -p | grep 'simbody' 

* Test code
We will test our installation of simbody by trying to compile the following example (taken from the readme.md of simbody repository).
   * Create a folder and create a file named test.cc with the following content

	```cpp
	#include "Simbody.h"
using namespace SimTK;
int main() {
    // Define the system.
    MultibodySystem system;
    SimbodyMatterSubsystem matter(system);
    GeneralForceSubsystem forces(system);
    Force::Gravity gravity(forces, matter, -YAxis, 9.8);

    // Describe mass and visualization properties for a generic body.
    Body::Rigid bodyInfo(MassProperties(1.0, Vec3(0), UnitInertia(1)));
    bodyInfo.addDecoration(Transform(), DecorativeSphere(0.1));

    // Create the moving (mobilized) bodies of the pendulum.
    MobilizedBody::Pin pendulum1(matter.Ground(), Transform(Vec3(0)),
            bodyInfo, Transform(Vec3(0, 1, 0)));
    MobilizedBody::Pin pendulum2(pendulum1, Transform(Vec3(0)),
            bodyInfo, Transform(Vec3(0, 1, 0)));

    // Set up visualization.
    Visualizer viz(system);
    system.addEventReporter(new Visualizer::Reporter(viz, 0.01));

    // Initialize the system and state.
    State state = system.realizeTopology();
    pendulum2.setRate(state, 5.0);

    // Simulate for 20 seconds.
    RungeKuttaMersonIntegrator integ(system);
    TimeStepper ts(system, integ);
    ts.initialize(state);
    ts.stepTo(20.0);
}
	```

	* Assuming that all the previous step were done you should be able to compile the code as follow :

			clang++ test.cc -std=c++11 `pkg-config --cflags --libs simbody`

		Here I use clang++ but you could replace that with g++. If the compilation went well you should be able to run the compiled program with ./a.out. You should see something like this : 
>In case you get an error about the Visualizer it simply means that the OS can not find the simbody-visualizer executable. If you installed in /optSimbody the visualizer should be located in /opt/Simbody/libexec/. 

	![Double pendulum](https://github.com/simbody/simbody/raw/master/doc/images/doublePendulum.gif)


#### OpenSim

* Important Compilation options
```cpp
		BUILD_TESTING OFF
		BUILD_EXAMPLES ON
		SIMBODY_HOME /opt/Opensim
		CMAKE_BUILD_TYPE Release
		CMAKE_INSTALL_PREFIX /opt/OpenSim
```
* Compilation : similar to __Simbody__ see above.

  > If you get an error saying that some simbody headers can not be found make sure you set the SIMBODY_HOME compilation option to the right folder. If you follow this guide this should be /opt/Simbody


* Post installation 
Similarly to what we did for __Simbody__ we need to make sure __pkg-config__ and __ldconfig__ find the library.
	1. pkg-config makes use of .pc file to locate libraries. OpenSim doesn't come with a pc file. Lets create a file called opensim.pc in /opt/OpenSim/lib/pkgconfig. This file should contain : 
	
				prefix=/path/to/Opensim
				libdir=${prefix}/lib
				includedir=${prefix}/sdk/include
				
				Name: opensim
				Description: OpenSim Libraries
				Version: 3.5
				Requires:
				Cflags: -I${includedir}
				Libs: -L${libdir} -lSimTKcommon -lSimTKmath -lSimTKsimbody -losimCommon -losimAnalyses -losimActuators -losimLepton -losimSimulation -losimTools

		Now we should make pkg-config find our .pc file. This can be done by adding the folder where the the simbody.pc file is located to the PKG_CONFIG_PATH environment variable. You can put a line like this one in your ~/.bashrc file :

			export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/path/to/folder/containing/opensim.pc
			
		> Make sur to replace __/path/to/folder/containing/opensim.pc__ with the correct path.
		
		Run the last command in the terminal to update the PKG_CONFIG_PATH environment variable in your current shell. You should see something like this when searching for simbody with pkg-config : 

			$ pkg-config --cflags --libs opensim
			-I/opt/OpenSim/sdk/include  -L/opt/OpenSim/lib -lSimTKcommon -lSimTKmath -lSimTKsimbody -losimCommon -losimAnalyses -losimActuators -losimLepton -losimSimulation -losimTools 
	 
	2. ld-config looks for library using configuration file present in the /etc/ld.so.conf.d folder. Create a new file in this folder called opensim.conf that should contain the path to the libraries. On my computer the content of the file is : 

			/opt/OpenSim/lib

		Now we should update the ld cache by runnnig _sudo ldconfig_. If everything went well the following command should show something : 

			ldconfig -p | grep 'osim' 


* Test code
We will test our installation of simbody by trying to compile the following example (taken from the readme.md of simbody repository).
   * Create a folder and create a file named test.cc with the following content

		```cpp
		#include <OpenSim/OpenSim.h>
		using namespace SimTK;
		using namespace OpenSim; using OpenSim::Body;
		int main() {
		    Model model; model.setUseVisualizer(true);
		    // Two links, with mass of 1 kg, center of mass at the
		    // origin of the body's frame, and moments/products of inertia of zero.
		    OpenSim::Body* link1 = new OpenSim::Body("humerus", 1, Vec3(0), Inertia(0));
		    OpenSim::Body* link2 = new OpenSim::Body("radius", 1, Vec3(0), Inertia(0));
		
		    // Joints that connect the bodies together.
		    PinJoint* joint1 = new PinJoint("shoulder",
		            // Parent body, location in parent, orientation in parent.
		            model.getGroundBody(), Vec3(0), Vec3(0),
		            // Child body, location in child, orientation in child.
		            *link1, Vec3(0, 1, 0), Vec3(0));
		    PinJoint* joint2 = new PinJoint("elbow",
		            *link1, Vec3(0), Vec3(0), *link2, Vec3(0, 1, 0), Vec3(0));
		
		    // Add an actuator that crosses the elbow, and a joint stop.
		    Millard2012EquilibriumMuscle* muscle = new
		        Millard2012EquilibriumMuscle("biceps", 200, 0.6, 0.55, 0);
		    muscle->addNewPathPoint("point1", *link1, Vec3(0, 0.8, 0));
		    muscle->addNewPathPoint("point2", *link2, Vec3(0, 0.7, 0));
		
		    // A controller that specifies the excitation of the biceps muscle.
		    PrescribedController* brain = new PrescribedController();
		    brain->addActuator(*muscle);
		    // Muscle excitation is 0.3 for the first 0.5 seconds, and 1.0 thereafter.
		    brain->prescribeControlForActuator("biceps",
		            new StepFunction(0.5, 3, 0.3, 1));
		
		    // Add bodies and joints to the model.
		    model.addBody(link1); model.addBody(link2);
		    model.addJoint(joint1); model.addJoint(joint2);
		    model.addForce(muscle);
		    model.addController(brain);
		
		    // Configure the model.
		    State& state = model.initSystem();
		    // Fix shoulder joint, flex elbow joint.
		    model.updCoordinateSet()[0].setLocked(state, true);
		    model.updCoordinateSet()[1].setValue(state, 0.5 * Pi);
		    model.equilibrateMuscles(state);
		
		    // Add display geometry.
		    model.updMatterSubsystem().setShowDefaultGeometry(true);
		    Visualizer& viz = model.updVisualizer().updSimbodyVisualizer();
		    viz.setBackgroundColor(Vec3(1, 1, 1));
		    // Ellipsoids: 0.5 m radius along y axis, centered 0.5 m up along y axis.
		    DecorativeEllipsoid geom(Vec3(0.1, 0.5, 0.1)); Vec3 center(0, 0.5, 0);
		    viz.addDecoration(link1->getMobilizedBodyIndex(), Transform(center), geom);
		    viz.addDecoration(link2->getMobilizedBodyIndex(), Transform(center), geom);
		
		    // Simulate.
		    RungeKuttaMersonIntegrator integrator(model.getSystem());
		    Manager manager(model, integrator);
		    manager.setInitialTime(0); manager.setFinalTime(10.0);
		    manager.integrate(state);
		};
	```
	
	* Assuming that all the previous step were done you should be able to compile the code as follow :

			clang++ test.cc -std=c++11 `pkg-config --cflags --libs simbody opensim`

		Here I use clang++ but g++ could be used. If the compilation went well you should be able to run the compiled program with ./a.out. You should see something like this : 


