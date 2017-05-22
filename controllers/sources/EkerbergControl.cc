#include "EkerbergControl.hh"

using namespace std;

EkerbergControl::EkerbergControl() : 
muscleActivations(16,0.01),
legCurrentPhase(2,EKERBERG_PHASE::STANCE),
legPrevPhase(2,EKERBERG_PHASE::TOUCH_DOWN),
angles(3,0.0),
forces(8,0.0),
activations(8,0.01)
{

	cout << "Ekerberg Control Constructor called " << endl;

}

void EkerbergControl::Init()
{
	PrintDebug("Ekerberg Control Init Function");
}

void EkerbergControl::Step(std::vector<double>& jointAngles, std::vector<double>& muscleForces, bool groundContact_LEFT,bool groundContact_RIGHT)
{

	// LEFT Leg
	for (uint j = 0; j < 3; j++)
	{
		angles[j] = jointAngles[j];
	}

	for (uint j = 0; j < 8; j++)
	{
		forces[j] = muscleForces[j];
	}

	SIDE::Side legSide = SIDE::LEFT;

	this->NeuralControl(groundContact_LEFT,legSide);

	for (uint j = 0 ; j < 8; j++)
	{
		muscleActivations[j] = activations[j];
	}

	// RIGHT Leg
	for (uint j = 3; j < 6; j++)
	{
		angles[j-3] = jointAngles[j];
	}

	for (uint j = 8; j < 16; j++)
	{
		forces[j-8] = muscleForces[j];
	}

	legSide = SIDE::RIGHT;

	this->NeuralControl(groundContact_RIGHT, legSide);

	for (uint j = 8 ; j < 16; j++)
	{
		muscleActivations[j] = activations[j-8];
	}
}

void EkerbergControl::NeuralControl(bool groundContact, SIDE::Side side)
{
	// Four States :
	/*
	1. Lift off : 
	2. Swing :
	3. TouchDown : 
	4. Stance : 
	*/

	if(crossCoupling)
	{
		SIDE::Side contraLateralSide = (SIDE::Side)!side;
	}

	if ((groundContact) && (legCurrentPhase[side] == EKERBERG_PHASE::TOUCH_DOWN) && (legPrevPhase[side] == EKERBERG_PHASE::SWING))
	{

		legCurrentPhase[side] = EKERBERG_PHASE::STANCE;

		legPrevPhase[side] = EKERBERG_PHASE::TOUCH_DOWN;

		PrintDebug(EKERBERG_PHASE::ToString(legCurrentPhase[side]) + " : " + SIDE::toString(side) + 
		"     " + EKERBERG_PHASE::ToString(legCurrentPhase[contraLateralSide]) + " : " + SIDE::toString(contraLateralSide) + 
		"     " + " Side Val : " + to_string(side) + " ContraSide Val : " + to_string(contraLateralSide));	
	}

	else if (((angles[0] < - 0.4) && (forces[6] < 1)) && (legCurrentPhase[side] == EKERBERG_PHASE::STANCE) && (legPrevPhase[side] == EKERBERG_PHASE::TOUCH_DOWN) && (legCurrentPhase[!side] == EKERBERG_PHASE::STANCE))
	{

		legCurrentPhase[side] = EKERBERG_PHASE::LIFT_OFF;

		legPrevPhase[side] = EKERBERG_PHASE::STANCE;

		PrintDebug(EKERBERG_PHASE::ToString(legCurrentPhase[side]) + " : " + SIDE::toString(side) + 
		"     " + EKERBERG_PHASE::ToString(legCurrentPhase[contraLateralSide]) + " : " + SIDE::toString(contraLateralSide) + 
		"     " + " Side Val : " + to_string(side) + " ContraSide Val : " + to_string(contraLateralSide));	
	}

	else if ((!groundContact) && (legCurrentPhase[side] == EKERBERG_PHASE::LIFT_OFF) && (legPrevPhase[side] == EKERBERG_PHASE::STANCE))
	{

		legCurrentPhase[side] = EKERBERG_PHASE::SWING;

		legPrevPhase[side] = EKERBERG_PHASE::LIFT_OFF;

		PrintDebug(EKERBERG_PHASE::ToString(legCurrentPhase[side]) + " : " + SIDE::toString(side) + 
		"     " + EKERBERG_PHASE::ToString(legCurrentPhase[contraLateralSide]) + " : " + SIDE::toString(contraLateralSide) + 
		"     " + " Side Val : " + to_string(side) + " ContraSide Val : " + to_string(contraLateralSide));
	}

	else if (((angles[0] > 0.6)&&(angles[1] > -0.8)) && (legCurrentPhase[side] == EKERBERG_PHASE::SWING) && (legPrevPhase[side] == EKERBERG_PHASE::LIFT_OFF))
	{

		legCurrentPhase[side] = EKERBERG_PHASE::TOUCH_DOWN;

		legPrevPhase[side] = EKERBERG_PHASE::SWING;

		PrintDebug(EKERBERG_PHASE::ToString(legCurrentPhase[side]) + " : " + SIDE::toString(side) + 
		"     " + EKERBERG_PHASE::ToString(legCurrentPhase[contraLateralSide]) + " : " + SIDE::toString(contraLateralSide) + 
		"     " + " Side Val : " + to_string(side) + " ContraSide Val : " + to_string(contraLateralSide));
	}

	if ((legCurrentPhase[side] == EKERBERG_PHASE::STANCE) && (legPrevPhase[side] == EKERBERG_PHASE::TOUCH_DOWN))
	{
		Stance();
	}

	else if ((legCurrentPhase[side] == EKERBERG_PHASE::LIFT_OFF) && (legPrevPhase[side] == EKERBERG_PHASE::STANCE))
	{
		LiftOff();
	}

	else if ((legCurrentPhase[side] == EKERBERG_PHASE::SWING) && (legPrevPhase[side] == EKERBERG_PHASE::LIFT_OFF))
	{
		Swing();
	}

	else if ((legCurrentPhase[side] == EKERBERG_PHASE::TOUCH_DOWN) && (legPrevPhase[side] == EKERBERG_PHASE::SWING))
	{
		TouchDown();
	}

}

