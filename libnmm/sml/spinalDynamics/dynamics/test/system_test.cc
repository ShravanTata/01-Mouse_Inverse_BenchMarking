#include "catch.hpp"

#include <cstdio>
#include <cstdlib>
#include <iostream>          
#include <fstream>
#include <string>


#include <setting-manager/settings.hh>
#include <timer/clock.hh>

#include <link_library.hh>
#include <state.hh>
#include <network_manager.hh>
#include <network_factory.hh>
#include <system.hh>
#include <io_system.hh>



TEST_CASE( "System", "[system]") {
	SECTION( "NewTime local clock"){
		Network localNet;
		Time my_clock(localNet);
		my_clock.init();

		REQUIRE( my_clock.getTime() == 0.0 );
		localNet.update();
		REQUIRE( my_clock.getTime() == my_clock.getTimeStep() );
		localNet.update();
		REQUIRE( my_clock.getTime() == 2*my_clock.getTimeStep() );
	}

	SECTION( "Impulse link") {
		Clock<time_unit::us> clock;

		UInt networkSize = 1000;
		std::vector<Real> outputs;
		std::vector<Impulse*> impulses; 
		Network localNet;
		localNet.initClock(0.01);
		clock.start();
		for (UInt i=0; i< networkSize; i++){
			outputs.push_back(0.0);
		}
		for (UInt i=0; i< networkSize; i++){
			impulses.push_back(new Impulse(&localNet));
			impulses.back()->init(outputs[i],localNet.getClock().getTime()+i/10.0);
		}

		clock.end();
		//clock.printMean();
		/*
		Network creation time
			1	   : 29us
			10	   : 755us
			100    : 4276us
			1000   : 29171us
			10000  : 247641us
			100000 : 26000000us


		Network update time

			1      : 2us
			10     : 31us
			100    : 346us
			1000   : 2ms
			10000  : 35ms
			100000 : 360ms


			Size = [1,10,100,1000,10000,100000];
			Duration = [2, 31, 346, 2, 35, 360];
		*/


		int i=0;

		while ( i < 20){
			i++;
			
			//clock.start();
			localNet.update();

			// std::cout 
			// << localNet.getClock().getTime() 
			//  	<< "\t" << impulses[0].getOutput(0) 
			//  	<< "\t" << impulses[1].getOutput(0)
			//  	<< "\t" << impulses[2].getOutput(0)
			//  	<< std::endl;

			REQUIRE(outputs[0] == impulses[0]->getOutput(0) );
			REQUIRE(outputs[1] == impulses[1]->getOutput(0) );
			REQUIRE(outputs[2] == impulses[2]->getOutput(0) );

			//clock.end();
		

			//std::cout << NetworkManager::GetNetwork().getClock().getTime() << "\t" << x(0) << std::endl;
		}
		//clock.printMean();
	}



	class FirstOrderLowPassFilterSystem : public IOSystem {
		using IOSystem::IOSystem;
		public:
		  void init(){
			for( UInt i=0; i<inputs.size(); i++){
				newLink<FirstOrderLowPassFilter>(inputs[i],outputs[i]);
			}
		}
	};

    #include <ctime>
    SECTION("Simple input/output system"){
     	static FirstOrderLowPassFilterSystem* filters = new FirstOrderLowPassFilterSystem(10);
     	filters->initClock();
     	filters->init();
		while (false){
			for( UInt i=0; i<filters->inputs.size(); i++){
				(*filters->inputs[i])(0) = cos(i/11.0*2*3.14 + NetworkManager::GetNetwork().getClock().getTime()) + 0.5*((double)std::rand()/RAND_MAX-1.0);
			}

		}
		NetworkManager::Update();
		filters->update();
		//cout << (*filters->inputs[5])(0) << " " << (*filters->outputs[5])(0) << endl;
	}

}