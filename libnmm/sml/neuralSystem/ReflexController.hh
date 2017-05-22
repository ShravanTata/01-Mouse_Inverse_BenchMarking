#pragma once
#ifndef __ReflexController_HH__
#define __ReflexController_HH__

#include <sml/sml-tools/Helper.hh>
#include <setting-manager/parameters.hh>
#include <dynamics/controller.hh>


class Sml;

class ReflexController : public Controller {

public:
	//using Controller::Controller;
	ReflexController();
	Sml* body;

	void update();

private:
	int time_step;
    void step_INsen_to_IN();
    void step_INcpg_to_IN(); 
    void step_IN_to_MN();
    void step_MN_to_output();

    double get_IN_contribution(int IN,bool cpg);
	void step_IN_to_MN(array2d<Connection*,SENSORY_INTERNEURON::NUMBER,MUSCLES::NUMBER> __IN_MN,bool cpg);

	void initCpgParameters();
	void initialiseFeedbacks(SIDE::Side side);


	void applyGateNeuron(Interneuron * interneuron);
	std::string getLimbState_str(SIDE::Side side);
    CYCLE_PHASE::CyclePhase getLimbState(SIDE::Side side);

public:
	bool isGateOpen(Interneuron * interneuron);
	bool feedback_right_loaded;
	bool feedback_left_loaded;
	bool cpg_right_loaded;
	bool cpg_left_loaded;
	void loadPassiveFeedback();
	void loadCpgFile();

	void loadFeedback(SIDE::Side side);
	void loadCpg(SIDE::Side side);
	void loadPartialCpg(SIDE::Side side);
	void initialiseFeedbacks();
	void initialiseMotoneurons();
	void initialiseCpgs(std::map<std::string, Sensor *> sensors);


public:

    std::array<Entity*,SENSORY_INTERNEURON::NUMBER> INsens;
    std::array<Entity*,SENSORY_INTERNEURON::NUMBER> INcpgs;
    std::array<Motoneuron *, MUSCLES::NUMBER> motoneurons;
    
    array2d<Connection*,SENSORY_INTERNEURON::NUMBER,MUSCLES::NUMBER> __INsen_MN;
    array2d<Connection*,SENSORY_INTERNEURON::NUMBER,MUSCLES::NUMBER> __INcpg_MN;
    
    
	std::vector<std::string> modelized_sensory_interneurons;

};





#endif