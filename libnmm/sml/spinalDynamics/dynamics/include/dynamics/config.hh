#ifndef __CONFIG__HH__
#define __CONFIG__HH__

// #pragma GCC diagnostic ignored "-Wunused-parameter"
// #pragma GCC diagnostic ignored "-Wweak-vtables"
// #pragma GCC diagnostic ignored "-Wc++98-compat-pedantic"
// #pragma GCC diagnostic ignored "-Wnewline-eof"
// #pragma GCC diagnostic ignored "-Wglobal-constructors"
// #pragma GCC diagnostic ignored "-Wdeprecated"
// #pragma GCC diagnostic ignored "-Wpadded"
// #pragma GCC diagnostic ignored "-Wextra-semi"
// #pragma GCC diagnostic ignored "-Wfloat-equal"
// #pragma GCC diagnostic ignored "-Wexit-time-destructors"


#include <unordered_map>
#include <vector>

class Link;
class State;
class InputSystem;
class OutputSystem;
class InputOutputSystem;

typedef double Real;
typedef unsigned long int UInt;
typedef long int Int;

/**
 * @brief unordered map of Link pointers. Values are Link pointer and Keys are associated unique id.
 */
typedef std::unordered_map<UInt, Link*> LinkGroup;
/**
 * @brief unordered map State pointers. Values are Link pointer and Keys are associated unique id.
 */
typedef std::unordered_map<UInt, State*> StateGroup;
/**
 * @brief unordered map of LinkGroup. Values are LinkGroup and Keys are associated State id.
 * @details this map is used to associate State and list of links. In pratice it is used
 * to record all Link starting from a State i.e. NetworkGroup::linksInState or pointing to a State linksOutState
 */
typedef std::unordered_map< UInt, LinkGroup> StateLinkGroupMap;

typedef std::unordered_map<UInt, InputSystem*> InputSystemGroup;
typedef std::unordered_map<UInt, OutputSystem*> OutputSystemGroup;
typedef std::unordered_map<UInt, InputOutputSystem*> InputOutputSystemGroup;

/**
 * @brief Fills a Real vector with the list of values passed as parameters
 * @details given a Vector X the method can be called as follow : fillVector(X, 1.0, 2.0, 3.0);
 * 
 * @param X The Vector to be filled
 * @param t The value to add
 */

 template<class T>
inline void fillVector(T &X, double t)
{
    X.push_back(t);
}
/**
 * @brief Fills a Real vector with the list of values passed as parameters
 * @details given a Vector X the method can be called as follow : fillVector(X, 1.0, 2.0, 3.0);
 * 
 * @param X The Vector to be filled
 * @param t The current value to add
 * @param args all other values
 */
template<class T, typename... Args>
inline void fillVector(T &X, double t, Args... args) // recursive variadic function
{
    X.push_back(t);
    fillVector(X,args...);
}


#define TIME_STEP 0.03
#define MEMORY 30
#define EPSILON 0.0000000001


#endif