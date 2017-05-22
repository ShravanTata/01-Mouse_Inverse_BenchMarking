#pragma once
#ifndef __SmlMouse_HH__
#define __SmlMouse_HH__


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

class SmlMouse
{

private :

	// Methods
	void initialiseConstant();
	void initialiseJoints();
	void initialiseMuscles();
	void initialiseSensors();
	void intiailiseDataRead();
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
	SmlMouse();
	~SmlMouse();
	// Variables
	int time_step;
	double dt;
	std::vector<double> Constant;
	std::vector<double> Input;
	std::vector<double> Output;
	std::array<Muscle *,8> muscles; // CHANGE FOR DIFFERENT NUMBER OF MUSCLES : MUSCLES::NUMBER
	std::map<std::string, Sensor *> sensors;
	std::array<Joint *,3>joints;  // CHANGE FOR DIFFERENT NUMBER OF JOINTS : JOINT::NUMBER
	ofstream dataRead;

	const int numJoints = 2;

	double time;
	double getTime();
	// Methods
	void init();
	int step();
};

#endif /* __ROB_HH__ */
