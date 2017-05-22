#ifndef __STATE__HH__
#define __STATE__HH__
#include <vector>
#include <cmath>
#include <stdexcept>      // std::length_error
#include <iostream>
#include <functional>
#include <deque>          // std::queue
#include <unordered_map>

#include <dynamics/config.hh>
#include <dynamics/indexed.hh>

typedef std::vector<Real> Vector;


/**
 * @brief a class that stores a multidimensional state.
 * @details 
 * 
 *  This class encapsulates the std::vector and is used
 *  as a building block to represent States for non
 *  autonomous multidimensional dynamical system
 *   
 *  
 * # Memory management
 * 
 * Each time a Network is updated the current State is saved.
 * The value of the State at previous time step can be accessed with the () operator.
 * The maximum memory size is defined in the MEMORY constant in dynamics/config.hh header
 * 
 * \code{.cc}
 * 
 * State& a = NetworkFactory::NewState(1.0,1.0,1.0); // 3 dimensional State created with an intial value of 1.0
 * 
 * NetworkManager::Update();
 * a(0) = 2.0;
 * NetworkManager::Update();
 * a(0) = 3.0;
 * NetworkManager::Update();
 * a(0) = 4.0;
 * 
 * a(0) == 4.0; // Access the first dimension of the current state value here a(0) == 4.0
 * a(0,1) == 3.0; // Access the first dimension of the state at the last time step (i.e. x(t-dt))
 * 
 * \endcode
 * 
 * # Transfer function
 * 
 * A transfer function is associated to each State (by default a Identity transfer function is used).
 * 
 * The value of the state after the transfer function can be accessed as follow : 
 * 
 * \code{.cc}
      State x1 = State(2.0,1.0); // State creation
      std::function<Real (const Real&)> f = [](const Real& x) -> Real 
      {
          return 2*x;
      }; // Function to be used as trasnfer function


      State x2 = x1;

      REQUIRE( x2.transferFunction(0) == x2(0) ); // With identity transfer function the value of the state is not changed
      REQUIRE( x2.transferFunction(1) == x2(1) );

      x2.transferFunction.set(new State::TransferFunctions::AnyFunc(f));
      REQUIRE( x2.transferFunction(0) == f(x2(0)) ); // With the transfer function the value of the state is changed
      REQUIRE( x2.transferFunction(1) == f(x2(1)) );
 * \endcode
 * 
 */
class State : public Indexed
{
friend class NetworkManager;
public:
  /**
   * @brief structure encapsulating the different TransferFunctions that can be used as transfer function for the State
   */
  struct TransferFunctions {
    /**
     * @brief base class of the TransferFunctions familiy
     * @details The TransferFunctions family is defined by a () operator that takes a real as argument.
     * 
     * This methods represents the action of the transferFunction on one dimension of the State.
     * Therefore, the transferFunction effect on each dimension of the State is the same
     */
    struct Base {
      virtual Real operator()(const Real& x)=0;
      virtual ~Base(){}
    };
    /**
     * @brief Identity transfer function. The default transfer function used by a State
     */
    struct Identity: public Base{
      Real operator()(const Real& x){
        return x;
      }
    };
    /**
     * @brief This structure can be used to use any std::function defined locally as a transfer function.
     * Providing that the function has the right structure (const Real& x) -> Real 
     * @details Example : 
     * 
     * 
\code{.cc}
State x = State(2.0,1.0);
std::function<Real (const Real&)> f = [](const Real& x) -> Real 
{
    return 2*x;
};
x.transferFunction.set(new State::TransferFunctions::AnyFunc(f));

\endcode
     * 
     */
    struct AnyFunc: public Base{
      AnyFunc(std::function<Real ( const Real&) > f_) : f(f_){}
      Real operator()(const Real& x){
        return f(x);
      }
      std::function<Real ( const Real&)> f;
    };
  };
  /**
   * @brief The transferFunction class.
   * @details Each State has a member transferFunction.
   * This class has a pointer to the State and to the TransferFunctions::Base class.
   * It has the same () operator than the state which make the transferFunction usage similar to the states one
   * 
   * Example 
   * 
\code{.cc}
State x = State(2.0,1.0);
x(0); // Access the element 0 of the state
x.transferFunction(0); // Access the element 0 of the state after having pass through the tf transfer function.
\endcode
   */
  class TransferFunction
  {
    friend class State;
  public:
    TransferFunction();
    void set(TransferFunctions::Base* f);
    State operator ()() const;
    Real operator ()(UInt i) const;
    Real operator ()(UInt,UInt) const;

