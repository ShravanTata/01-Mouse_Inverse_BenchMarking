#include "Motoneuron.hh"
#include "Entity.hh"
#include <sml/sml-tools/EventManager.hh>
#include <sml/musculoSkeletalSystem/Interneuron.hh>

#include <fstream>
#include <iostream>
using namespace std;

Motoneuron::Motoneuron(SIDE::Side side, string name, double& muscle,int time_step):Entity(side), name(name), muscle(muscle),input(0.0),tau(0.01),dt((double)time_step/1000.0){}
string Motoneuron::getName(){
    static string side_str;
    side_str = "left_";
    if (side == SIDE::RIGHT) side_str = "right_";
    return side_str+name;
}
void Motoneuron::apply(){
    static int mn_tf = Settings::get<int>("transfer_function_motoneurons");
    static double dA;
	if(mn_tf == 1)
	{
		state = tf(input);
	}
	else{
        dA = (input-state) * dt/tau;
        state += dA;
	}
	
} // to change, add a differential equation
void Motoneuron::set(double value){this->state = value;}
void Motoneuron::add(double value){this->input += value;}