#include <OpenSim/OpenSim.h>

#include <setting-manager/parameters.hh>
#include <controller.hh>

using namespace SimTK;
using namespace OpenSim; using OpenSim::Body;

/**
 * @brief Helper structure to get access to different model components of an open sim model
 */
struct ModelExt {
	/**
	 * @brief returns the OpenSim::Body reference corresponding to the name passed in parameter
	 * 
	 * @param model a reference to the model
	 * @param name the name of the link to returns
	 * 
	 * @return the OpenSim::Body reference
	 */
	static OpenSim::Body& getLink(Model& model, std::string name);
	/**
	 * @brief returns the PinJoint reference corresponding to the name passed in parameter
	 * 
	 * @param model a reference to the model
	 * @param name the name of the joint to returns
	 * 
	 * @return the OpenSim::PinJoint reference
	 */
	static const PinJoint& getJoint(Model& model, std::string name);
	/**
	 * @brief returns the OpenSim::Muscle reference corresponding to the name passed in parameter
	 * 
	 * @param model a reference to the model
	 * @param name the name of the muscle to returns
	 * 
	 * @return the OpenSim::Muscle reference
	 */
	static OpenSim::Muscle& getMuscle(Model& model, std::string name);
	/**
	 * @brief Save the simulation results into the file filename
	 * 
	 * @param manager a reference to the current Manager
	 * @param filename the name of the file where to save the simulation results 
	 */
	static void saveResults(Manager& manager, std::string filename);
	/**
	 * @brief Save the current model to the file filename
	 * 
	 * @param model a reference to the current Model
	 * @param filename  the name of the file where to save the model
	 */
	static void saveModel(Model& model, std::string filename);
};

/**
 * @brief Class used to control an openSim model
 * 
 */
class ModelBase {
public:
	Model model;
	State* state;
	/**
	 * @brief Parameters associated to the model
	 */
	Parameters parameters;
	Controller* controller;
	/**
	 * @brief ModelBase constructor.
	 * @details The constructor is used to set the parameters associated to a model
	 */
	ModelBase();
	template <typename T>
	void cfg(std::string str, T val){ parameters.get<T>(str) = val; };
	/**
	 * @brief Initialise the model
	 */
	void init();
	/**
	 * @brief Simulate the model
	 */
	void simulate();

protected:

	void updBackground( Visualizer::BackgroundType );
	void saveModel(std::string );
	void saveSimulation( );

	virtual void init_parameters(){}
	virtual void init_body(){
			model.setGravity(Vec3(0,0,0));

	}
	virtual void init_visualization(){}
	virtual void init_controller(){}
	virtual void init_post_InitSystem(){}
	virtual void simulate_pre(){}
	virtual void simulate_post(){}

    State& getState(){
    	return *state;
    }


};


class Cube : public ModelBase {
public:
	using ModelBase::ModelBase;

private:
	void init_parameters();
	void init_body();
	void init_controller();
	void init_visualization();
	void init_post_InitSystem();
};



class Arm1Dof2 : public ModelBase {
public:
	using ModelBase::ModelBase;

private:
	void init_body();
	void init_controller();
	void init_visualization();
	void init_post_InitSystem();
};