  private:
    State * super;
    TransferFunctions::Base* tf;
  };
  TransferFunction transferFunction;
  

public:
  State(UInt x): X(x){}
  template<typename... Args>
  State(Real first, Args... args){
    fillVector(X,first, args...);
    init();
  }
  State(const std::vector<Real>&);

  virtual ~State();



  State(const State&);


  /**
   * @brief Remove the oldest memory and Save the current state in the memory
   */
  void save();
  /**
   * @brief Post processing method implemented by the StateLess method.
   */
  virtual void reset(){}

public:



  UInt dimension();
  UInt dimension() const;
  

  /**
   * @brief Access the element i of the State
   * 
   * @param i the element number (indice starts at 0)
   * @return the value of State element i
   */
  virtual Real  operator ()(UInt i) const;
  /**
   * @brief Access the element i of the State)
   * 
   * @param i the element number (indice starts at 0)
   * @return a reference to the value of State element i
   */
  virtual Real& operator ()(UInt i);
  /**
   * @brief Access the element i of the State at time t-delay (where t is the current time)
   * 
   * @param i the element number (indice starts at 0)
   * @param delay the delay 
   * @return the value of State element i at time t-delay
   */
  virtual Real  operator ()(UInt i,UInt delay) const;
  /**
   * @brief Access the element i of the State at time t-delay (where t is the current time)
   * 
   * @param i the element number (indice starts at 0)
   * @param delay the delay 
   * @return a reference to the value of State element i at time t-delay
   */
  virtual Real& operator ()(UInt i,UInt delay);
  /**
   * @brief Same than () operator 
   */
  virtual Real  operator [](UInt i) const;
  /**
   * @brief Same than () operator 
   */
  virtual Real& operator [](UInt i);

  State& operator +=(const State& other);
  State& operator -=(const State& other);
  //State& operator *=(State const& rhs);
  State& operator *=(Real const& scalar);

  const State& operator = (const State& other);
  const State  operator + (const State& other) const;
  const State  operator - () const;
  const State  operator - (const State& other) const;
  const State  operator *(Real const& scalar) const;


public:
  Vector X;

protected:
  /**
   * @brief sets the transferFunction tf to Identity and fills the Xqueue vector (i.e. the memory) with the initial value
   */
  void init();
  /**
   * The memory. deque of Vector*
   */
  std::deque<Vector *> Xqueue;
  static Real nan;
  

};



/**
 * @brief StateLess. Similar to State except that the State has no memory.
 * @details In the update process, after the all the State have been saved, 
 * the Link::updateState method is called all the StateLess state are reset,
 * by calling the resest methods
 */
class StateLess : public State{
public:
  using State::State;
  /**
   * @brief Resets the State value. 
   */
  void reset(){
    for (auto& kv: X)
      kv = 0.0;
  }
};



/* --------------------------------------------------------------------------- */


