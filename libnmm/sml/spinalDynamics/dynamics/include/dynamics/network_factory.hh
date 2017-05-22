#ifndef __NETWORK_FACTORY__HH__
#define __NETWORK_FACTORY__HH__


#include <dynamics/state.hh>
#include <dynamics/link.hh>

/**
 * @brief This class can be used to manage a Network (either the global Network or a local one)
 * @details It mainly consists of helper templated methods to be used to create and add Link or State to
 * a Network at the same time. Internally the Factory makes use of the NetworkManager class.
 * 
 * 
 * Example usage:
 * 
 * 1. State:
 * \code{.cc}
 *        State& a = NetworkFactory::NewState(1.0);                  // With the NetworkFactory
 *        State& s = NetworkManager::AddState(new State(0.0));       // With the NetworkManager
 *  
 * \endcode
 * 2. Link:
 * \code{.cc}
 *        Link& l2 = NetworkFactory::NewLink<StandardLink>(b,a);     // With the NetworkFactory
 *        Link& l1 = NetworkManager::AddLink(new StandardLink(a,b)); // With the NetworkManager
 *  
 * \endcode
 * 
 */
class NetworkFactory {
 public:
    template<typename T=State>
    static State& NewEmptyState(UInt x);
    template<typename T=State>
    static State& NewEmptyState(Network* subNet, UInt x);



    /**
     * @brief Adds a State to the global Network
     * 
     * @tparam T : the type of State (State and its children). Can be _State_ or _StateLess_
     * 
     * @param args a list of double representing the initial State
     * 
     * @return a reference to the State
     */
    template<typename T=State, typename... Args>
    static State& NewState(Real x, Args... args);

    /**
     * @brief Adds a State to a local Network
     * 
     * @tparam T : the type of State (State and its children). Can be _State_ or _StateLess_
     * 
     * @param subNet the local Network
     * @param args a list of double representing the initial State
     * 
     * @return a reference to the State
     */
    template<typename T=State, typename... Args>
    static State& NewState(Network* subNet, Args... args);

    /**
     * @brief Adds a StateLess state to the golbal Network
     * 
     * 
     * @param args a list of double representing the initial State
     * 
     * @return a reference to the State
     */
    template<typename... Args>
    static State& NewStateLess(Args... args);

    /**
     * @brief Adds a StateLess state to a local Network
     * 
     * 
     * @param subNet the local Network
     * @param args a list of double representing the initial State
     * 
     * @return a reference to the State
     */
    template<typename... Args>
    static State& NewStateLess(Network* subNet, Args... args);


    /**
     * @brief Adds a Link to the global Network
     * 
     * @tparam T : the type of Link
     * 
     * @param in a reference to the input State
     * @param out a reference to the output State
     * 
     * @return a reference to the Link
     */
    template<typename T>
    static Link& NewLink(State &in, State &out);

    /**
     * @brief Adds a Link to a local Network
     * 
     * @tparam T : the type of Link
     * 
     * @param subNet the local Network
     * @param in a reference to the input State
     * @param out a reference to the output State
     * 
     * @return a reference to the Link
     */
    template<typename T>
    static Link& NewLink(Network* subNet, State &in, State &out);
};

template<typename T>
State& NetworkFactory::NewEmptyState(UInt x){
    return NetworkManager::AddState(new T(x));
}
template<typename T>
State& NetworkFactory::NewEmptyState(Network* subNet, UInt x){
    return NetworkManager::AddState(subNet, new T(x));
}

template<typename T, typename... Args>
State& NetworkFactory::NewState(Real x, Args... args){
    return NetworkManager::AddState(new T(x, args...));
}
template<typename T, typename... Args>
State& NetworkFactory::NewState(Network* subNet, Args... args){
    return NetworkManager::AddState(subNet,new T(args...));
}
template<typename... Args>
State& NetworkFactory::NewStateLess(Args... args){
    return NetworkFactory::NewState<StateLess>(args...);
}
template<typename... Args>
State& NetworkFactory::NewStateLess(Network* subNet, Args... args){
    return NetworkFactory::NewState<StateLess>(subNet, args...);
}
template<typename T>
Link& NetworkFactory::NewLink(State &in, State &out){

    return NetworkManager::AddLink(new T(in,out));
}
template<typename T>
Link& NetworkFactory::NewLink(Network* subNet, State &in, State &out){

    return NetworkManager::AddLink(subNet,new T(in,out));
}

#endif