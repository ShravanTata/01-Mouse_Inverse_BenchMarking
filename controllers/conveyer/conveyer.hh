#ifndef __CONVEYER_HH__
#define __CONVEYER_HH__

#include <string>
#include <math.h>
#include <vector>
#include <map>
#include <fstream>

#include <webots/Robot.hpp>
#include <webots/Motor.hpp>
#include <webots/PositionSensor.hpp>

#define TIME_STEP 1

#define inverse 1

using namespace std;

// Mouse Hind Limb Class
class Conveyer: public webots::Robot{

private:
	
	webots::Motor * conveyer[2];
	
public:
	// Constructor
	Conveyer();
	// Destructor
	~Conveyer();
	
	// Methods
  	void runSync();
    
};

#endif /* __CONVEYER_HH__ */