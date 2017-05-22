#ifndef __IO_SYSTEM__HH__
#define __IO_SYSTEM__HH__



#include <network.hh>

/**
 * @brief A input output system.
 * @details This class can be used to create a Network that needs to take input from the outside world and needs to set 
 * output in the outside world. The idea is to use two vector of State pointer (inputs and outputs).
 * The person in charge of the implementation needs then to ensure that before updating the network the inputs State 
 * are set correctly and that after the network has been updated the outputs state are correctly used to update
 * the output values.
 * 
 * Since this class inherits from the Network class, it can also be used as part of the NetworkManager
 * 
 * All class inheriting from IOSystem needs to implement the init methods.
 */
class IOSystem : public Network{
public:
  IOSystem(){}
  virtual ~IOSystem(){}
  IOSystem(int dim);
  IOSystem(int input_dim, int output_dim);
  std::vector<State*> inputs;
  std::vector<State*> outputs;

  virtual void init()=0;
  template< typename T>
  void newLink(State* in, State* out);
};

template<typename T>
void
IOSystem::newLink(State* in, State* out){
  addLink(new T(*in,*out));
}

#endif