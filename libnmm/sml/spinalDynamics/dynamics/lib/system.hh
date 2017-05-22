#ifndef __SYSTEM__HH__
#define __SYSTEM__HH__

#include <network.hh>
#include <indexed.hh>
/**
 * @brief class used to manage a system.
 * @details A system has the role of encapsulating a 
 * functional part of a network. 
 * This functonal part can be either part of an existing network
 * or can work on its on network.
 * 
 * System has 3 constructor.
 * 
 * The default ones uses the global singleton network object 
 * (returned by NetworkManager::Network() )
 * The two other one uses an existing network passed by pointer
 * or reference.
 * 
 */
class System {
public:
	Network* network;
  /**
   * @brief Default constructor
   * @details Uses the global network stored in the NetworkManager class
   */
  System();
  /**
   * @brief Constructor takes as param  the network on which the system will be added
   * 
   * @param network pointer to the network
   */
  System(Network* network);
  /**
   * @brief Constructor takes as param  the network on which the system will be added
   * 
   * @param network reference to the network
   */
  System(Network& network);

  /**
   * @brief Returns the output of the system
   * 
   * @param i The output to return
   * @return The value
   */
  virtual Real getOutput(int i);
  /**
   * @brief Returns a reference to the output of the system
   * 
   * @param i The output to return
   * @return The reference
   */
  virtual Real& updOutput(int i);
  /**
   * @brief Returns the input of the system
   * 
   * @param i The input to return
   * @return The value
   */
   virtual Real getInput(int i);
  /**
   * @brief Returns a reference to the input of the system
   * 
   * @param i The input to return
   * @return The reference
   */
   virtual Real& updInput(int i);



//protected:
protected:
  std::vector<Real*> inputs;
  std::vector<Real*> outputs;
};

class Time: public System{
public:
  using System::System;
  State* x;
  Link* l;

  void init();
  void init(Real dt);
  /**
   * @brief Returns the current time
   * @return The current time
   */
  Real getTime() const;
  /**
   * @brief Returns the current time_step
   * @return The current time_step
   */
  Real getTimeStep() const;

};

class InputSystem : public System, public Indexed{
public:
  InputSystem();
  InputSystem(Network* network);
  InputSystem(Network& network);
   virtual void updInput() = 0;
};

class OutputSystem : public System, public Indexed{
public:
  OutputSystem();
  OutputSystem(Network* network);
  OutputSystem(Network& network);
   virtual void updOutput() = 0;
};

class InputOutputSystem : public System, public Indexed{
public:
  InputOutputSystem();
  InputOutputSystem(Network* network);
  InputOutputSystem(Network& network);
   virtual void updInput() = 0;
   virtual void updOutput() = 0;
};



class Impulse: public OutputSystem{

public:
	using OutputSystem::OutputSystem;
	State* x;
	Link* l;

	void init(Real&, Real start_time);
  void init(Real&, Real start_time,Real duration,Real amplitude,Real time_constant);

  void updOutput();
  Real getOutput(int i);

};


#endif