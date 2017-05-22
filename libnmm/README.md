
LibNMM install guide for Hardware (arm) and for Webots 7.4.3 on Ubuntu 14.02 
----

The LibNMM library can be used to create spinal network model of locomotion. The library in its current form targets human locomotion.
The library should run on any OS but has up to now only been tested on Linux Ubuntu.

## Hardware

###1. Cross compiling for arm.

1. Install toolchains `apt-get install g++-4.9-arm-linux-gnueabihf`
2. Create the environment : `mkdir cross-compile && cd cross-compile && mkdir lib`
3. Create a helper environment variable : `export CROSS_COMPILE_LIBNMM_FOLDER=$(pwd)`
3. Clone the repo

	```
	git clone https://efx@bitbucket.org/efx/libnmm.git
	cd libnmm
	```

3. Set up the cross-compile

	```
	export CC=/usr/bin/arm-linux-gnueabihf-gcc-4.9
	export CXX=/usr/bin/arm-linux-gnueabihf-g++-4.9
	```

4. Compile spinalDynamics

	```
	./compile spinalDynamics-cross
	```

5. Add spinalDynamics library to PKGconfig path

	```
	export PKG_CONFIG_PATH=$CROSS_COMPILE_LIBNMM_FOLDER/lib/pkgconfig/:$PKG_CONFIG_PATH
	```

5. Compile libnmm

	```
	./configure --version=5.0 --prefix=../
	make -j 2 ENABLE_OPTIMIZATION=False

	```

6. Compile `hibsoreflex`

	```
		make -j8
	```

## Simulation


###1. Install Webots version 7.4.3

> If you have already webots 7.4.3 installed on your machine you don't need to follow the steps below just make sure that `/usr/local/webots743` is a correct simlink to the place where webots 7.4.3 is installed.

If you don't have Webots 7.4.3 installed on your machine.

1. download the archive of webots-7.4.3 from http://www.cyberbotics.com/archive/linux
2. extract the folder
3. move it to /usr/local/ and rename the folder to webots743
4. create the simlink for the webots executable in `/usr/local/bin`

	```
	sudo ln -sf /usr/local/webots743/webots /usr/local/bin
    ```
5. get a license file (Webots pro is needed for this implementation of libNMM to work)

you should now be able to launch webots by simply typing `webots` in a terminal



###2. Install libNMM

> **Requirements.**
The installation will be done using git, make and the g++ compilator. The library depends on the boost library and the liboptimization library (although the latter is not required to compile the code as presented in this guide). On ubuntu launch the following command to install the required softwares and library:  
	    `sudo apt-get install g++ git libboost1.55-dev libjpeg62-dev`


1. Clone the repository :


	```
	git clone https://efx@bitbucket.org/efx/libnmm.git
	```

2. Get the submodule.
		run the following command from the libraries root folder (i.e. the `libnmm` folder)

	```
    git submodule update --init --recursive
	```

2.  Compile spinalDynamics

	The new version of the library now depends on the spinalDynamics library. In the long run this library will replace most of the component of libnmm.  New spinalDynamics is shipped with libnmm. You can compile it as follow

	```
		$ ./compile spinalDynamics
	```
It will ask for the password because the compiled file go by default in /usr/local/.

3. Compile the library
	The library is compatible with an optimization toolbox which let you optimize for the weights of the model.

	a. **WITH OPTIMIZATION SUPPORT (for ubuntu 14.04 only)**

	We first need to install the optimization framework. You can install it with

	```
	wget -qO- https://ponyo.epfl.ch/packages/sign.key | sudo apt-key add -  
    sudo add-apt-repository https://ponyo.epfl.ch/packages
    sudo apt-get update
    sudo apt-get install biorob-optimization biorob-optimization-dev
    ```

	with the optimization framework support enabled run from the library's root folder :

	```
	$ ./compile 5
	```

	b. **WITHOUT OPTIMIZATION SUPPORT**


	The library version we will use here is the version 5 of the library, to compile it without the optimization framework support enabled run from the library's root folder :

	```
	$ ./compile 5 DISABLE_OPTIMIZATION
	```


