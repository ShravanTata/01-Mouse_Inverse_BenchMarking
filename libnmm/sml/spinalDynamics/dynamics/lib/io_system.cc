#include <io_system.hh>
#include <link.hh>
#include <state.hh>

IOSystem::IOSystem(int dim)
{
	for(int i=0; i<dim; i++){
		//inputs.push_back(&NetworkFactory::NewState(static_cast<Network *>(this),0.0));
		inputs.push_back(new State(0.0));
	}
	for(int i=0; i<dim; i++){
		//outputs.push_back(&NetworkFactory::NewState(static_cast<Network *>(this),0.0));
		outputs.push_back(new State(0.0));
	}
}

IOSystem::IOSystem(int input_dim, int output_dim)
{
	for(int i=0; i<input_dim; i++){
		inputs.push_back(new State(0.0));
	}
	for(int i=0; i<output_dim; i++){
		outputs.push_back(new State(0.0));
	}
}
