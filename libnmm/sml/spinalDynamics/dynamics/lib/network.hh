#ifndef __NETWORK__HH__
#define __NETWORK__HH__


#include <config.hh>

class Link;
class State;
class Time;
class InputSystem;
class OutputSystem;
class InputOutputSystem;

/**
 * @brief a sub part of a whole Network.
 * @details this is used by the Network class to store
 * different part of a network. This can be used
 * to seperate special part of a network.
 * For example StateLess state (that need to be reset after
 * links have been updated and before links affect their output)
 * are on specific NetworkGroup in the Network class called
 * subNet_startfromlessState.
 *
 */
class NetworkGroup{
friend class NetworkManager;
public:
  /**
   * @brief Number of states in the group
   */
  UInt stateNum();
  /**
   * @brief Number of links in the group
   */
  UInt linkNum();
public: // TODO : change to private
  /**
   * @brief Map of all links in the NetworkGroup
   */
  LinkGroup links;
  /**
   * @brief Map of all states in the NetworkGroup
   */
  StateGroup states;
  /**
   * @brief Map recording all the link starting from each State
   * @details In other words. Given a State id, this maps returns a LinkGroup of all links starting from this State
   */
  StateLinkGroupMap linksInState;
  /**
   * @brief Map recording all the link pointing to a State
   * @details In other words. Given a State id, this maps returns a LinkGroup of all links pointing to this State
   */
  StateLinkGroupMap linksOutState;

  InputSystemGroup inputSystems;
  OutputSystemGroup outputSystems;
  InputOutputSystemGroup inputOutputSystems;
  /**
   * @brief Add a Link to the NetworkGroup
   */
  void addLink(Link * link);
  /**
   * @brief Remove a Link from the NetworkGroup
   */
  void removeLink(Link * link);
  /**
   * @brief Add a State to the NetworkGroup
   */
  void addState(State * state);
  /**
   * @brief Remove a State from the NetworkGroup
   */
  void removeState(State * state);

  void addInputSystem(InputSystem* system);
  void removeInputSystem(InputSystem* system);
  void addOutputSystem(OutputSystem* system);
  void removeOutputSystem(OutputSystem* system);
  void addInputOutputSystem(InputOutputSystem* system);
  void removeInputOutputSystem(InputOutputSystem* system);
};


/**
 * @brief The Network class.
 */
class Network {
  friend class NetworkManager;
  friend class NetworkFactory;
public:
  /**
   * @brief Default constructor
   * @details Creates a new network
   */
  Network();
  /**
   * @brief Constructor taking a reference to Time object.
   * @details This constructor can be used to specify a Time object to be used 
   * as the network time.
   * 
   * @param time reference to the Time object to be used as time manager.
   */
  Network(Time& time);


  virtual ~Network(){}
  Link& addLink(Link * link);
  void removeLink(Link * link);
  State& addState(State * state);
  void removeState(State * state);
  InputSystem& addInputSystem(InputSystem* system);
  void removeInputSystem(InputSystem* system);
  OutputSystem& addOutputSystem(OutputSystem* system);
  void removeOutputSystem(OutputSystem* system);
  InputOutputSystem& addInputOutputSystem(InputOutputSystem* system);
  void removeInputOutputSystem(InputOutputSystem* system);

public:
  /**
   * @brief Update the Network
   * @details The Network update is a 4 stages process
   * 1. Save all states 
   * 2. Update all links states
   * 3. Reset StateLess State
   * 4. Update all link output

\code{.cc}
  // 1) Save states
  for (auto& state : fullNet.states){
    state.second->save();
  }
  // 2) Update all links states
  for (auto& link : fullNet.links){
    link.second->updateState();
  }
  // 3) Reset LessStates
  for (auto& state : subNet_startfromlessState.states){
    state.second->reset();
  }
  // 4) Update all link output
  for (auto& link : fullNet.links){
    link.second->apply();
  }
\endcode
   */
  void update();

  /**
   * @brief Starts a temporary Network. Usefull for initialization purpose
   */
  void startTemporaryNetwork();
  /**
   * @brief Ends a temporary Network
   */
  void endTemporaryNetwork();
  /**
   * @brief Update the temporary Network
   */
  void updateTemporaryNetwork();
  /**
   * @brief Returns true if a temporary Network is started
   */
  bool hasTemporaryNetwork();

  /**
   * NetworkGroup containing the whole Network
   */
  NetworkGroup fullNet;
  /**
   * NetworkGroup containing the sub part of the network that have link that starts from StateLess
   */
  NetworkGroup subNet_startfromlessState;
  /**
   * NetworkGroup containing the sub part of the network that does not have link that starts from StateLess
   */
  NetworkGroup subNet_dontstartfromlessState;

public:
  /**
   * The temporary network 
   */
  Network* temporary_network;

private:
  /**
   * Time
   */
  Time* clock;

public:
  /**
   * @brief Set the clock to be used by the network
   * @details By default a network uses the global singleton time object returned by Time::getSingleton()
   * 
   * @param clock reference to the Time object to be used as clock.
   */
  void setClock(Time& clock);
  /**
   * @brief see setClock(Time& clock) for details.
   * @param clock pointer to the Time object to be used as clock.
   */
  void setClock(Time* clock);
  /**
   * @brief Returns a reference to the clock used by the network
   * @details [long description]
   * @return A reference to the clock
   */
  Time& getClock();
  /**
   * @brief initialize the clock with default time step (#define TIME_STEP)
   */
  void initClock();
  /**
   * @brief Initialize the clock with user defined time step 
   * @details [long description]
   * 
   * @param dt the time step to be used in seconds
   */
  void initClock(double dt);
  /**
   * @brief Returns the current time
   */
  Real getTime();
  /**
   * @brief Returns the current time step
   */
  Real getTimeStep();
};



#endif