4. Make sure the library is accessible. The library uses pkg-config. Since the compile script install the library locally we need to make sure that pkg-config will find it. You can do so in two steps
	- run this in a terminal
	 `export PKG_CONFIG_PATH=~/.local/usr/lib/pkgconfig:$PKG_CONFIG_PATH`
	pkg-config should now find the library. Launch `pkg-config --cflags sml-5.0`. The output should be :
	`		-I$HOME/.local/usr/include/sml-5.0  `
	- now add this line to your bashrc file (bashrc_profile in MacOsx) to make changes permanent
	`export PKG_CONFIG_PATH=~/.local/usr/lib/pkgconfig:$PKG_CONFIG_PATH`


###3. Install SML : The webots7 implementation of libNMM


1. Clone the repository :

	```
	git clone https://efx@bitbucket.org/efx/sml.git
	```

2. Config
	- disable optimization
open the file `./controller_current/webots/mf/optimization.mf` and make sur that the Optimization variable is set to "0".

3. Compile the controller
	go in the `./controller_current/webots/controllers/version52_wbt7` folder and run :

	```
	make -j
	```
4. Compile the physics plugin
    - make sur the following package are installed `libgl1-mesa-dev libglu1-mesa-dev`
	- go in the `./controller_current/webots/plugins/physics/no_lift` and run :


	```
	make -j
	```

4. Launch the simulation.
The simulation can be launched with the helper script located in `./tools/config_tools/` and called `webots_loadlib`. The script takes as argument the webots world te be used.

	```
	$ ./tools/config_tools/webots_loadlib controller_current/webots/worlds/743.wbt
	```

	Depending on your system configuration you might be able to run directly the software like this

	```
	$ /usr/local/webots743/webots ../../current/webots/worlds/version52.wbt
	```

###4. Control the behavior of the simulation

A bunch of config files are used by the simulation. The config files are located in the `../../current/webots/conf_NAME` folder where **NAME** is the config prefix used. By default the config prefix used here is `articleFrontier_wavy2`.

When a simulation starts, the first config file being loaded is the settings file. By default the file `../../current/webots/conf_NAME/settings/settings.xml` is used. You can specify an other settings filename with the `-s` parameter. For example if you want to load the file `../../current/webots/conf_NAME/settings/settings2.xml` you can do it as follow :

```
$ ./webots_loadlib -s settings2.xml PATH_TO_WORLD_FILE
```


###5. Use the optimization framework.


1. Install the biorob optimization toolbox.

    - Follow the instruction at http://biorob.epfl.ch/page-36418-en.html
       ( section **Ubuntu Software Repository )
    - Install the biorob-optimization software suits

	sudo apt-get install biorob-optimization biorob-optimization-dev

	> You can find Information related to the optimization framework can be found on the same web page.

2. Make sure you compiled both `libnmm` and `sml` with the optimization flag.

	> In the sml directory open the file `./current/webots/mf/optimization.mf` and make sur that the Optimization variable is set to "1".


3. Launch an optimization

	1. Go to the `./data/optimization/tutorial` folder

	    In this folder you will see two xml file: a  `exp_PSO.xml` and `exp_stagePSO.xml`. The first file will run a standard PSO the fitness expression is defined in the XML file `fitness`  section, where you can use different parameters returned by the optimization. All the logged parameters can be found in the `libnmm/sml/sml-tools/EventManager.cc` more precisely the `EventManager::getMap()` function.

		The second file is an optimization that uses stage PSO to optimize first for distance then for speed and finally for energy (this is what has been used to produce the results showed in the Frontiers2014 article). You can control the number of iteration and the number of particles as well as all other optimization related parameters.


	2. Launch the optimaster and optiworker in two different terminal

	3. Launch the actual optimization

	     To launch the optimization simply do (from the tutorial folder)

		optirunner ./exp.xml

	> If you get an error complaining that the sml-5.0 library does not exist you shold
	> Open the /usr/bin/webots scripts and change the line 8 from

	> `WEBOTS_LIB_DIR="$webotsBinaryDir/lib"`
	> to
	> `WEBOTS_LIB_DIR="$webotsBinaryDir/lib:/home/{USERNAME}/.local/usr/lib"`
	> Replace {USERNAME} by your unix username.


