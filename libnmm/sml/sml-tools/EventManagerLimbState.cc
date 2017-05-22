#include "EventManager.hh"
#include "Settings.hh"
#include <boost/bind/protect.hpp>
#include <iostream>

using namespace STATES;
using namespace HUMAN;

extern SmlParameters parameters;
void EventManager::updateFeet(){
    
    updateFeetState();
    updateFeetDistanceToCM();
}
void EventManager::updateFeetState(){
    if( ((Foot *)sml->sensors["right_foot"])->inStance() ){
        states[RIGHT_FOOT] = CYCLE_PHASE::STANCE_START;
    }
    else{
        states[RIGHT_FOOT] = CYCLE_PHASE::SWING_START;
    }
    if( ((Foot *)sml->sensors["left_foot"])->inStance() ){
        states[LEFT_FOOT] = CYCLE_PHASE::STANCE_START;
    }
    else{
        states[LEFT_FOOT] = CYCLE_PHASE::SWING_START;
    }
    static double swing_threshold;
    static double stance_threshold;
    
    swing_threshold = parameters[0]["swing_end"]+parameters[1]["swing_end"];
    stance_threshold = parameters[0]["stance_end"]+parameters[1]["stance_end"];
    
    if( boost::get<int>(states[LEFT_FOOT]) == CYCLE_PHASE::SWING_START )
        if(get<double>(LEFT_Z_DIST_NECK) > swing_threshold)
            states[LEFT_FOOT]  = CYCLE_PHASE::SWING_END;
    if( boost::get<int>(states[RIGHT_FOOT]) == CYCLE_PHASE::SWING_START )
        if(get<double>(LEFT_Z_DIST_NECK) > swing_threshold)
            states[RIGHT_FOOT] = CYCLE_PHASE::SWING_END;
  
    /*    */
    if( boost::get<int>(states[LEFT_FOOT]) == CYCLE_PHASE::STANCE_START )
        if(get<double>(LEFT_Z_DIST_NECK) < stance_threshold)
            states[LEFT_FOOT] = CYCLE_PHASE::STANCE_END;
    if( boost::get<int>(states[RIGHT_FOOT]) == CYCLE_PHASE::STANCE_START )
        if(get<double>(RIGHT_Z_DIST_NECK) < stance_threshold)
            states[RIGHT_FOOT] = CYCLE_PHASE::STANCE_END;
    /*     */
    if( sml->right_foot->inStance() && sml->left_foot->inStance() ) {
        if( sml->right_foot->touchedBefore(sml->left_foot) ) {
            states[RIGHT_FOOT] = CYCLE_PHASE::STANCE_END;
        }
        else {
            states[LEFT_FOOT]  = CYCLE_PHASE::STANCE_END;
        }
    }
}
void EventManager::updateFeetDistanceToCM(){
   updateFootDistanceToCM(SIDE::LEFT);
   updateFootDistanceToCM(SIDE::RIGHT);
}
using namespace boost::numeric::ublas;
namespace MAT{
    matrix<double> R(double x){
        matrix<double> m (2, 2);
        m(0,0) = cos(x);
        m(0,1) = -sin(x);
        m(1,0) = sin(x);
        m(1,1) = cos(x);
        return m;
    }
    vector<double> Vxy(double x, double y, double theta){
        vector<double> X (2);
        X(0) = x;
        X(1) = y;
        return prod(X,R(theta));
    }
    vector<double> V0y(double y){
        return Vxy(0.0,y,0.0);
    }
    vector<double> V0y(double y, double theta){
        return Vxy(0.0,y,theta);
    }
    vector<double> Vxy(double x, double y){
        return Vxy(x,y,0.0);
    }

}
double EventManager::getAngle(JOINT::Joints j){
    return sml->joints[j]->getAngle();
}
vector<double> EventManager::getSegment(SEGMENTS::Segments s){
    using namespace SEGMENTS;
    static double y;
    y = 0.0;
    switch(s){
        case TRUNK:
            y = sml->Constant[CONSTANT::LENGTH_TRUNK];
            break;
        case THIGH:
            y = sml->Constant[CONSTANT::LENGTH_THIGH];
            break;
        case SHIN:
            y = sml->Constant[CONSTANT::LENGTH_SHIN];
            break;
        case ANKLE:
            y = sml->Constant[CONSTANT::LENGTH_ANKLE];
            break;
        case FOOT:
            y = sml->Constant[CONSTANT::LENGTH_FOOT];
            break;
    }
    static vector<double> X (2,0.0);
    X(0) = 0.0;
    X(1) = y;
    return X;
}
void EventManager::updateFootDistanceToCM(SIDE::Side side){
#if VERSION == 6 || VERSION == 5
    if(side == SIDE::LEFT){
        states[STATES::LEFT_X_DIST_NECK] = -sml->Input[INPUT::MIDHIP_X]+sml->Input[INPUT::TOE_LEFT_X];
        states[STATES::LEFT_Z_DIST_NECK] = -sml->Input[INPUT::MIDHIP_Z]+sml->Input[INPUT::TOE_LEFT_Z];
        states[STATES::LEFT_Y_DIST_NECK] = sml->Input[INPUT::MIDHIP_Y]-sml->Input[INPUT::TOE_LEFT_Y];
    }
    else{
        states[STATES::RIGHT_X_DIST_NECK] = -sml->Input[INPUT::MIDHIP_X]+sml->Input[INPUT::TOE_RIGHT_X];
        states[STATES::RIGHT_Z_DIST_NECK] = -sml->Input[INPUT::MIDHIP_Z]+sml->Input[INPUT::TOE_RIGHT_Z];
        states[STATES::RIGHT_Y_DIST_NECK] = sml->Input[INPUT::MIDHIP_Y]-sml->Input[INPUT::TOE_RIGHT_Y];
    }
	//std::cout << states[STATES::LEFT_X_DIST_NECK] << "," << states[STATES::RIGHT_X_DIST_NECK] << std::endl;        
	//std::cout << states[STATES::LEFT_Z_DIST_NECK] << "," << states[STATES::RIGHT_Z_DIST_NECK] << std::endl;        
    //std::cout << states[STATES::RIGHT_X_DIST_NECK] << "," << states[STATES::RIGHT_Y_DIST_NECK] << std::endl;
#else
#warning "Old update foot distance to CM think of ugrading your code (look up version 6)"
    using namespace SEGMENTS;
    using namespace MAT;
    using namespace JOINT;

    vector<double> x0 = getSegment(SEGMENTS::TRUNK);
    vector<double> x1 = getSegment(THIGH);
    vector<double> x2 = getSegment(SHIN);
    vector<double> x3 = getSegment(ANKLE);
    vector<double> x4 = getSegment(FOOT);
    double a0 = sml->trunk->getAngle();
    double a1;
    double a2;
    double a3;
    if(side == SIDE::LEFT){
        a1 = getAngle(HIP_LEFT);
        a2 = getAngle(KNEE_LEFT);
        a3 = getAngle(ANKLE_LEFT);
    }
    else{
        a1 = getAngle(HIP_RIGHT);
        a2 = getAngle(KNEE_RIGHT);
        a3 = getAngle(ANKLE_RIGHT);
    }
    double a4 = -PI/2;
    //std::cout << sml->trunk->getAngle() << std::endl;
    //std::cout << getAngle(HIP_LEFT) << " " << getAngle(KNEE_LEFT) << " " << getAngle(ANKLE_LEFT) << std::endl;
    //std::cout << getAngle(HIP_RIGHT) << " " << getAngle(KNEE_RIGHT) << " " << getAngle(ANKLE_RIGHT) << std::endl;
    vector<double> l = x0
                      +prod(R(a1),x1)
                      +prod(R(a1),vector<double>(
                          prod(R(a2),x2)))
                      +prod(R(a1),vector<double>(
                          prod(R(a2),vector<double>(
                              prod(R(a3),x3)))))
                      +prod(
                          R(a1),vector<double>(
                              prod(R(a2),vector<double>(
                                  prod(R(a3),vector<double>(
                                      prod(R(a4),x4)))))))
                      ;
    vector<double> l2 =prod(R(a0),l); 
    if(side == SIDE::LEFT){
        states[STATES::LEFT_X_DIST_NECK] = l2(0);
        states[STATES::LEFT_Y_DIST_NECK] = l2(1);
    }
    else{
        states[STATES::RIGHT_X_DIST_NECK] = l2(0);
        states[STATES::RIGHT_Y_DIST_NECK] = l2(1);
    }
#endif
}