#include "EkerbergMouse.hh"
#include <sml/sml-tools/Settings.hh>
#include <webots/Node.hpp>
#include <webots/Receiver.hpp>
#include <webots/PositionSensor.hpp>
#include <webots/Motor.hpp>
#include <webots/Emitter.hpp>
#include <boost/xpressive/xpressive.hpp>
#include <cstdlib>

using namespace std;
using namespace webots;
using namespace boost::xpressive;
using namespace MOUSE;

extern double debug;
extern EventManager* eventManager;
extern SmlParameters parameters;


void EkerbergMouse::ConstantInit() {
    // Segment constant
    sml::Constant[CONSTANT::LENGTH_THIGH] = 0.016089;
    sml::Constant[CONSTANT::WEIGHT_THIGH] = 0.000441;
    sml::Constant[CONSTANT::LENGTH_SHIN] = 0.017879;
    sml::Constant[CONSTANT::WEIGHT_SHIN] = 0.000216;
    sml::Constant[CONSTANT::LENGTH_FOOT] = 0.016258;
    sml::Constant[CONSTANT::WEIGHT_FOOT] = 0.000063;
    // Joints constant

    // Left
    sml::Constant[CONSTANT::REF_POS_HIP_LEFT] = 0 * (2.0 * PI / 360.0);
    sml::Constant[CONSTANT::MAX_POS_HIP_LEFT] = +50.0  * (2.0 * PI / 360.0) + sml::Constant[CONSTANT::REF_POS_HIP_LEFT];
    sml::Constant[CONSTANT::MIN_POS_HIP_LEFT] = -30.0  * (2.0 * PI / 360.0) + sml::Constant[CONSTANT::REF_POS_HIP_LEFT];
    sml::Constant[CONSTANT::REF_POS_KNEE_LEFT] = 90 * (2.0 * PI / 360.0);
    sml::Constant[CONSTANT::MAX_POS_KNEE_LEFT] = -45.0  * (2.0 * PI / 360.0) + sml::Constant[CONSTANT::REF_POS_KNEE_LEFT];
    sml::Constant[CONSTANT::MIN_POS_KNEE_LEFT] = -145.0  * (2.0 * PI / 360.0) + sml::Constant[CONSTANT::REF_POS_KNEE_LEFT];
    sml::Constant[CONSTANT::REF_POS_ANKLE_LEFT] = 0 * (2.0 * PI / 360.0);
    sml::Constant[CONSTANT::MAX_POS_ANKLE_LEFT] = +45.0  * (2.0 * PI / 360.0) + sml::Constant[CONSTANT::REF_POS_ANKLE_LEFT];
    sml::Constant[CONSTANT::MIN_POS_ANKLE_LEFT] = -45.0  * (2.0 * PI / 360.0) + sml::Constant[CONSTANT::REF_POS_ANKLE_LEFT];


    // Right
    sml::Constant[CONSTANT::REF_POS_HIP_RIGHT] = 0 * (2.0 * PI / 360.0);
    sml::Constant[CONSTANT::MAX_POS_HIP_RIGHT] = +50.0  * (2.0 * PI / 360.0) + sml::Constant[CONSTANT::REF_POS_HIP_RIGHT];
    sml::Constant[CONSTANT::MIN_POS_HIP_RIGHT] = -30.0  * (2.0 * PI / 360.0) + sml::Constant[CONSTANT::REF_POS_HIP_RIGHT];
    sml::Constant[CONSTANT::REF_POS_KNEE_RIGHT] = 90 * (2.0 * PI / 360.0);
    sml::Constant[CONSTANT::MAX_POS_KNEE_RIGHT] = -45.0  * (2.0 * PI / 360.0) + sml::Constant[CONSTANT::REF_POS_KNEE_RIGHT];
    sml::Constant[CONSTANT::MIN_POS_KNEE_RIGHT] = -145.0  * (2.0 * PI / 360.0) + sml::Constant[CONSTANT::REF_POS_KNEE_RIGHT];
    sml::Constant[CONSTANT::REF_POS_ANKLE_RIGHT] = 0 * (2.0 * PI / 360.0);
    sml::Constant[CONSTANT::MAX_POS_ANKLE_RIGHT] = +45.0  * (2.0 * PI / 360.0) + sml::Constant[CONSTANT::REF_POS_ANKLE_RIGHT];
    sml::Constant[CONSTANT::MIN_POS_ANKLE_RIGHT] = -45.0  * (2.0 * PI / 360.0) + sml::Constant[CONSTANT::REF_POS_ANKLE_RIGHT];


    // MUSCLES

    //PMA
    sml::Constant[CONSTANT::REF_PHI_PMA] =  7.5758 * (2.0 * PI / 360.0) + sml::Constant[CONSTANT::REF_POS_HIP_LEFT];
    sml::Constant[CONSTANT::MAX_PHI_PMA] =  41.9192 * (2.0 * PI / 360.0) + sml::Constant[CONSTANT::REF_POS_HIP_LEFT];
    //CF
    sml::Constant[CONSTANT::REF_PHI_CF] = 0.5050 * (2.0 * PI / 360.0) + sml::Constant[CONSTANT::REF_POS_HIP_LEFT];
    sml::Constant[CONSTANT::MAX_PHI_CF] = -21.7172 * (2.0 * PI / 360.0) + sml::Constant[CONSTANT::REF_POS_HIP_LEFT];
    //SM
    sml::Constant[CONSTANT::REF_PHI_SM_HIP] = -29.798 * (2.0 * PI / 360.0) + sml::Constant[CONSTANT::REF_POS_HIP_LEFT];
    sml::Constant[CONSTANT::MAX_PHI_SM_HIP] = -4.5455 * (2.0 * PI / 360.0) + sml::Constant[CONSTANT::REF_POS_HIP_LEFT];
    sml::Constant[CONSTANT::REF_PHI_SM_KNEE] = -61.5152 * (2.0 * PI / 360.0) + sml::Constant[CONSTANT::REF_POS_KNEE_LEFT];
    sml::Constant[CONSTANT::MAX_PHI_SM_KNEE] = -92.2727 * (2.0 * PI / 360.0) + sml::Constant[CONSTANT::REF_POS_KNEE_LEFT];
    //POP
    //CHANGE = -118.6364 -> -90
    sml::Constant[CONSTANT::REF_PHI_POP] =  -95.6364 * (2.0 * PI / 360.0) + sml::Constant[CONSTANT::REF_POS_KNEE_LEFT];
    sml::Constant[CONSTANT::MAX_PHI_POP] = -136.2121 * (2.0 * PI / 360.0) + sml::Constant[CONSTANT::REF_POS_KNEE_LEFT];
    //RF
    sml::Constant[CONSTANT::REF_PHI_RF] =  -95.404 * (2.0 * PI / 360.0) + sml::Constant[CONSTANT::REF_POS_KNEE_LEFT];
    sml::Constant[CONSTANT::MAX_PHI_RF] =  -45.404 * (2.0 * PI / 360.0) + sml::Constant[CONSTANT::REF_POS_KNEE_LEFT];
    //TA
    sml::Constant[CONSTANT::REF_PHI_TA] =  22.7273 * (2.0 * PI / 360.0) + sml::Constant[CONSTANT::REF_POS_ANKLE_LEFT];
    sml::Constant[CONSTANT::MAX_PHI_TA] =  10.6061 * (2.0 * PI / 360.0) + sml::Constant[CONSTANT::REF_POS_ANKLE_LEFT];
    //SOL
    sml::Constant[CONSTANT::REF_PHI_SOL] =  -2.5253 * (2.0 * PI / 360.0) + sml::Constant[CONSTANT::REF_POS_ANKLE_LEFT];
    sml::Constant[CONSTANT::MAX_PHI_SOL] =  2.5253 * (2.0 * PI / 360.0) + sml::Constant[CONSTANT::REF_POS_ANKLE_LEFT];
    //LG
    sml::Constant[CONSTANT::REF_PHI_LG_KNEE] = -38.0808 * (2.0 * PI / 360.0) + sml::Constant[CONSTANT::REF_POS_KNEE_LEFT];
    sml::Constant[CONSTANT::MAX_PHI_LG_KNEE] = -45.404 * (2.0 * PI / 360.0) + sml::Constant[CONSTANT::REF_POS_KNEE_LEFT];
    sml::Constant[CONSTANT::REF_PHI_LG_ANKLE] = -20.7071 * (2.0 * PI / 360.0) + sml::Constant[CONSTANT::REF_POS_ANKLE_LEFT];
    sml::Constant[CONSTANT::MAX_PHI_LG_ANKLE] = -0.5050 * (2.0 * PI / 360.0) + sml::Constant[CONSTANT::REF_POS_ANKLE_LEFT];
}