5. Monitoring the optimization.

	The optimization will log different variable related to the optimisation in the results database. The parameters logged have to be defined in the

6. Extracting solutions

	- With the optiextractor

		after you finished the optimization a database is created.
		You can view the results using the `optiextractor` command.

	- With sml python scripts

		you can also use the `extract.py` script to extract the best solution. The script is located in  `./tools/job_tools/extract.py`. Run it with the `--help` to see what you can do. An example usage is as follow :

		`./extract2.py parameters -s -f 1.3 pso_1.3-IP.1.db`

		This will extract the best parameters of a stage pso experiment ( `-s` option) save it in a file called `1.3` from database `pso_1.3-IP.1.db`. You can then make your model use this parameter file by:
		 -  placing it in the config folder used by your controller and
		 -  editing the settings file used by your controller to use this setting file.

		For instance if you open the 743.wbt world file and look for controller you will see this :

		```
		controller "version52_wbt7"
		controllerArgs "settings_jw_notf.xml sample"
		```

		In this example our controller uses the `settings_jw_notfl.xml` file located in the folder `./controller_current/webots/conf_sample/settings` folder.  This folder contains several folder. Here we are interested in two of them. The settings folder which contains a set of xml files (and especially the one you are interested in here which is in our case **settings_jw_notf.xml** and the **fullReflex_gate** folder.

		You want to use our newly optimized reflex parameters. We can do this by moving our file in the fullReflex_gate folder. We then open the xml settings file and locate the **experiment** xml tag. It should contains something like this :

		      fullReflex{p_file=param3.txt,start_after_step=300}

		Now the p_file is the name of your file (**in this case 1.3 WITHOUT the .txt**). the start_after_step parameter says when your model will switch from launching gait parameter to those one.




(DEPRECATED) LibNMM install guide for Webots 6.4.4
----

The LibNMM library can be used to create spinal network model of locomotion. The library in its current form targets human locomotion.
The library should run on any OS but has up to now only been tested on Linux Ubuntu.



###1. Install Webots version 6.4.4

> If you have already webots 6.4.4 installed on your machine you don't need to follow the steps below just make sure that `/usr/local/webots644` is a correct simlink to the place where webots 6.4.4 is installed.

If you don't have Webots 6.4.4 installed on your machine.

1. download the archive of webots-6.4.4 from http://www.cyberbotics.com/archive/linux
2. extract the folder
3. move it to /usr/local/ and rename the folder to webots644
4. create the simlink for the webots executable in `/usr/local/bin`

	```
	sudo ln -sf /usr/local/webots644/webots /usr/local/bin
    ```
5. get a license file (Webots pro is needed for this implementation of libNMM to work)

you should now be able to launch webots by simply typing `webots` in a terminal



###2. Install libNMM

> **Requirements.**
The installation will be done using git, make and the g++ compilator. The library depends on the boost library and the liboptimization library (although the latter is not required to compile the code as presented in this guide). On ubuntu launch the following command to install the required softwares and library:  
	    `sudo apt-get install g++ git libboost1.55-dev libjpeg62-dev cmake`


1. Clone the repository :


	```
	git clone https://efx@bitbucket.org/efx/libnmm.git
	```

2. Get the submodule.

	3. Check that you have the correct submodule address.

		- Enter in the cloned repository and open the file `./.git/config`
		- Check that the url of the sml/sml-tools submodule is correct. It should read

		```
		[submodule "sml/sml-tools"]
        url = https://efx@bitbucket.org/efx/libnmm-sml-tools.git
        ```

	2. Get the submodule

		run the following command from the libraries root folder (i.e. the `libnmm` folder)

		```
		git pull && git submodule init && git submodule update && git submodule status
		```
3.  Compile the library

	The library can be compiled using the compile script.
	The library version we will use here is the version 5 of the library, to compile it without the optimization framework support enabled run from the library's root folder :

	```
		$ ./compile 5 DISABLE_OPTIMIZATION
	```
4. Make sure the library is accessible. The library uses pkg-config. Since the compile script install the library locally we need to make sure that pkg-config will find it. You can do so in two steps
	- run this in a terminal `export PKG_CONFIG_PATH=~/.local/usr/lib/pkgconfig:$PKG_CONFIG_PATH`
	pkg-config should now find the library. Launch `pkg-config --cflags sml-5.0`. The output should be : `		-I$HOME/.local/usr/include/sml-5.0  `
	- add `export PKG_CONFIG_PATH=~/.local/usr/lib/pkgconfig:$PKG_CONFIG_PATH` to your  `~/.bashrc` file.


###3. Install SML : The webots6 implementation of libNMM


1. Clone the repository :

	```
	git clone https://efx@bitbucket.org/efx/sml.git
	```

2. Config
	- disable optimization
open the file `./current/webots/mf/optimization.mf` and make sur that the Optimization variable is set to "0".

3. Compile the controller
	go in the `./current/webots/controllers/version52` folder and run :

	```
	make
	```
4. Compile the physics plugin
    - make sur the following package are installed `libgl1-mesa-dev libglu1-mesa-dev`
	- go in the `./current/webots/plugins/physics/no_lift` and run :


	```
	make
	```

4. Launch the simulation.
The simulation can be launched with the helper script located in `./tools/config_tools/` and called `webots_loadlib`. The script takes as argument the webots world te be used.

	```
	$ ./webots_loadlib ../../current/webots/worlds/version52.wbt
	```

###4. Control the behavior of the simulation

A bunch of config files are used by the simulation. The config files are located in the `../../current/webots/conf_NAME` folder where **NAME** is the config prefix used. By default the config prefix used here is `articleFrontier_wavy2`.

When a simulation starts, the first config file being loaded is the settings file. By default the file `../../current/webots/conf_NAME/settings/settings.xml` is used. You can specify an other settings filename with the `-s` parameter. For example if you want to load the file `../../current/webots/conf_NAME/settings/settings2.xml` you can do it as follow :

```
$ ./webots_loadlib -s settings2.xml PATH_TO_WORLD_FILE
```


###5. Enable the optimization framework.


1. Install the biorob optimization toolbox.

    - Follow the instruction at http://biorob.epfl.ch/page-36418-en.html
       ( section **Ubuntu Software Repository )
    - Install the biorob-optimization software suits

	sudo apt-get install biorob-optimization biorob-optimization-dev

	> You can find Information related to the optimization framework can be found on the same web page.

2. Enable optimization

In the sml directory open the file `./current/webots/mf/optimization.mf` and make sur that the Optimization variable is set to "1".

3. Recompile the ``libnmm'' and the ``sml'' implementation


4. Launch an optimization

	1. Go to the `./data/optimization/tutorial` folder

	    In this folder you will see two xml file: a ``exp_PSO.xml'' and ``exp_stagePSO.xml''. The first file will run a standard PSO the fitness expression is defined in the xml file ``fitness''  section, where you can use different parameters returned by the optimsation. All the logged parameters can be found in the ``libnmm/sml/sml-tools/EventManager.cc'' ``EventManager::getMap()'' function. This is a sample file of an optimization that uses stage PSO to optimize first for distance then for speed and finally for energy. You can control the number of iteration and the number of particles as well as all other optimization related parameters.


	2. Launch the optimaster and optiworker in two different terminal

	3. Launch the actual optimization

	     To launch the optimization simply do (from the tutorial folder)

		optirunner ./exp.xml

	> If you get an error complaining that the sml-5.0 library does not exist you shold
	> Open the /usr/bin/webots scripts and change the line 8 from

	> `WEBOTS_LIB_DIR="$webotsBinaryDir/lib"`
	> to
	> `WEBOTS_LIB_DIR="$webotsBinaryDir/lib:/home/{USERNAME}/.local/usr/lib"`
	> Replace {USERNAME} by your unix username.


5. Monitoring the optimization.

	The optimization will log different variable related to the optimisation in the results database. The parameters logged have to be defined in the

6. Extracting solutions

	- With the optiextractor


	- With sml python scripts

