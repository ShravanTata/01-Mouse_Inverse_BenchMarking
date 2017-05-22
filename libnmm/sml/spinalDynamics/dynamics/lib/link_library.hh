#ifndef __LINK_LIBRARY__HH__
#define __LINK_LIBRARY__HH__

#include <link.hh>



namespace Input2Link      {
  struct Empty: Base      { void apply(Link & link, const State & in, Real t); };
  struct Constant: Base   { void apply(Link & link, const State & in, Real t); };
  struct Absolute: Base   { void apply(Link & link, const State & in, Real t); };
  struct Inverse: Base    { void apply(Link & link, const State & in, Real t); };
  struct Difference: Base { void apply(Link & link, const State & in, Real t); };
  struct Delay: Base      { void apply(Link & link, const State & in, Real t); };
  struct Linear : Base    { void apply(Link & link, const State & in, Real t); };
};

namespace LinkProcessing  {
  struct Constant: Base   { void process(Link & link); };
  struct Euler: Base      { void process(Link & link); };
  struct RungeKutta: Base { void process(Link & link); };
};

namespace Link2Output {

  struct Setter : Base    { void apply(Link & link); };
  struct Adder : Base     { void apply(Link & link); };
  struct Multiplier : Base{ void apply(Link & link); };

};

/**
 * @brief A constant link.
 * @details This link does not change the state of the link it simply adds it 
 * to the output : $ out += link $
 */
class ConstantLink : public Link {

public:
  ConstantLink(State &in, State& out);
};

/**
 * @brief A standard link
 * @details This link simply add the input to the output
 * to the output : $ out += in $
 */
class StandardLink : public Link {
public:
  StandardLink(State &in, State& out);
};
/**
 * @brief Identical to the StandardLink: here for retrocompatibility
 */
class LinkAdd : public Link {
public:
  LinkAdd(State &in, State& out);
};
/**
 * @brief A stardard link adding the absolute value to the output 
 * @details This link simply adds the absolute value of the input to the output
 * to the output : $ out += |in| $
 */
class LinkAbs : public Link {
public:
  LinkAbs(State &in, State& out);
};
/**
 * @brief A standard link substractor
 * @details This link simply substract the input to the output
 * to the output : $ out -= in $
 */
class LinkSub : public Link {
public:
  LinkSub(State &in, State& out);
};
/**
 * @brief An euler integrator
 * @details This link simply add the input to the output
 * to the output : $ out += dt*in $
 */
class LinkI1 : public Link {
public:
  LinkI1(State &in, State& out);
};
/**
 * @brief A constant multiplier
 * @details This link adds a linear transformation of the input
 * : $ out += p0*in $
 * 
 */
class LinkD0 : public Link {
public:
  LinkD0(State &in, State& out);
};
/**
 * @brief A derivator
 * @details This link adds a linear transformation of the input
 * : $ out = p0*in $
 * 
 */
class LinkD1 : public Link {
public:
  LinkD1(State &in, State& out);
};

/**
 * @brief A FirstOrderLowPassFilter
 */
class FirstOrderLowPassFilter : public Link {
  /*
  Z domain ==> Y(z)/X(z) = 1/ (1 + tau*z)

  ==> dy/dt = tau * (x - y);
  */
  struct equadiff : Input2Link::Base {
    void apply(Link & link, const State & in, Real t);
  };
public:
  FirstOrderLowPassFilter(State &in, State& out) : Link(in, out){
    setInput2Link<equadiff>();
    setLink2Output<Link2Output::Adder>();
    setLinkProcessing<LinkProcessing::Euler>();
  }
};


/**
 * @brief An impulse response link
 * @details This link applies an impulse response
 *
    dx = tau * ( u(t) - x )

    u(t) = amp if ON 0 else
 * 
 */
class LinkImpulse : public Link {
public:
  /*
  
    dx = tau * ( u(t) - x )

    u(t) = amp if ON 0 else

  */
  struct equadiff : Input2Link::Base {
    void apply(Link & link, const State & in, Real t);
  };
  LinkImpulse(State&in, State& out) : Link(in,out){
    setInput2Link<equadiff>();
    setLink2Output<Link2Output::Adder>();
    setLinkProcessing<LinkProcessing::Euler>();
    parameters.get<double>("impulse_start_time") = 0.0;
    parameters.get<double>("impulse_duration") = 1.0;
    parameters.get<double>("impulse_amplitude") = 1.0;
    parameters.get<double>("impulse_time_constant") = 1.0;
  }
};

#endif