void EkerbergMouse::InputInit() {
    for (unsigned joint_pos = INPUT::FIRST_JOINT, joint = JOINT::FIRST_JOINT; joint_pos <= INPUT::FIRST_JOINT + 5; joint_pos++, joint++) {
#ifdef WEBOTS6
        getServo(JOINT::toString(joint))->enablePosition(time_step);
#elif WEBOTS8
        getPositionSensor(JOINT::toString(joint) + "_POS")->enable(time_step);
#else
        getMotor(JOINT::toString(joint))->enablePosition(time_step);
#endif
    }

    getTouchSensor("LEFT_TOUCH")->enable(time_step);
    getTouchSensor("RIGHT_TOUCH")->enable(time_step);

    InputUpdate();
}

void EkerbergMouse::InputUpdate() {
    for (unsigned joint_pos = INPUT::FIRST_JOINT, joint = JOINT::FIRST_JOINT; joint_pos <= INPUT::FIRST_JOINT + 5; joint_pos++, joint++) {
#ifdef WEBOTS6
        sml::Input[joint_pos] = getServo(JOINT::toString(joint))->getPosition();
#elif WEBOTS8
        sml::Input[joint_pos] = -1 * getPositionSensor(JOINT::toString(joint) + "_POS")->getValue();
#else
        if (JOINT::toString(joint) == "HIP_LEFT")
        {
            sml::Input[joint_pos] = -1 * getMotor(JOINT::toString(joint))->getPosition();
        }
        else if (JOINT::toString(joint) == "KNEE_LEFT")
        {
            sml::Input[joint_pos] = getMotor(JOINT::toString(joint))->getPosition();
        }
        else
        {
            sml::Input[joint_pos] = -1 * getMotor(JOINT::toString(joint))->getPosition();
        }
#endif
    }
}

