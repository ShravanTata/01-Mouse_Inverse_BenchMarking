#include "link_library.hh"
#include "system.hh"



ConstantLink::ConstantLink(State &in, State& out) : Link(in, out){
  setInput2Link<Input2Link::Empty>();
  setLink2Output<Link2Output::Adder>();
  setLinkProcessing<LinkProcessing::Constant>();
}

StandardLink::StandardLink(State &in, State& out) : Link(in, out){
  setInput2Link<Input2Link::Constant>();
  setLink2Output<Link2Output::Adder>();
  setLinkProcessing<LinkProcessing::Constant>();
}

LinkAdd::LinkAdd(State &in, State& out) : Link(in, out){
  setInput2Link<Input2Link::Constant>();
  setLink2Output<Link2Output::Adder>();
  setLinkProcessing<LinkProcessing::Constant>();
}

LinkAbs::LinkAbs(State &in, State& out) : Link(in, out){
  setInput2Link<Input2Link::Absolute>();
  setLink2Output<Link2Output::Adder>();
  setLinkProcessing<LinkProcessing::Constant>();
}
LinkSub::LinkSub(State &in, State& out) : Link(in, out){
  setInput2Link<Input2Link::Inverse>();
  setLink2Output<Link2Output::Adder>();
  setLinkProcessing<LinkProcessing::Constant>();
}
LinkI1::LinkI1(State &in, State& out) : Link(in, out){
  setInput2Link<Input2Link::Linear>();
  setLink2Output<Link2Output::Adder>();
  setLinkProcessing<LinkProcessing::Euler>();
}

LinkD0::LinkD0(State &in, State& out) : Link(in, out){
  setInput2Link<Input2Link::Linear>();
  setLink2Output<Link2Output::Adder>();
  setLinkProcessing<LinkProcessing::Constant>();
}

LinkD1::LinkD1(State &in, State& out) : Link(in, out){
  setInput2Link<Input2Link::Difference>();
  setLink2Output<Link2Output::Adder>();
  setLinkProcessing<LinkProcessing::Constant>();
}



void FirstOrderLowPassFilter::equadiff::apply(Link & link, const State & in, Real t){
  State& out = link.out;
  Real& p0 = link.parameters.get<double>("p0");
  for(UInt i=0; i<in.dimension(); i++){
    link.link[i] = p0*(in.transferFunction(i)-out(i));
  }
}

void LinkImpulse::equadiff::apply(Link & link, const State & in, Real t){
  State& out = link.out;
  Real& impulse_start_time = link.parameters.get<double>("impulse_start_time");
  Real& impulse_duration = link.parameters.get<double>("impulse_duration");
  Real& impulse_amplitude = link.parameters.get<double>("impulse_amplitude");
  Real& impulse_time_constant= link.parameters.get<double>("impulse_time_constant");

  std::function<bool (const Real&)> impulseOn = [&](const Real& t) -> bool
  {
    if ( t >= impulse_start_time && t <= impulse_start_time + impulse_duration){
      return true;
    }
    else{
      return false;
    }
  };

  std::function<Real (const Real&)> u = [&](const Real& t) -> Real
  {
    if (impulseOn(t)){
      if( ! link.parameters.get<bool>("impulse/going_on") )
        link.parameters.get<bool>("impulse/just_started") = true;
      else
        link.parameters.get<bool>("impulse/just_started") = false;
      link.parameters.get<bool>("impulse/going_on") = true;
      return impulse_amplitude;
    }
    else{
      return 0.0;
    }
  };



  for(UInt i=0; i<in.dimension(); i++){
    link.link[i] = impulse_time_constant*(u(t)-out(i));
  }

}



void LinkProcessing::Constant::process(Link & l){
  l.updateFunction(l.in, l.clock->getTime());
}
void LinkProcessing::Euler::process(Link & l){
  l.updateFunction(l.in,l.clock->getTime());
  l.link *= l.clock->getTimeStep();
}
void LinkProcessing::RungeKutta::process(Link & l){
  const Real& dt = l.clock->getTimeStep();
  const Real& t = l.clock->getTime();
  const State& in = l.in;

  l.updateFunction(l.in,t);
  State& k1 = l.link;
  k1*=dt/2;
  l.updateFunction(l.in+k1,t+dt/2);
  State& k2 = l.link;
  k2*=dt/2;
  l.updateFunction(l.in+k2,t+dt/2);
  State& k3 = l.link;
  k3*=dt;
  l.updateFunction(l.in+k3,t+dt);
  State& k4 = l.link;

  l.link = k1;
  l.link += k2*2.0;
  l.link += k3*2.0;
  l.link += k4;
  l.link *= dt/6;
}






void Input2Link::Empty::apply(Link & link, const State &in, Real t){
}
void Input2Link::Constant::apply(Link & link, const State &in, Real t){
  link.link = link.in.transferFunction();
}
void Input2Link::Absolute::apply(Link & link, const State &in, Real t){
  for(UInt i=0; i<link.in.dimension(); i++)
    link.link(i) = fabs(link.in.transferFunction(i));
}

void Input2Link::Inverse::apply(Link & link, const State &in, Real t){
  link.link = -link.in.transferFunction();
}

void Input2Link::Difference::apply(Link & link, const State &in, Real t){
  Real& p0 = link.parameters.get<double>("p0");
  State& x = link.in;
  const Real& dt = TIME_STEP;
  for(UInt i=0; i<link.in.dimension(); i++)
    link.link[i] = p0*(x.transferFunction(i,0)-x.transferFunction(i,1))/dt;
}

void Input2Link::Delay::apply(Link & link, const State & in, Real t){
  State& out = link.out;
  UInt& p0 = link.parameters.get<UInt>("p0");
  for(UInt i=0; i<link.in.dimension(); i++)
    link.link[i] = p0*in.transferFunction(i,p0);
}

void Input2Link::Linear::apply(Link & link, const State & in, Real t){
  State& out = link.out;
  Real& p0 = link.parameters.get<double>("p0");
  for(UInt i=0; i<link.in.dimension(); i++)
    link.link[i] = p0*in.transferFunction(i);
}


void Link2Output::Setter::apply(Link & link){
  link.out = link.link;
}
void Link2Output::Adder::apply(Link & link){
  link.out += link.link;
}
void Link2Output::Multiplier::apply(Link & link){
  for(UInt i=0; i<link.in.dimension();i++)
  link.out(i) *= link.link(i);
}