#ifndef __LINK__HH__
#define __LINK__HH__
#include <dynamics/indexed.hh>
#include <dynamics/state.hh>
#include <dynamics/network_manager.hh>
#include <setting-manager/parameters.hh>
#include <map>


namespace Input2Link{ class Base; }
namespace LinkProcessing{ class Base; }
namespace Link2Output{ class Base; }


/**
 * @brief The Link class. Used to model the link between two States.
 * @details 
 * 
 * # General overview
 * 
 * The Link has 2 references to the input and output states and a link member which represents
 * the State of the link.
 * 
 * The behavior of the link is controlled by three members : input2link, linkProcessing, link2output.
 * When a Network is updated all the Links present in the network are updated. 
 * When a Link is updated the following process occurs : 
 *
 *   - First the state of the Link is updated and processed
 *      This is done by calling the updateState method which in turns calls the process method of the linkProcessing object
        This method updates the Link (calls the apply method of input2Link object) and process it
 *   - Second the Link's state affects the output State
 * 
 * This 2 step process is handled by 3 objects inheriting from : Input2Link::Base, LinkProcessing::Base, Link2Output::Base respectively.
 * All children classes are defined in the dynamics/link_library.hh header. The Base classes are defined in the dynamics/link.hh header.
 * 
 *   - The object of the class _Input2Link_ defines the Function mapping the input State to the link State
 *      - Input2Link::Empty
 *      - Input2Link::Constant
 *      - Input2Link::Absolute
 *      - Input2Link::Inverse
 *      - Input2Link::Difference
 *      - Input2Link::Delay
 *      - Input2Link::Linear 
 *   - The object of the class _LinkProcessing_ defines how the link State is processed : 
 *      - LinkProcessing::Constant : calls the updateFunction method of the class Link
 *      - LinkProcessing::Euler : calls the updateFunction method of the class Link to update the state of the link and multiply the resultant link state by dt
 *      - LinkProcessing::RungeKutta
 *   - The object of the class _Link2Output_ defines how the link State is mapped to the output State :
 *      - Link2Output::Setter : the output State is overriden by the current link State
 *      - Link2Output::Adder : the link State is added to the current output State 
 *   
 * # Parameterization
 * 
 * All links inherits from the Parametrized class (defined in the Settings&Parameter Manager library.
 * This inheritance adds a field parameters to the object deriving from Link with a set of default parameters
 * that can be used to parametrize the link.
 * 
 * Example with link using the Input2Link::Delay object.
 * 
 *    This class is used to add a delay between two States. The input parameter we expect to be set
 *    by the user  is the duration of the delay. This can be easily done thanks to the parametrization mechanism.
 *
 *    > Note that the parameter used to control the behavior of a link should be documented by the developer.
 *    
 *    
 *    The method defining the Input2Link::Delay looks like this :
 *    
 *    \code{.cc}
 *    void Input2Link::Delay::apply(Link & link, const State & in, Real t){
 *    State& out = link.out;
 *    UInt& p0 = link.parameters.get<UInt>("p0"); // Here the parameter "p0" controls the delay.
 *    for(UInt i=0; i<link.in.dimension(); i++)
 *      link.link[i] = p0*in.transferFunction(i,p0);
 *    }
 *    \endcode
 *    
 * 
 * # Implementation of a Link subclass
 * 
 * The creation of a Link is a two stage process. 
 * 
 * 1. Choose / ( Create if needed ) the _Input2Link_ _LinkProcessing_ and _Link2Output_ classes 
 * 
 * 2. Create the Link class
 *    Two examples of Link are given below the LinkI1 which implements a simple integrator and the LinkD1 which
 *    implements a simple differentiator.
 *    
 *    - Integrator: given an input x a integrator is defined as y(t+dt) = y(t) + dt*(K*x(t)).
 *      We want to set the link state to the state of the input (multiplied by a constant) we therefore use an Input2Link::Linear 
 *      We want to multiply the state of the link by the time step we therefore use a LinkProcessing::Euler
 *      We want to add the state of the link to the output state we therefore use a Link2Output::Adder
 *      
        \code{.cc} 
         LinkI1::LinkI1(State &in, State& out) : Link(in, out){
          setInput2Link<Input2Link::Linear>();
          setLink2Output<Link2Output::Adder>();
          setLinkProcessing<LinkProcessing::Euler>();
        }
        \endcode
 *    - Differentiator: given an input x a differentiator is defined as y(t+dt) = y(t) + K*(x(t)-x(t-dt))/dt .
 *    
 *      We want to set the link state to the (x(t)-x(t-dt))/dt (multiplied by a constant) we therefore use an Input2Link::Difference 
 *      We dont want to process anything so we use a LinkProcessing::Constant
 *      We want to add the state of the link to the output state we therefore use a Link2Output::Adder
 *      
        \code{.cc} 
          LinkD1::LinkD1(State &in, State& out) : Link(in, out){
            setInput2Link<Input2Link::Difference>();
            setLink2Output<Link2Output::Adder>();
            setLinkProcessing<LinkProcessing::Constant>();
          }
        \endcode

 */
