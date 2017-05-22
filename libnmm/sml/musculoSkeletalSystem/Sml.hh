#pragma once
#ifndef __Sml_HH__
#define __Sml_HH__


#include <string>
#include <math.h>
#include <vector>
#include <fstream>
#include <map>
#include <array>

//#include <boost/algorithm/string.hpp>


#include <sml/types/types.h>
#include <sml/musculoSkeletalSystem/Interneuron.hh>
#include <sml/musculoSkeletalSystem/Motoneuron.hh>
#include <sml/musculoSkeletalSystem/Entity.hh>
#include <sml/musculoSkeletalSystem/Connection.hh>
#include <sml/musculoSkeletalSystem/Joint.hh>
#include <sml/musculoSkeletalSystem/Muscle.hh>
#include <sml/musculoSkeletalSystem/Sensor.hh>

#include <sml/sml-tools/EventManager.hh>
#include <sml/sml-tools/PerturbationManager.hh>
#include <sml/sml-tools/OnlineCtrl.hh>
#include <sml/neuralSystem/ReflexController.hh>


#define DEBUG 0


class Sml
{
private:
	bool flag;
	bool fixedViewPoint;
    void initialiseOnlineCtrl();
    void initialisePerturbation();
	void initialiseConstant();
	void initialiseJoints();
	void initialiseSensors();
	void initialiseMuscles();


    
    virtual void ConstantInit()=0;
    virtual void InputInit()=0;
    virtual void InputUpdate()=0;
    
    double vdisttocm[2];
    double hdisttocm[2];

	void step_JOINT(double dt);
	void step_MTU_to_TORQUE(double dt);
	void step_TORQUE_to_JOINT();
	void step_SENSORS();
	void step_PERTURBATION();

	double phi_knee_off;
	double soft_limit_stiffness;
	double theta_trunk_ref;
	double zero_length_offset;
	double one_gain;

	virtual void initialiseSpinalControl();
	virtual void step_SPINE_TO_MTU(double dt);

protected:
	SIDE::Side finishing_stance;

public:
    std::vector<double> Constant;
    std::vector<double> Input;
    std::vector<double> Output;

	double dt;
	int time_step;
	int step();

	std::array<Muscle *, MUSCLES::NUMBER> muscles;
	std::map<std::string, Sensor *> sensors;
	std::array<Joint *, JOINT::NUMBER> joints;
	Foot * left_foot;
	Foot * right_foot;	
	Trunk * trunk;

	PerturbatorManager perturbator;
	
	Sml();
	//destructor
	//~Sml();

    void init();
	
	// void setHDistToCM(SIDE::Side side);
	// void setHDistToCM();
	// void setVDistToCM(SIDE::Side side);
	// void setVDistToCM();
 	// double getHDistToCM(SIDE::Side side);
private:
    double getTime();
};

#endif /* __ROB_HH__ */
