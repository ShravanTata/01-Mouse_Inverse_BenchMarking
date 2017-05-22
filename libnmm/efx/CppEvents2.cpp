#include <iostream>
#include "../otherboost/signals2.hpp"

struct HelloWorld
{
  void operator()() const
  {
    std::cout << "Hello, World!" << std::endl;
  }
};

class Foot{
public:
	double test;
	Toto(){test=2.0;};
	void operator()(int state) const
	{
	}
};

// Signal with no arguments and a void return value

int main(){


boost::signals2::signal<void (int)> sig;
// Connect a HelloWorld slot
Foot left_foot;
Foot right_foot;

left_foot.connect(stance);
left_foot.connect(swing);

right_foot.connect(stance);
right_foot.connect(swing);


// Call all of the slots
sig(2);


}
