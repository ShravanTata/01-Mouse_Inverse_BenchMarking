#ifndef __MOTORCHECK_HH__
#define __MOTORCHECK_HH__

#include <string>
#include <math.h>
#include <vector>
#include <map>
#include <fstream>

#include <webots/Robot.hpp>
#include <webots/Motor.hpp>
#include <webots/PositionSensor.hpp>
#include <webots/Display.hpp>

#define TIME_STEP 1

#define inverse 1

using namespace std;

// Mouse Hind Limb Class
class Muscle: public webots::Robot{

private:
	webots::Motor * joints[12];

	webots::PositionSensor * joints_pos[12];
	
	std::ifstream joint_data_file;

	std::ofstream save_data;

    double joint_angles[6];
	
public:
	// Constructor
	Muscle();
	// Destructor
	~Muscle();
        
	// Methods
	void MuscleSimulate();
	int getJointPosition();

};

#endif /* __MOTORCHECK_HH__ */