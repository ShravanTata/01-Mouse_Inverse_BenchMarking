#include <network.hh>
#include <link.hh>
#include <state.hh>
#include <system.hh>

Network::Network(){
	temporary_network = 0;
	clock = 0;
};
Network::Network(Time& clock){
	this->clock = &clock;
	temporary_network = 0;
};

Link& Network::addLink(Link * link){
	if (hasTemporaryNetwork())
		temporary_network->addLink(link);


	fullNet.addLink(link);

	StateLess * s = dynamic_cast<StateLess*>(&link->in);
	if (s){
		subNet_startfromlessState.addLink(link);
	}
	else
		subNet_dontstartfromlessState.addLink(link);


	link->clock = clock;
	return *link;
}
void Network::removeLink(Link * link){
	fullNet.removeLink(link);
	subNet_startfromlessState.removeLink(link);
	subNet_dontstartfromlessState.removeLink(link);
}
State& Network::addState(State * state){
	if (hasTemporaryNetwork())
		temporary_network->addState(state);

	fullNet.addState(state);

	StateLess * s = dynamic_cast<StateLess*>(state);
	if (s){
		subNet_startfromlessState.addState(state);
	}
	else
		subNet_dontstartfromlessState.addState(state);

	return *state;
}

void Network::removeState(State * state){
	fullNet.removeState(state);

	StateLess * s = dynamic_cast<StateLess*>(state);
	if (s){
		subNet_startfromlessState.removeState(state);
	}
	else
		subNet_dontstartfromlessState.removeState(state);
}

InputSystem& Network::addInputSystem(InputSystem* system){
	fullNet.addInputSystem(system);
	return *system;
}
void Network::removeInputSystem(InputSystem* system){
	fullNet.removeInputSystem(system);
}
OutputSystem& Network::addOutputSystem(OutputSystem* system){
	fullNet.addOutputSystem(system);
	return *system;
}
void Network::removeOutputSystem(OutputSystem* system){
	fullNet.removeOutputSystem(system);
}
InputOutputSystem& Network::addInputOutputSystem(InputOutputSystem* system){
	fullNet.addInputOutputSystem(system);
	return *system;
}
void Network::removeInputOutputSystem(InputOutputSystem* system){
	fullNet.removeInputOutputSystem(system);
}

void Network::update(){
	// 1) Updated system inputs
	for (auto& system : fullNet.inputOutputSystems){
		system.second->updInput();
	}
	for (auto& system : fullNet.inputSystems){
		system.second->updInput();
	}

	// 2) Save states
	for (auto& state : fullNet.states){
		state.second->save();
	}
	// 3) Update all links states
	for (auto& link : fullNet.links){
		link.second->updateState();
	}
	// 4) Reset LessStates
	for (auto& state : subNet_startfromlessState.states){
		state.second->reset();
	}
	// 5) Update all link output
	for (auto& link : fullNet.links){
		link.second->apply();
	}

	// for (auto& system : fullNet.inputOutputSystems){
	// 	system.second->updOutput();
	// }
	for (auto& system : fullNet.outputSystems){
		//std::cout << system.second << std::endl;
		system.second->updOutput();
	}
}

void Network::startTemporaryNetwork(){
	temporary_network = new Network();
}

void Network::endTemporaryNetwork(){
	delete temporary_network;
	temporary_network = 0;
}

void Network::updateTemporaryNetwork(){
	if (temporary_network)
		temporary_network->update();
}

bool Network::hasTemporaryNetwork(){
	if (temporary_network)
		return true;
	return false;
}




UInt NetworkGroup::linkNum(){
	return links.size();
}
UInt NetworkGroup::stateNum(){
	return states.size();
}

void NetworkGroup::addLink(Link * link){
	links[link->id] = link;
	linksInState[link->in.id][link->id] = link;
	linksOutState[link->out.id][link->id] = link;
}
void NetworkGroup::removeLink(Link * link){
	links.erase(link->id);
	linksInState[link->in.id].erase(link->id);
	linksOutState[link->out.id].erase(link->id);
}
void NetworkGroup::addState(State * state){
	states[state->id] = state;
}

void NetworkGroup::removeState(State * state){
	// remove the state 
	states.erase(state->id);
	// remove inward links if they are some
	if ( linksInState.find(state->id) != linksInState.end()){
		linksInState[state->id].clear();
		linksInState.erase(state->id);
	}
	// remove outward links
	if ( linksOutState.find(state->id) != linksOutState.end()){
		linksOutState[state->id].clear();
		linksOutState.erase(state->id);
	}
}


void NetworkGroup::addInputSystem(InputSystem* system){
	inputSystems[system->id] = system;
}
void NetworkGroup::removeInputSystem(InputSystem* system){
	inputSystems.erase(system->id);
}
void NetworkGroup::addOutputSystem(OutputSystem* system){
	outputSystems[system->id] = system;
}
void NetworkGroup::removeOutputSystem(OutputSystem* system){
	outputSystems.erase(system->id);
}
void NetworkGroup::addInputOutputSystem(InputOutputSystem* system){
	inputOutputSystems[system->id] = system;
}
void NetworkGroup::removeInputOutputSystem(InputOutputSystem* system){
	inputOutputSystems.erase(system->id);
}


Real Network::getTime(){
	return clock->getTime();
}

Real Network::getTimeStep(){
	return clock->getTimeStep();
}

void Network::setClock(Time& clock){
	this->clock = &clock;
};

void Network::setClock(Time* clock){
	this->clock = clock;
};

Time& Network::getClock(){
	return *clock;
};

void Network::initClock(double dt){
	clock = new Time(this);
	clock->init(dt);
}

void Network::initClock(){
	clock = new Time(this);
	clock->init();
}