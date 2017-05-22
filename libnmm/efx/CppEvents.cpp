#include <iostream>
#include "../otherboost/signals2.hpp"

struct HelloWorld
{
  void operator()() const
  {
    std::cout << "Hello, World!" << std::endl;
  }
};

class Toto{
public:
	double test;
	Toto(){test=2.0;};
	void operator()(std::string test3, float test2) const
	{
		std::cout << "fuck" << test2 << test3 << std::endl;
	}
	void testeur(std::string hello, float test, float test3){
		std::cout << "hem" << hello << std::endl;
	}
};

// Signal with no arguments and a void return value
void test(std::string test2, float test, float test3){
	std::cout << "nooo" << test2 << std::endl;
}

namespace CONTROL{
typedef enum
{
	ONE = 0,
	TWO = 2,
	THR = 3,

} EventType;
}


int main(){

//boost::signals2::signal<void (float)> sig;
//typedef boost::signals2::signal<void(Foot * foot,)> EVENT;
typedef boost::signals2::signal<void(float)> EVENT;
EVENT limbon;
EVENT limboff;

EVENT sig;
/*
if(loadParam)
	limbon.connect(boost::bind(&Parameters::loadReflexParameters

if( 
*/
Toto toto;
boost::function<void(float, float)> onClick = boost::bind(&Toto::testeur, boost::ref(toto), "hellohello",_1,_1);
std::cout << CONTROL::ONE << std::endl;

sig.connect(boost::bind(&Toto::testeur, boost::ref(toto), "hello",_1,_1));
boost::signals2::connection c = sig.connect(boost::bind(&Toto::testeur, boost::ref(toto), "hellohello",_1,_1));
//sig.connect(boost::bind(boost::bind(&Toto::testeur, toto, _1), "hello",_1));
//sig.connect(boost::bind(&Toto::testeur, toto, _1));
//sig.connect(boost::bind(toto, "hello",_1));



// Call all of the slots
sig(-2);


std::cout << "--" << std::endl;

c.disconnect();

sig(-2);

sig.connect(boost::bind(&Toto::testeur, boost::ref(toto), "hellohello",_1,_1));
std::cout << "--" << std::endl;
sig(-2);
}


/*
typedef boost::signals2::signal<void (int keyCode)> SigKeyPressed;
typedef SigKeyPressed::slot_type SigKeyPressedType;

class SomeClass
{
	SigKeyPressed mSigKeyPressed;
	
public:
	SomeClass() { mSigKeyPressed.connect(boost::bind(&SomeClass::keyPressed, this, _1)); }
	void keyPressed(const SigKeyPressedType &slot);
};
*/
