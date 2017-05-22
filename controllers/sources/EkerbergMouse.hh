#ifndef __HindMouse_HH__
#define __HindMouse_HH__

#include <string>
#include <math.h>
#include <vector>
#include <fstream>
#include <map>
#include <limits>
#include <iostream>
//#include <boost/algorithm/string.hpp>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <webots/Keyboard.hpp>
#include <webots/Servo.hpp>
#include <webots/Supervisor.hpp>
#include <webots/TouchSensor.hpp>
#include <webots/Display.hpp>
#include <sml/types/types.h>
#include <sml/sml-tools/PerturbationManager.hh>
#include <sml/musculoSkeletalSystem/SmlHindMouse.hh>
#include <sml/sml-tools/EventManager.hh>
#include "EkerbergControl.hh"

#define DEBUG 0

class EkerbergMouse: public webots::Supervisor, public SmlHindMouse
{
private:

    void ConstantInit();
    void InputInit();
    void InputUpdate();
    void step_SPINE_TO_MTU(double dt);
    int getJointPosition();
    
    ~EkerbergMouse();

    std::vector<double> muscleForces;
    std::vector<double> muscleActivations;
    webots::Keyboard key;
    EkerbergControl *control;

public:
    typedef Supervisor supervisor;
    typedef SmlHindMouse sml;
    int step();
    const int numJoints = 0;
  //-----------------------------------

    //constructor
    EkerbergMouse(): 
    muscleForces(16,0.0),
    muscleActivations(16,0.01),
    SmlHindMouse(){
        sml::init();
        init();
        }

    //destructor
    //~Rob();
    void init();
    // Variables
    std::ifstream joint_data_file;
    double joint_angles[6];
};

#endif /* __HINDMOUSE_HH__ */
