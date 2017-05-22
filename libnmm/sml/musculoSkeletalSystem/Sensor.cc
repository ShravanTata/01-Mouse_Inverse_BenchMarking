#include "Sensor.hh"
#include "Muscle.hh"
#include "Sml.hh"
#include <boost/any.hpp>

#include <sml/sml-tools/RungeKutta.hh>
#include <sml/sml-tools/EventManager.hh>
#include <sml/sml-tools/Helper.hh>

extern EventManager* eventManager;
using namespace std;

double Sensor::tf(double x){
    static double y;
    static int noise_sensor = Settings::get<int>("noise_sensor");
    y=x;
    
    if(noise_sensor == 1 && eventManager->get<bool>(STATES::IS_LAST_PHASE))
    {
        y += getSignalDependentNoise(y);
    }
    
    
    return y;
}
  

Trunk::Trunk(Sml *sml,
    INPUT::Inputs input_id,
    double& theta_trunk_ref, 
    double& theta_offset): 
        sml(sml),
        input_id(input_id),
        theta(sml->Input[input_id]), 
        theta_ref(theta_trunk_ref), 
        theta_offset(theta_offset), 
        d_theta(0.0), 
        theta_to(theta_trunk_ref){
            //theta = - sml->Input[INPUT::THETA_TRUNK];
            
        }
double Trunk::getAngle(){
    return sml->Input[input_id];
}
void Trunk::step(double dt){
    static double old_theta;
    old_theta     = theta;
    theta = sml->Input[input_id];
    d_theta     = (theta     - old_theta   )  / dt;
}
void Trunk::setThetaRef(double t){
    theta_ref = t;
}
double Trunk::getThetaRef(){
    return theta_ref;
}
void Trunk::addThetaRef(double t){
    theta_ref += t;
}
    

double TouchSensor::getInput(){
    return sml->Input[input_id];
}
double TouchSensor::update(){
    static vector<double> in(1,0.0);
    static vector<double> y(1,0.0);
    in.at(0) = getInput();
    y.at(0) = lastValue;
    
    y=rungeKutta4(eventManager->get<double>(STATES::TIME),dt,y,in,(Derivatives *)this);
    lastValue = y[0];
    return tf(lastValue);
}

vector<double> TouchSensor::dydt(double x, vector<double> y, vector<double> param, vector<double> input){
    static vector<double> dadt(1,0.0);
    static double tau = 5.0/(5.0/0.6931*dt);
    if(input[0]<y[0])
        tau = 1.0/(5.0/0.6931*dt);
    dadt.at(0) = tau*(input[0]-y[0]);
    return dadt;
}




double MuscleLengthSensor::update(){
    //change was: return gain*(muscle->l_CE/muscle->l_opt - offset);
    static double out;
    out = gain*(muscle->getL_CE()/muscle->l_opt - offset);
    if(out <0)
        out = 0.0;
    return tf(out);
}

double MuscleForceSensor::update(){
    return tf(gain * muscle->getForce() / muscle->F_max);
}
double JointPositionSensor::update(){
    return tf(kp * (this->joint->angle - ref) + kd * joint->d_angle);
}
double TrunkPositionSensor1::update(){
    return tf(
        kp * (this->trunk->theta - this->trunk->theta_ref - this->trunk->theta_offset) 
        + 
        kd * trunk->d_theta
           );
}


double TrunkPositionSensor2::update(){
    return tf( kp *0.68 * (this->trunk->theta - this->trunk->theta_ref - this->trunk->theta_offset) 
        + 
        kd * trunk->d_theta
    );
}
double TrunkTakeOffSensor::update(){
    return trunk->theta_to - ref;
}

double TrunkTakeOffSensorOld::update(){
    return trunk->theta_to - trunk->theta_ref;
}
double AngleSensor::update(){
    return joint->angle - offset;
}
/**
 * 
 * Foot class (special sensors that have to TouchSensor to feel the ground 
 * and also a limb_state variable that says if the foot thinks is in stance or swing
 * 
 *   This is a special sensor used by the Gate that turns the feedbacks on/off 
 *   regarding the limb_state on which they need to be active.
 *   
 */

std::string Foot::getName(){
    return SIDE::toString(side);
}

double Foot::update(){
    return tf(getFeltForce()/scalingfactor);
}

bool Foot::inStance(){
    if(limb_state==CYCLE_PHASE::STANCE)
        return true;
    else
        return false;
}
bool Foot::justTouchTheGround(){
    if(limb_state==CYCLE_PHASE::STANCE && lastlimb_state==CYCLE_PHASE::SWING){
        return true;
    }
    else
        return false;
}
bool Foot::justGetOffGround(){
    if(limb_state==CYCLE_PHASE::SWING && lastlimb_state==CYCLE_PHASE::STANCE)
        return true;
    else
        return false;
}
double Foot::getFeltForce(){
    static double x;
    x = heel->get()+toe->get();
    return 1/(1+exp(-5*(x-100)))*x; // pass through a sigmoid filter
}
double Foot::getRealFeltForce(){
    return heel->get()+toe->get();
}
double Foot::getXForce(){
    return 0.0;//heel->get(0)+toe->get(0);
}
double Foot::getYForce(){
    return heel->getInput()+toe->getInput();
}
//double Foot::getZForce(){
//    return heel->get(2)+toe->get(2);
//}
CYCLE_PHASE::CyclePhase Foot::getLastState(){
    return lastlimb_state;
}
CYCLE_PHASE::CyclePhase Foot::getState(){
    return limb_state;
}
void Foot::setState(CYCLE_PHASE::CyclePhase str){
    limb_state = str;
}
void Foot::updateState(double actual_time){
    lastlimb_state = limb_state;
    //      if(actual_time - last_time_limb_state_changed > delay){
        if(this->getRealFeltForce() > 0.1){ //if the foot bear enough weight, the leg is in stance
            limb_state=CYCLE_PHASE::STANCE;
        }
        else{
            limb_state=CYCLE_PHASE::SWING;
        }
        //      }
        if(lastlimb_state != limb_state){
            last_time_limb_state_changed = actual_time;
            if(lastlimb_state == CYCLE_PHASE::SWING && limb_state == CYCLE_PHASE::STANCE)
            {
                duration_last_step = actual_time - last_time_foot_touch;
                last_time_foot_touch = actual_time;
                if(duration_last_step < 0.0 && !eventManager->get<bool>(STATES::IS_LAUNCHING_PHASE)){
#ifndef REAL_TIME
                    std::cout << "error duration negative: " << duration_last_step << std::endl;
#endif
                }
            }
        }
        //return limb_state;
}
bool Foot::changeStateAt(){
    return last_time_limb_state_changed;
}

bool Foot::touchedBefore(Foot * cfoot){
    if (this->last_time_foot_touch < cfoot->last_time_foot_touch )
        return true;
    else
        return false;
}