void EkerbergMouse::init() {

    supervisor::step(time_step);

    control  = new EkerbergControl();
}

void EkerbergMouse::step_SPINE_TO_MTU(double dt) 
{
    static double time = 0;
    time += 0.001;

    //Get Muscle Forces
    for (uint j = 0; j < 16; j ++)
    {
        muscleForces[j] = muscles[j]->getForce();
    }

    control->Step(sml::Input,muscleForces,getTouchSensor("LEFT_TOUCH")->getValue(), getTouchSensor("RIGHT_TOUCH")->getValue());

    //control->DebugStates(sml::Input,muscleForces,getTouchSensor("LEFT_TOUCH")->getValue(), getTouchSensor("RIGHT_TOUCH")->getValue(), 'L' + '0' , '1' + '0' );

    muscleActivations = control->MuscleActivations();

    // Stimulate the muscles of hind limbs
    for (int j = 0; j < 16; j++)
    {
        muscles[j]->stim = muscleActivations[j];
    }

}

int EkerbergMouse::step() {
    // FOR DEBUG PURPOSE ONLY
    static int f = 1;
    if (f < 1)
    {
        Settings::printAll();
        f++;
    }

    sml::step();

    step_SPINE_TO_MTU(dt);

    for (unsigned joint = JOINT::FIRST_JOINT,
        joint_tau = OUTPUT::FIRST_JOINT;
        joint_tau <= OUTPUT::FIRST_JOINT + 5;
        joint_tau++,
        joint++
        )
    {
        double torque;

#ifdef WEBOTS6
        torque = sml::Output[joint_tau];
#else
        torque = -sml::Output[joint_tau];
        {
            if (JOINT::toString(joint) == "HIP_LEFT2")
                torque = -sml::Output[joint_tau];
            else
                torque = sml::Output[joint_tau];
        }
#endif

#ifdef WEBOTS6
        getServo(JOINT::toString(joint))->setForce(torque);
#else
        // WEBOTS 8.6.0 has sign change
        getMotor(JOINT::toString(joint))->setTorque(torque * 10000000);

#endif
    }

    getMotor("FORE_U_LEFT")->setPosition(0);
    getMotor("FORE_L_LEFT")->setPosition(0);
    getMotor("FOREPAW_LEFT")->setTorque(0);
    getMotor("FORE_U_RIGHT")->setPosition(0);
    getMotor("FORE_L_RIGHT")->setPosition(0);
    getMotor("FOREPAW_RIGHT")->setTorque(0);

    return 0;
}

EkerbergMouse::~EkerbergMouse() 
{}