class Link : public Indexed, public Parametrized
{
  friend class NetworkManager;
  friend class Network;


public:

  /**
   * @brief Links reference constructor
   */
  Link(State& in, State& out);
/**
   * @brief Links pointer constructor
   */
  Link(State* in, State* out);

  /**
   * @brief creates the input2link object of type T
   * 
   * @tparam T The Input2Link to use for this link
   */
  template <typename T>
  void setInput2Link();
  /**
   * @brief creates the linkProcessing object of type T
   * 
   * @tparam T The LinkProcessing to use for this link
   */
  template <typename T=LinkProcessing::Base>
  void setLinkProcessing();
  /**
   * @brief creates the link2output object of type T
   * 
   * @tparam T The Link2Output to use for this link
   */
  template <typename T>
  void setLink2Output();

  /**
   * @brief This method calls the apply method of the Input2Link
   * 
   * @param in the input state
   * @param t the current time
   */
  void updateFunction(const State & in, const Real& t );

  /**
   * @brief Method used to process the link. 
   * @details The method calls the process method of the linkProcessing class which 
   * first update the state of the Link with the apply method of the Input2Link class 
   * and then process the input based on the LinkProcessing type
   */
  void updateState();
  /**
   * @brief Method used to apply the link to the output
   */
  void apply();

  virtual ~Link();

  /* ------------------------------------------------------------------------ */
  /* Members                                                                  */
  /* ------------------------------------------------------------------------ */
public:
  /**
   * Reference to the input State
   */
  State& in;
  /**
   * Reference to the output State
   */
  State& out;
  /**
   * state of the Link
   */
  State link;



private:

  Input2Link::Base* input2link;
  LinkProcessing::Base* linkProcessing;
  Link2Output::Base* link2output;

public:
  /**
   * The clock that give the time to the link.
   * By default the clock used is the global clock
   * returned by the NewTime::GetSingleton static method
   * and belonging to the global network returned by the 
   * NetworkManager::GetNetwork static method.
   */
  const Time *clock;
};



namespace Input2Link{
/**
 * @brief Base class of the Input2Link family
 */
class Base{
  public:
    /**
     * @brief Method mapping the input state to the link state
     * @details this method is called by the Link::updateFunction methods
     * 
     * @param link the current Link
     * @param in the input state
     * @param t the current time
     */
    virtual void apply(Link & link, const State & in, Real t) = 0;
    virtual ~Base();

};
}



namespace LinkProcessing {
/**
 * @brief Base class of the LinkProcessing family
 */
class Base {
  public:
    /**
     * @brief The method calls the Link::updateFunction method to updated the link's State and process the output
     * @details The method is called by the updateState method in the Network::update method.
     * @param link a reference to the current link to process
     */
    virtual void process(Link & link) = 0;
    virtual ~Base();
};
}

namespace Link2Output {
class Base {
/**
 * @brief Base class of the Link2Output family
 */
  public: 
    /**
     * @brief The method is called by the Link::apply method and updates the output State out based on the current link's State
     * 
     * @param link a reference to the current link
     */
    virtual void apply(Link & link) = 0;
    virtual ~Base();
};
}


template <typename T>
void Link::setLinkProcessing(){
  linkProcessing = new T();
}

template <typename T>
void Link::setInput2Link(){
  input2link = new T();
}

template <typename T>
void Link::setLink2Output(){
  link2output = new T();
}



#endif