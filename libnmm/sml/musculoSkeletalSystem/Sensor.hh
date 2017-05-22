#pragma once
#ifndef __SENSOR_HH__
#define __SENSOR_HH__

/*!
 * \file Sensor.hh
 * \brief Sensor description (touch sensor / muscle length / muscle force / foot / ...)
 * \author efx
 * \version 0.1
 */

#include <vector>
#include <string>
#include <math.h>
#include <map>
#include <sml/sml-tools/RungeKutta.hh>

class Sml;
class Muscle;
class Joint;

class Sensor {
public:
	double value;
    void step(){
    	value = update();
    };
    const double& get(){
    	return value;
    }
    double tf(double);
private:
	virtual double update() = 0;
};


/**
 * 
 * Trunk class, angle are with respect to the vertical axes (the one // to gravity)
 * 
 */

/* Trunk class. 
 *
 * Model of the Trunk
 * INPUT :
 *  -> current_angle (rad)
 *
 * The default implementation does nothing.
 */
class Trunk{
public:
	Sml * sml;
    INPUT::Inputs input_id;
	double theta;
	double& theta_ref;
	double& theta_offset;
	double d_theta;
	double theta_to;
	//Trunk():theta(0.0),theta_ref(0.0),d_theta(0.0),theta_to(0.0){}
	Trunk(Sml *sml,
        INPUT::Inputs input_id,
        double& theta_trunk_ref, 
        double& theta_offset);
    double getAngle();
	void step(double dt);
	void setThetaRef(double t);
	double getThetaRef();
	void addThetaRef(double t);
	
};


/**
 * 
 * TouchSensor class
 * 
 * 		Ground sensors (left teel, left ankle, right teel, right ankle) are Biosensors
 * 
 * 
 */
class TouchSensor : public Sensor, public Derivatives {
public:
    Sml *sml;
    INPUT::Inputs input_id;
	double lastValue;
	double dt;
	TouchSensor(Sml * sml, INPUT::Inputs input_id, double dt): sml(sml),input_id(input_id),lastValue(10.0),dt(dt){}
	
    double getInput();
	std::vector<double> dydt(double x, std::vector<double> y, std::vector<double> param, std::vector<double> input);
private:
	double update();
};


/**
 * 
 * MuscleLengthSensor class
 * 
 * 
 */
class MuscleLengthSensor: public Sensor{
public:
	Muscle * muscle;
	double& offset;
	double& gain;
	MuscleLengthSensor(Muscle * muscle, double &offset, double &gain):muscle(muscle),offset(offset),gain(gain){}
private:
	double update();
};

/**
 * 
 * MuscleForceSensor class
 * 
 * 
 */
class MuscleForceSensor: public Sensor{
	Muscle * muscle;
	double& gain;
public:
	MuscleForceSensor(Muscle * muscle, double &gain):muscle(muscle), gain(gain){}
private:
	double update();
};


/**
 * 
 * JointPositionSensor class
 * 
 * 
 */
class JointPositionSensor: public Sensor{
	Joint * joint;
	double& kp;
	double& kd;
	double& ref;
public:
	JointPositionSensor(Joint * joint, double& kp, double& kd, double & ref):joint(joint),kp(kp),kd(kd), ref(ref){}
private:
	double update();
};

/**
 * 
 * TrunkPositionSensor class
 * 
 * 
 */
/*
class TrunkPositionSensor: public Sensor{
	Trunk * trunk;
	double& kp;
	double& kd;
	double& ref;
public:
	TrunkPositionSensor(Trunk * trunk, double& kp, double& kd, double & ref):trunk(trunk),kp(kp),kd(kd), ref(ref){}
	double get(){
		return get(0.0);
	}
	double get(double o){
		return kp * (this->trunk->theta - (ref+o) ) + kd * trunk->d_theta;
	}
};
*/
class TrunkPositionSensor1: public Sensor{
	Trunk * trunk;
	double& kp;
	double& kd;
public:
	TrunkPositionSensor1(Trunk * trunk, double& kp, double& kd):trunk(trunk),kp(kp),kd(kd){}
private:
	double update();
};

class TrunkPositionSensor2: public Sensor{
	Trunk * trunk;
	double& kp;
	double& kd;
public:
	TrunkPositionSensor2(Trunk * trunk, double& kp, double& kd):trunk(trunk),kp(kp),kd(kd){}
private:
	double update();
};
/**
 * 
 * TrunkTakeOffSensor class
 * 
 * 
 */
class TrunkTakeOffSensor: public Sensor{
	Trunk * trunk;
	double& ref;
public:
	TrunkTakeOffSensor(Trunk * trunk, double &ref): trunk(trunk),ref(ref){}
private:
	double update();
};

class TrunkTakeOffSensorOld: public Sensor{
	Trunk * trunk;
public:
	TrunkTakeOffSensorOld(Trunk * trunk): trunk(trunk){}
private:
	double update();
};
/**
 * 
 * AngleSensor class for OverExtensionReflex
 * 
 * 
 */
class AngleSensor: public Sensor{
	Joint * joint;
	double& offset;
public:
	AngleSensor(Joint * joint, double& offset): joint(joint), offset(offset){}
private:
	double update();
};
/**
 * 
 * Foot class (special sensors that have to TouchSensor to feel the ground 
 * and also a limb_state variable that says if the foot thinks is in stance or swing
 * 
 *   This is a special sensor used by the Gate that turns the feedbacks on/off 
 *   regarding the limb_state on which they need to be active.
 *   
 */

class Foot: public Sensor
{
public:
	double scalingfactor;
	CYCLE_PHASE::CyclePhase limb_state;
	CYCLE_PHASE::CyclePhase lastlimb_state;
	double delay;
	TouchSensor * heel;
	TouchSensor * toe;
	double last_time_limb_state_changed;
	double last_time_foot_touch;
	double duration_last_step;
	double distance_last_step;
	double last_pos_foot_touch;
	double &off;
	SIDE::Side side;
	Foot(TouchSensor * heel, TouchSensor * toe, SIDE::Side side,double &off):  scalingfactor(784.8),limb_state(CYCLE_PHASE::STANCE), lastlimb_state(CYCLE_PHASE::STANCE), delay(0.05), heel(heel), toe(toe),last_time_limb_state_changed(-1.0),last_time_foot_touch(0.5),duration_last_step(0.5),distance_last_step(0.0), last_pos_foot_touch(0.0),off(off),side(side){}
	Foot(TouchSensor * heel, TouchSensor * toe, double time, SIDE::Side side, double &off):  scalingfactor(784.8),limb_state(CYCLE_PHASE::STANCE), lastlimb_state(CYCLE_PHASE::STANCE), delay(0.05), heel(heel), toe(toe), last_time_limb_state_changed(time),off(off),side(side){}
	std::string getName();
	bool inStance();
	bool justTouchTheGround();
	bool justGetOffGround();
	double getFeltForce();
	double getRealFeltForce();
	double getXForce();
	double getYForce();
	//double getZForce();
	CYCLE_PHASE::CyclePhase getLastState();
	CYCLE_PHASE::CyclePhase getState();
	void setState(CYCLE_PHASE::CyclePhase str);
	void updateState(double actual_time);
    bool touchedBefore(Foot * cfoot);
    bool changeStateAt();
private:
	double update();
};


#endif /* __SENSOR_HH__ */

