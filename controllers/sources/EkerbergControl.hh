#ifndef __EKERBERG_CONTROL_HH__
#define __EKERBERG_CONTROL_HH__

#include <iostream>
#include <vector>
#include <sml/types/types.h>
#include <string>

#define CONTROL_DEBUG 0

class EkerbergControl
{

private:
	// Variables
	std::vector<double> muscleActivations;
	std::vector<double> activations;
	std::vector<EKERBERG_PHASE::Phase> legCurrentPhase;
	std::vector<EKERBERG_PHASE::Phase> legPrevPhase;

	const bool crossCoupling = 1;
	SIDE::Side contraLateralSide;
	std::vector<double> angles;
	std::vector<double> forces;

	// Functions 
	void NeuralControl(bool groundContact_LEFT,SIDE::Side side);
	void Stance();
	void LiftOff();
	void Swing();
	void TouchDown();
	void PrintDebug(std::string t);

public:
	EkerbergControl() ;
	void Init();
	void Step(std::vector<double>& jointAngles, std::vector<double>& muscleForces, bool groundContact_LEFT,bool groundContact_RIGHT);	
	std::vector<double> MuscleActivations();
};

#endif /* __EKERBERG_CONTROL_HH__ */