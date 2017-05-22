#include "network_manager.hh"

void NetworkManager::InitClock(){
	InitClock(NetworkManager::GetNetwork());
}

void NetworkManager::InitClock(Network* network){
	network->initClock();
}

void NetworkManager::InitClock(Network& network){
	network.initClock();
}

Link&  NetworkManager::AddLink(Link* link){
	return GetNetwork().addLink(link); 
}
Link&  NetworkManager::AddLink(Network* network, Link* link){
	return network->addLink(link);
}

void   NetworkManager::RemoveLink(Link* link){
	GetNetwork().removeLink(link);
}
void   NetworkManager::RemoveLink(Network* network, Link* link){
	network->removeLink(link);
}

State& NetworkManager::AddState(State* state){
	return GetNetwork().addState(state);
}
State& NetworkManager::AddState(Network* network, State* state){
	return network->addState(state);
}

void   NetworkManager::RemoveState(State* state){
	GetNetwork().removeState(state);
}
void   NetworkManager::RemoveState(Network* network, State* state){
	network->removeState(state);
}

InputSystem& NetworkManager::AddInputSystem(InputSystem * system){
	return GetNetwork().addInputSystem(system);
}
InputSystem& NetworkManager::AddInputSystem(Network * network, InputSystem * system){
	return network->addInputSystem(system);
}
OutputSystem& NetworkManager::AddOutputSystem(OutputSystem * system){
	return GetNetwork().addOutputSystem(system);
}
OutputSystem& NetworkManager::AddOutputSystem(Network * network, OutputSystem * system){
	return network->addOutputSystem(system);
}
InputOutputSystem& NetworkManager::AddInputOutputSystem(InputOutputSystem * system){
	return GetNetwork().addInputOutputSystem(system);
}
InputOutputSystem& NetworkManager::AddInputOutputSystem(Network * network, InputOutputSystem * system){
	return network->addInputOutputSystem(system);
}

void NetworkManager::RemoveInputSystem(InputSystem * system){
	GetNetwork().removeInputSystem(system);
}
void NetworkManager::RemoveInputSystem(Network * network, InputSystem * system){
	network->removeInputSystem(system);
}
void NetworkManager::RemoveOutputSystem(OutputSystem * system){
	GetNetwork().removeOutputSystem(system);
}
void NetworkManager::RemoveOutputSystem(Network * network, OutputSystem * system){
	network->removeOutputSystem(system);
}
void NetworkManager::RemoveInputOutputSystem(InputOutputSystem * system){
	GetNetwork().removeInputOutputSystem(system);
}
void NetworkManager::RemoveInputOutputSystem(Network * network, InputOutputSystem * system){
	network->removeInputOutputSystem(system);
}


void   NetworkManager::Update(){ GetNetwork().update(); }


void   NetworkManager::StartTemporaryNetwork(){ 
	GetNetwork().startTemporaryNetwork(); }
void   NetworkManager::EndTemporaryNetwork(){
	GetNetwork().endTemporaryNetwork(); }
void   NetworkManager::UpdateTemporaryNetwork(){
	if ( GetNetwork().hasTemporaryNetwork() )
		GetNetwork().updateTemporaryNetwork();
}
Network& NetworkManager::GetNetwork(){
	static Network network = Network();
	return network;
}