inline const State& State::operator = (const State& other){
  try{
    if(dimension() != other.dimension())
      throw std::out_of_range ("State dimension mismatch");
    UInt i;
    for(i=0;i<other.dimension();i++){
      (*this)[i] = other[i];
    }
    return *this;
  }
  catch (const std::out_of_range& e){
    std::cerr << "Out of Range error: " << e.what() << std::endl;
  }
  return *this;
}
inline State& State::operator -=(const State& other){
  try{
    if(dimension() != other.dimension())
      throw std::out_of_range ("State dimension mismatch");
    UInt i;
    for(i=0;i<other.dimension();i++){
      (*this)[i] -= other[i];
    }
    return *this;
  }
  catch (const std::out_of_range& e){
    std::cerr << "Out of Range error: " << e.what() << std::endl;
  }
  return *this;
}
inline State& State::operator +=(const State& other){
  try{
    if(dimension() != other.dimension())
      throw std::out_of_range ("State dimension mismatch");
    UInt i;
    for(i=0;i<other.dimension();i++){
      (*this)[i] += other[i];
    }
    return *this;
  }
  catch (const std::out_of_range& e){
    std::cerr << "Out of Range error: " << e.what() << std::endl;
  }
  return *this;
}
inline const State State::operator + (const State& other) const
{
  State v(*this);
  v += other;
  return v;
}

inline const State State::operator - () const
{
  State v(*this);
  UInt i;
  for(i=0;i<dimension();i++){
    v[i] = -(*this)[i];
  }
  return v;
}
inline const State State::operator - (const State& other) const
{
  State v(*this);
  v += -other;
  return v;
}

inline const State State::operator *(Real const& scalar) const
{
  State v(*this);
  v *= scalar;
  return v;
}
// inline State& State::operator*=(State const& rhs)
// {
//   return *this;
// }

inline State& State::operator*=(Real const& scalar)
{
  UInt i;
  for(i=0;i<dimension();i++){
    (*this)[i] *= scalar;
  }
  return *this;
}

inline Real State::operator [](UInt i) const{
  /*
    read [] overloading operator
  */
  if (i < X.size()){
    return X[i];
  }

  std::cout << "Error State size is " << X.size() << " but requested element " << i+1 << std::endl;
  return State::nan;
}


inline Real& State::operator [](UInt i){
  /*
    assignment [] overloading operator
  */
  if (i < dimension()){
    return X[i];
  }
  return State::nan;
}




inline State& operator*(Real const& scalar, State lhs)
{
    return lhs *= scalar; // calls lhs.operator*=(scalar);
}



inline Real State::operator ()(UInt i) const{
  /*
    read [] overloading operator
  */
  if (i < X.size()){
    return X[i];
  }

  std::cout << "Error State size is " << X.size() << " but requested element " << i+1 << std::endl;
  return State::nan;
}
inline Real& State::operator ()(UInt i){
  /*
    read [] overloading operator
  */
  if (i < X.size()){
    return X[i];
  }

  std::cout << "Error State size is " << X.size() << " but requested element " << i+1 << std::endl;
  return State::nan;
}
inline Real State::operator ()(UInt i, UInt Dt) const{
  /*
    read () overloading operator
  */
  if (Dt > MEMORY){
    Dt = MEMORY;
    std::cout << "Error requested state " << Dt << " time step before" << std::endl;
    std::cout << "Limit was : " << MEMORY << " time step " << std::endl;
  } 
  if (i < X.size()){
    return (*Xqueue[Dt])[i];
  }

  std::cout << "Error State size is " << X.size() << " but requested element " << i+1 << std::endl;
  return State::nan;
}
inline Real& State::operator ()(UInt i, UInt Dt){
  /*
    read () overloading operator
  */
  if (Dt > MEMORY){
    Dt = MEMORY;
    std::cout << "Error requested state " << Dt << " time step before" << std::endl;
    std::cout << "Limit was : " << MEMORY << " time step " << std::endl;
  } 
  if (i < X.size()){
    return (*Xqueue[Dt])[i];
  }

  std::cout << "Error State size is " << X.size() << " but requested element " << i+1 << std::endl;
  return State::nan;
}

/* -------------------------------------------------------------------------- */
#endif //__VECTOR__HH__
