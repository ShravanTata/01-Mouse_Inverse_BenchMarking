#include "config.hh"
#include "system.hh"
#include "network_manager.hh"
#include "network_factory.hh"
#include "link_library.hh"

System::System(){
	this->network = &NetworkManager::GetNetwork();
};
System::System(Network* network){
	this->network = network;
};
System::System(Network& network){
	this->network = &network;
};


OutputSystem::OutputSystem() : System(){
	NetworkManager::AddOutputSystem(&NetworkManager::GetNetwork(),this);
};
OutputSystem::OutputSystem(Network* network) : System(network){
	NetworkManager::AddOutputSystem(network,this);
};
OutputSystem::OutputSystem(Network& network) : System(network){
	NetworkManager::AddOutputSystem(&network,this);
};
InputSystem::InputSystem() : System(){
	NetworkManager::AddInputSystem(&NetworkManager::GetNetwork(),this);
};
InputSystem::InputSystem(Network* network) : System(network){
	NetworkManager::AddInputSystem(network,this);
};
InputSystem::InputSystem(Network& network) : System(network){
	NetworkManager::AddInputSystem(&network,this);
};

InputOutputSystem::InputOutputSystem() : System(){
	NetworkManager::AddInputOutputSystem(&NetworkManager::GetNetwork(),this);
};
InputOutputSystem::InputOutputSystem(Network* network) : System(network){
	NetworkManager::AddInputOutputSystem(network,this);
};
InputOutputSystem::InputOutputSystem(Network& network) : System(network){
	NetworkManager::AddInputOutputSystem(&network,this);
};



Real System::getOutput(int i){
	std::cout << "not implemented" << std::endl;
	return 0;
};
Real& System::updOutput(int i){
	std::cout << "not implemented" << std::endl;
	static Real k=0.0; return k;
};
Real System::getInput(int i){
	std::cout << "not implemented" << std::endl;
	return 0.0;
};
Real& System::updInput(int i){
	std::cout << "not implemented" << std::endl;
	static Real k=0.0; return k;
};



void Time::init(){
	init(TIME_STEP);
}


void Time::init(Real dt){
	x = new State(0.0);
	l = new ConstantLink(*x,*x);
	(*l).link[0] = dt; //updOutput(Output::TimeStep) = dt;
	(*x)[0] = 0.0; //updOutput(Output::Time) = 0.0;
	NetworkManager::AddState(network,x);
	NetworkManager::AddLink(network,l);
	// All link objects have a pointer to the Network time 
	// object they belong to. This pointer is set when
	// the link is added to the network.
	// The Time object is a system and has therefore a link
	// This links needs access to the clock object
	// Which is not yet part of the network
	// We therefore set the clock pointer of the link l 
	// to the Time object address.
	(*l).clock = this;
}

Real Time::getTime() const{
	return (*x)[0];
}

Real Time::getTimeStep() const{
	return (*l).link[0];
}





void Impulse::init(Real& output, Real start_time)
{
	outputs.push_back(&output);


	x = &(NetworkFactory::NewState(network,0.0));
	l = &(NetworkFactory::NewLink<LinkImpulse>(network,*x, *x));
	l->parameters.get<double>("impulse_start_time") = start_time;
	l->parameters.get<double>("impulse_time_constant") = 10;
	l->parameters.get<double>("impulse_amplitude") = 1;
	l->parameters.get<double>("impulse_duration") = 0.1;
}
void Impulse::init(Real& output, Real start_time,Real duration,Real amplitude,Real time_constant)
{
	outputs.push_back(&output);

	x = &(NetworkFactory::NewState(network,0.0));
	l = &(NetworkFactory::NewLink<LinkImpulse>(network,*x, *x));
	l->parameters.get<double>("impulse_start_time") = start_time;
	l->parameters.get<double>("impulse_duration") = duration;
	l->parameters.get<double>("impulse_amplitude") = amplitude;
	l->parameters.get<double>("impulse_time_constant") = time_constant;
}


Real Impulse::getOutput(int i){
	return (*x)[0];
}

void Impulse::updOutput(){
	*(outputs[0]) = getOutput(0);
};