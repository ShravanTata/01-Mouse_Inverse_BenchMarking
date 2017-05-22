#pragma once
#ifndef __SmlHindMouse_HH__
#define __SmlHindMouse_HH__


#include <string>
#include <math.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <map>
#include <array>

#define DEBUG 0

#include <sml/types/types.h>
#include <sml/musculoSkeletalSystem/Joint.hh>
#include <sml/musculoSkeletalSystem/Muscle.hh>
#include <sml/musculoSkeletalSystem/Sensor.hh>

using namespace std;

class SmlHindMouse
{

private :

	// Methods
	void initialiseConstant();
	void initialiseJoints();
	void initialiseMuscles();
	void initialiseSensors();
	void intiailiseLogData();
	void step_MTU_to_TORQUE(double dt);
	void step_JOINT(double dt);
	void step_TORQUE_to_JOINT();
	void step_SENSORS();
	


	// Virtual Methods
    virtual void ConstantInit()=0;
    virtual void InputInit()=0;
    virtual void InputUpdate()=0;
	virtual void initialiseSpinalControl();
  	virtual void step_SPINE_TO_MTU(double dt);


    // Variables
    double one_gain;
    double zero_length_offset;

public :
	// Constructor
	SmlHindMouse();
	~SmlHindMouse();
	// Variables
	int time_step;
	double dt;
	std::vector<double> Constant;
	std::vector<double> Input;
	std::vector<double> Output;
	std::array<Muscle *,16> muscles; // CHANGE FOR DIFFERENT NUMBER OF MUSCLES : MUSCLES::NUMBER
	std::map<std::string, Sensor *> sensors;
	std::array<Joint *,6>joints;  // CHANGE FOR DIFFERENT NUMBER OF JOINTS : JOINT::NUMBER
	ofstream logData;

	const int numJoints = 6;

	double time;
	double getTime();
	// Methods
	void init();
	int step();
};

#endif /* __SmlHindMouse_HH__ */
