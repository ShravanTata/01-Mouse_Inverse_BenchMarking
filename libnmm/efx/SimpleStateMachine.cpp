//////////////////////////////////////////////////////////////////////////////
// Copyright 2002-2006 Andreas Huber Doenni
// Distributed under the Boost Software License, Version 1.0. (See accompany-
// ing file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////
// The following code implements the state-machine (this version details an
// alternative way of retrieving the elapsed time from the main program):
//
//  --------------------------------
// |                                |
// |           O     Active         |
// |           |                    |<----
// |           v                    |     | EvReset
// |  ----------------------------  |     |
// | |                            | |-----
// | |         Stopped            | |
// |  ----------------------------  |
// |  |              ^              |
// |  | EvStartStop  | EvStartStop  |<-----O
// |  v              |              |
// |  ----------------------------  |
// | |                            | |
// | |         Running            | |
// |  ----------------------------  |
//  --------------------------------



#include <boost/statechart/event.hpp>
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/transition.hpp>
#include <boost/statechart/custom_reaction.hpp>

#include <boost/mpl/list.hpp>

#include <boost/config.hpp>

#include <ctime>
#include <iostream>

#ifdef BOOST_NO_STDC_NAMESPACE
namespace std
{
  using ::time;
  using ::difftime;
  using ::time_t;
}
#endif

#ifdef BOOST_INTEL
#  pragma warning( disable: 304 ) // access control not specified
#  pragma warning( disable: 444 ) // destructor for base is not virtual
#  pragma warning( disable: 981 ) // operands are evaluated in unspecified order
#endif



namespace sc = boost::statechart;
namespace mpl = boost::mpl;



struct EvStartStop : sc::event< EvStartStop > {};
struct EvReset : sc::event< EvReset > {};
struct EvPasAct : sc::event< EvPasAct > {};
struct EvGetElapsedTime : sc::event< EvGetElapsedTime >
{
  public:
    EvGetElapsedTime( double & time ) : time_( time ) {}

    void Assign( double time ) const
    {
      time_ = time;
    }

  private:
    double & time_;
};


struct EvOnOff          : sc::event< EvOnOff > {};
struct EvStanceEnd : sc::event< EvStanceEnd > {};
struct EvSwingEnd  : sc::event< EvSwingEnd > {};


struct Stance;
struct Stance_start;
struct Stance_end;
struct Swing;
struct Swing_start;
struct Swing_end;


struct Limb : sc::state_machine< Limb, Stance > {
public:
//	Limb( my_context ctx ) : my_base( ctx) {
//		post_event( EvOnOff() );
//	}
};

struct Stance : sc::simple_state< Stance, Limb, Stance_start > {
public:
	typedef sc::transition< EvOnOff, Swing > reactions;
	Stance(){
		std::cout << "We are in stance" << std::endl;
	}
	
};
struct Stance_start : sc::simple_state< Stance_start, Stance > {
public:
	typedef sc::transition< EvStanceEnd, Stance_end > reactions;
	Stance_start(){
		std::cout << "We are in stance_start" << std::endl;
	}
};
struct Stance_end : sc::simple_state< Stance_end, Stance > {
public:
	Stance_end(){
		std::cout << "We are in stance_end" << std::endl;
	}
};

struct Swing : sc::simple_state< Swing, Limb, Swing_start > {
public:
	typedef sc::transition< EvOnOff, Stance > reactions;
	Swing(){
		std::cout << "We are in swing" << std::endl;
	}
};

struct Swing_start : sc::simple_state< Swing_start, Swing > {
public:
	typedef sc::transition< EvSwingEnd, Swing_end > reactions;
	Swing_start(){
		std::cout << "We are in swing start" << std::endl;
	}
};
struct Swing_end : sc::simple_state< Swing_end, Swing > {
public:
	Swing_end(){
		std::cout << "We are in swing end" << std::endl;
	}
};



namespace
{
  char GetKey()
  {
    char key;
    std::cin >> key;
    return key;
  }
}

namespace LIMB_STATE{
	typedef enum
	{
		STANCE_START = 0,
		STANCE_END = 1,
		SWING_START = 2,
		SWING_END = 3
	} LimbState;
}
namespace LIMB{
	typedef enum
	{
		LEFT = 0,
		RIGHT = 1
	} LimbName;
}
int main()
{
  std::cout << "Boost.Statechart FSM example\n\n";
  std::cout << "l<CR>: Left on/off the ground\n";
  std::cout << "s<CR>: Switch\n";
  std::cout << "You may chain commands, e.g. rs<CR> resets and starts FSM\n\n";
  

  Limb left;
  Limb right;
  right.initiate();
  left.initiate();
  
  right.process_event( EvOnOff() );
  
  
  right.process_event( EvSwingEnd() );
  
  int * test;
  test = new int[2];

  test[LIMB::RIGHT] = LIMB_STATE::STANCE_START;
  
  /*
  char key = GetKey();

  while ( key != 'e' )
  {
    switch( key )
    {
      case 'l':
      {
        left.process_event( EvOnOff() );
      }
      break;

      case 's':
      {
        left.process_event( EvStanceEnd() );
      }
      break;
	  
	  case 'p':
	  {
		left.process_event( EvSwingEnd() );
	  }
	  break;
      default:
      {
        std::cout << "Invalid key!\n";
      }
      break;
    }

    key = GetKey();
  }
  */

  return 0;
}