void EkerbergControl::Stance()
{
	 // Hip Extension | Knee Extension & Knee Flexion | Ankle Extension 
	// PrintDebug("Setting muscle activations");

	activations[0] = 0.01;
	activations[1] = (angles[0] > -0.349 ) ? 0.95 : 0.85; // Hip Extension
	activations[2] = 0.01;
	activations[3] = 0.01;
	activations[4] = 0.05 + 0.02*forces[4] + 0.1*(angles[0] - 0.8726); // Knee Extension 
	activations[5] = 0.01;
	activations[6] = 0.8 + 0.02*forces[7]; // Ankle Extension
	activations[7] = 0.01*forces[6]; 

	// PrintDebug("Returning from transition");
}

void EkerbergControl::Swing()
{
	 // Hip Flexion | Hip Extension & Knee Flexion | Ankle Flexion 
	// PrintDebug("Setting muscle activations");

	activations[0] = 0.8 + 0.01*(0.69813 - angles[1]); // Hip Flexion
	activations[1] = 0.01;
	activations[2] = 0.01; // Knee Flexion Hip Extension 
	activations[3] = 0.01;
	activations[4] = 0.01;
	activations[5] = 0.8 + 0.01*(0.69813 - angles[1]); // Ankle Flexion
	activations[6] = 0.01;
	activations[7] = 0.01;
	// PrintDebug("Returning from transition");
}

void EkerbergControl::TouchDown()
{
	 // Hip Flexion | Hip Extension & Knee Flexion | Ankle Flexion 
	// PrintDebug("Setting muscle activations");

	activations[0] = 0.01; // Hip Flexion
	activations[1] = 0.85;
	activations[2] = 0.01; // Knee Flexion Hip Extension 
	// cout << "SM : " <<  ((angles[0] - angles[1])/5) << endl;
	activations[3] = 0.01;
	activations[4] = 0.01;
	activations[5] = 0.01; // Ankle Flexion
	activations[6] = 0.75;
	activations[7] = 0.01;
	// PrintDebug("Returning from transition");
}

void EkerbergControl::LiftOff()
{
	 // Hip Flexion | Hip Extension & Knee Flexion | Ankle Flexion 
	// PrintDebug("Setting muscle activations");

	activations[0] = 0.5; // Hip Extension
	activations[1] = 0.01;
	activations[2] = 0.1; // Knee Flexion Hip Extension 
	activations[3] = 0.01;
	activations[4] = 0.01;
	activations[5] = 0.9; // Ankle Flexion
	activations[6] = 0.01;
	activations[7] = 0.01;
	// PrintDebug("Returning from transition");
}


vector<double> EkerbergControl::MuscleActivations()
{

	return muscleActivations;

}

void EkerbergControl::PrintDebug(std::string t)
{
	if (CONTROL_DEBUG)
	{
		cout << "[DEBUG] : " << t << endl;
	}
}
