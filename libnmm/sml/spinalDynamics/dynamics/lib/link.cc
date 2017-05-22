#include "link.hh"
#include "state.hh"
#include "system.hh"

/* --------------------------------------------------------------------------- */

/*
----------
: HOW TO :
----------




*/

Link::Link(State* in, State* out): 
    in(*in), 
    out(*out),
    link(*in)
  {
    parameters.get<double>("p0") = 1.0;
    clock = 0;
  }

Link::Link(State& in, State& out): 
    in(in), 
    out(out),
    link(in)
  {
    parameters.get<double>("p0") = 1.0;
    clock = 0;
  }

Link::~Link(){
    delete input2link;
    delete linkProcessing;
    delete link2output;
    //NetworkManager::RemoveLink(this);
}

void Link::updateFunction(const State & in, const Real& t ){
  //this->input2link->apply(*this, in, Time::getTime());
	this->input2link->apply(*this, in, clock->getTime());
}
void Link::updateState(){
  linkProcessing->process(*this);
}
void Link::apply(){
  link2output->apply(*this);
}


namespace Input2Link{
  Base::~Base(){}
}



namespace LinkProcessing {
  Base::~Base(){}
}

namespace Link2Output {
  Base::~Base(){}
}
