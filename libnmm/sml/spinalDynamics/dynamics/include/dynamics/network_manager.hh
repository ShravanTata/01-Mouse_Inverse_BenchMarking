#ifndef __NETWORK_MANAGER__HH__
#define __NETWORK_MANAGER__HH__

#include <unordered_map>
#include <vector>

#include <dynamics/config.hh>
#include <dynamics/network.hh>

class State;
class Link;


/**
 * @brief static Class used to manage a Network of States and Link
 * @details this class can be used when only one global Network is used in the whole application.
 * 
 * Note that the NetworkFactory class has facilities for 
 * creating Link in the global Network as well as in a local Network
 * 
 */
class NetworkManager {
 public:

  /**
   * @brief Creates the clock of the global network
   * @details T
   */
  static void InitClock();
  /**
   * @brief Creates the clock of the network passed in parameters
   * 
   * @param subNet A pointer to the network on which a clock should be  creaed and initialized
   */
  static void InitClock(Network * subNet);
  /**
   * @brief Creates the clock of the network passed in parameters
   * 
   * @param subNet A reference to the network on which a clock should be  creaed and initialized
   */
  static void InitClock(Network & subNet);
  /**
   * @brief Add a Link to the global Network
   * 
   * @param link the Link to be added to the global Network
   * @return a reference to the Link added in the global Network
   */
  static Link& AddLink(Link * link);
  /**
   * @brief Add a Link to a local Network
   * 
   * @param subNet a pointer to the local Network
   * @param link the Link to be added to the local Network
   * 
   * @return a reference to the Link added in the global Network
   */
  static Link& AddLink(Network * subNet, Link * link);
  /**
   * @brief Remove a Link from the global Network
   * 
   * @param link the Link to be removed from the global Network
   */
  static void RemoveLink(Link * link);
  /**
   * @brief Remove a Link from a local Network
   * 
   * @param subNet a pointer to the local Network
   * @param link the Link to be removed from the local Network
   */
  static void RemoveLink(Network * subNet, Link * link);
  /**
   * @brief Add a State to the global Network
   * 
   * @param state the State to be added to the global Network
   * @return a reference to the State added in the global Network
   */
  static State& AddState(State * state);
  /**
   * @brief Add a State in a local Network
   * 
   * @param subNet a pointer to the local Network
   * @param state the State to be added from the local Network
   */
  static State& AddState(Network * subNet, State * state);
  /**
   * @brief Remove a State from the global Network
   * 
   * @param state the State to be removed from the global Network
   */
  static void RemoveState(State * state);
  /**
   * @brief Remove a State from a local Network
   * 
   * @param subNet a pointer to the local Network
   * @param state the State to be removed from the local Network
   */
  static void RemoveState(Network * subNet, State * state);

  static InputSystem& AddInputSystem(InputSystem * system);
  static InputSystem& AddInputSystem(Network * subNet, InputSystem * system);
  static void RemoveInputSystem(InputSystem * system);
  static void RemoveInputSystem(Network * subNet, InputSystem * system);

  static OutputSystem& AddOutputSystem(OutputSystem * system);
  static OutputSystem& AddOutputSystem(Network * subNet, OutputSystem * system);
  static void RemoveOutputSystem(OutputSystem * system);
  static void RemoveOutputSystem(Network * subNet, OutputSystem * system);

  static InputOutputSystem& AddInputOutputSystem(InputOutputSystem * system);
  static InputOutputSystem& AddInputOutputSystem(Network * subNet, InputOutputSystem * system);
  static void RemoveInputOutputSystem(InputOutputSystem * system);
  static void RemoveInputOutputSystem(Network * subNet, InputOutputSystem * system);

  /**
   * @brief Update the global Network.
   * @details All current states are saved in their internal memory and links are updated.
   * 
   * Local Network can be updated using their own update method
   */
  static void Update();

  /**
   * @brief Starts a temporary Network associated to the global Network. This network can be used for initialization purpose
   * @details As soon as a temporary Network is started all State and Link added to the network 
   * are also added in the temporary Network (see the Network class for more information).
   * The temporary Network can be deleted with the EndTemporaryNetwork() method
   * The temporary Network can be updated on its own with the UpdateTemporaryNetwork() method
   */
  static void StartTemporaryNetwork();
  /**
   * @brief Deletes the temporary Network associated to the global Network if present.
   */
  static void EndTemporaryNetwork();
  /**
   * @brief Starts a temporary Network that can be used for initialization purpose
   */
  static void UpdateTemporaryNetwork();

  /**
   * @brief Returns a reference to the global Network that can then be used as a normal Network.
   */
  static Network& GetNetwork();
};


#endif