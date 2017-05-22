#ifndef __CONTROLLER__HH__
#define __CONTROLLER__HH__

#include <setting-manager/parameters.hh>
#include <dynamics/network.hh>
#include <dynamics/system.hh>

/**
 * @brief Interface class used to implement controllers
 * @details A controller is implemented as general as possible : 
 * i.e. a black box with inputs and output parameters and a set of internal parameters
 * all stored as a Parameters class (from the setting-manager library).
 * Therefore the controller does not constraint in any way the settings
 * of the inputs / outputs and internal controller parameters, that is 
 * the business of the implementation.
 */
class Controller {

public:
	/**
	 * @brief used to store parameters used by the controller
	 */
	Parameters param;


	Controller(){}

	/**
	 * @brief used to set Inputs
	 * 
	 * @param s the string indentifing the input
	 * @param val a reference to the input value
	 * @tparam T=double type of the parameter
	 */
	template<typename T=double>
	void setInput(const std::string s, T& val){
		inputs.get<const T*>(s) = &val;
	}
	/**
	 * @brief used to set Outputs
	 * 
	 * @param s the string indentifing the output
	 * @tparam T=double type of the parameter
	 * @return a reference to the output
	 */
	template<typename T=double>
	void setOutput(const std::string s, T& val){
		outputs.get<T*>(s) = &val;
	}
	/**
	 * @brief used to get Inputs
	 * 
	 * @param s the string indentifing the input
	 * @tparam T=double type of the parameter
	 * @return a const reference to the input
	 */
	template<typename T=double>
	const T& getInput(const std::string s){
		return *inputs.get<const T*>(s);
	}
	/**
	 * @brief used to get Outputs
	 * 
	 * @param s the string indentifing the output
	 * @tparam T=double type of the parameter
	 * @return a reference to the output
	 */
	template<typename T=double>
	T& getOutput(const std::string s){
		return *outputs.get<T*>(s);
	}

	/**
	 * @brief updates the controller. To be used in the loop
	 */
	virtual void update() = 0;

public:
	/**
	 * @brief used to store inputs variable. 
	 * Input variables are set / get with the setInput / getInput
	 */
	Parameters inputs;
	/**
	 * @brief used to store output variables.
	 * Output variables are set / get with the setInput / getInput
	 */
	Parameters outputs;
};

/**
 * @brief Implementation of a twitching controller
 * @details A twitching controller is composed of a network of impulses.
 */
class TwitchingController : public Controller {
public:
	using Controller::Controller;
	void init();
	void update();
	void update_WithoutDynamics();

	Network* network;
    std::vector<Impulse> impulses; 
};



#endif