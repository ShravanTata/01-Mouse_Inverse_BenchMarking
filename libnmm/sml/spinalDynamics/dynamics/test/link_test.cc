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


TEST_CASE( "Link Class", "[link]" ) {
	SECTION( "LinkAdd") {
		State& x = NetworkFactory::NewState(1.0);
		State& yAdd = NetworkFactory::NewState(0.0);
		State& ySub = NetworkFactory::NewState(0.0);

		NetworkFactory::NewLink<LinkAdd>(x,yAdd);
		NetworkFactory::NewLink<LinkSub>(x,ySub);

		REQUIRE(yAdd(0) == 0.0);
		REQUIRE(ySub(0) == 0.0);
		NetworkManager::Update();
		REQUIRE(yAdd(0) == 1.0);
		REQUIRE(ySub(0) == -1.0);
		NetworkManager::Update();
		REQUIRE(yAdd(0) == 2.0);
		REQUIRE(ySub(0) == -2.0);
	}
	SECTION( "LinkD0/D1"){
		static State& x = NetworkFactory::NewState(0.0);
		static State& y = NetworkFactory::NewStateLess(0.0);
		static State& dy = NetworkFactory::NewStateLess(0.0);
		static State& Iy = NetworkFactory::NewState(0.0);
		static std::function<Real (const Real&)> f_cos = [](const Real& x) -> Real
        {
        	Real t = NetworkManager::GetNetwork().getClock().getTime();
         	return cos(t); // Not working with differential because think 2 seconds dude
         	// How do you want this function to know that it needs to use the past time
        };
		x.transferFunction.set(new State::TransferFunctions::AnyFunc (f_cos));

		NetworkFactory::NewLink<LinkAdd>(x,y);
		NetworkFactory::NewLink<LinkD1>(y,dy);
		NetworkFactory::NewLink<LinkI1>(y,Iy);



		for (int i = 0; i < 1000; ++i)
		{
				NetworkManager::Update();
				Real _dy = 1/NetworkManager::GetNetwork().getClock().getTimeStep()*( y.transferFunction(0,0) - y.transferFunction(0,1));
				REQUIRE( fabs(_dy - dy(0)) < EPSILON);
				//Real _Iy = sin(NetworkManager::GetNetwork().getClock().getTime()-NetworkManager::GetNetwork().getClock().getTime());
				//REQUIRE( fabs(_Iy - Iy(0)) < 0.01);
				//std::cout << _Iy << " " << Iy(0) << std::endl;
		}


	}
}

TEST_CASE( "DifferentialEquation", "[equaDiff]") {
	// Time stepping works
	// using namespace std;
	// class FirstOrder : public DifferentialEquation{
	// 	using DifferentialEquation::DifferentialEquation;
	// 	private: void updateFunction(const State & X, Real t){
	// 		//cout << (1-X[0])<< endl;
	// 		link[0] = time_constant*(abs(in(0))-abs(out(0)));
	// 	}
	// 	private: void updateState(){
	// 		//Integrator::RungeKutta(*this);
	// 		Integrator::Euler(*this);
	// 	}
	// 	public:
	// 		Real time_constant;
	// };
	
	// static State& x1 = NetworkFactory::NewState(0.0);


	SECTION( "Time check" ) {
		REQUIRE(NetworkManager::GetNetwork().getClock().getTimeStep() == TIME_STEP);
		Real t = NetworkManager::GetNetwork().getClock().getTime();
		NetworkManager::Update();
		Real newt = NetworkManager::GetNetwork().getClock().getTime();
		Real expected_newt = t+NetworkManager::GetNetwork().getClock().getTimeStep();
		REQUIRE(expected_newt == newt);
	}
	// SECTION( "Link creation" ) {
	// 	static int length = NetworkManager::fullNet.links.size();
	// 	Link& l = NetworkFactory::NewLink<FirstOrder>(x1,x1);
	// 	((FirstOrder&) l).time_constant = 100;
	// 	REQUIRE ( NetworkManager::fullNet.links.size() == length+1);
	// }


	SECTION( "Link + TransferFunction Combinaison" ) {
   		static State& y = NetworkFactory::NewState(1.0); // output of the system
   		static State& ydes = NetworkFactory::NewState(0.0); // what we want ( a sine wave)
   		std::function<Real (const Real&)> f = [](const Real& x) -> Real {
   			Real t = NetworkManager::GetNetwork().getClock().getTime();
   			return cos(t);
   		};
   		ydes.transferFunction.set(new State::TransferFunctions::AnyFunc (f));

   		static State error = NetworkFactory::NewState(0.0); // the error
   		static Link& l1 = NetworkFactory::NewLink<LinkAdd>(ydes, error);
   		static Link& l2 = NetworkFactory::NewLink<LinkSub>(y, error);
   		Real t = NetworkManager::GetNetwork().getClock().getTime();
   		NetworkManager::Update();
   		REQUIRE(error(0) == cos(t)-1.0);
   	}

   	SECTION( "MultiLink system"){
   		static State& out = NetworkFactory::NewStateLess(0.0);
   		static State& out2 = NetworkFactory::NewState(0.0);

   		static State& a = NetworkFactory::NewState(1.3);
   		static State& b = NetworkFactory::NewState(1.4);
   		static State& c = NetworkFactory::NewState(1.8);

   		static Link& l1 = NetworkFactory::NewLink<LinkAdd>(a, out);
   		static Link& l2 = NetworkFactory::NewLink<LinkAdd>(b, out);
   		static Link& l3 = NetworkFactory::NewLink<LinkAdd>(c, out);
   		static Link& l4 = NetworkFactory::NewLink<LinkAdd>(a, out2);
   		static Link& l5 = NetworkFactory::NewLink<LinkAdd>(b, out2);
   		static Link& l6 = NetworkFactory::NewLink<LinkAdd>(c, out2);

   		REQUIRE( out(0)  == 0.0 );
   		NetworkManager::Update();
   		for(int i=0; i<10; i++){
   			REQUIRE( out(0) == a(0)+b(0)+c(0) );
   			REQUIRE( out2(0) == out2(0,1)+a(0)+b(0)+c(0) );
    			if(out2(0,1) != 0.0)
    				REQUIRE( out2(0) != a(0)+b(0)+c(0) );
   		}
   	}
	std::function<Real (const Real&)> fcos = [](const Real& x) -> Real
    {
    	static Real t_start = NetworkManager::GetNetwork().getClock().getTime();
    	Real t = NetworkManager::GetNetwork().getClock().getTime();
     	return cos(t-t_start);
    };
	SECTION( "Retroaction, PID Control") {
   		/** State creation */
   		static State& y = NetworkFactory::NewState(10.0); // output of the system
   		static State& ydes = NetworkFactory::NewState(0.0); // what we want ( a sine wave)

        ydes.transferFunction.set(new State::TransferFunctions::AnyFunc (fcos));
        static State& u = NetworkFactory::NewStateLess(0.0);
   		static State& error = NetworkFactory::NewStateLess(0.0); // the error
   		static State& mean_error = NetworkFactory::NewState(0.0);
   		

   		/** Link creation */
   		static Link& l = NetworkFactory::NewLink<FirstOrderLowPassFilter>(error,mean_error);
		l.parameters.get<double>("p0") = 0.5;
		
		static Link& l1 = NetworkFactory::NewLink<LinkAdd>(ydes, error);
		static Link& l2 = NetworkFactory::NewLink<LinkSub>(y, error);
		static Link& l3 = NetworkFactory::NewLink<LinkD1>(error, u);
		l3.parameters.get<double>("p0") = 0.01;
	    static Link& l4 = NetworkFactory::NewLink<LinkD0>(error, u);   
	    l4.parameters.get<double>("p0") = 0.1;
	    static Link& l5 = NetworkFactory::NewLink<LinkI1>(error, u);
	    l5.parameters.get<double>("p0") = 0.1;
		static Link& l6 = NetworkFactory::NewLink<LinkAdd>(u,y);

		/** Loop */
		std::ofstream myfile;
		myfile.open ("out.txt");
		int counter = 0;
		while (true){
			NetworkManager::Update();
			myfile	
				<< std::setprecision(3) << y(0) << "\t" 
				<< std::setprecision(3) << u(0) << "\t"
				<< std::setprecision(3) << error(0) << "\t"
				<< std::setprecision(3) << l3.link.transferFunction(0) << "\t"
				<< std::setprecision(3) << l4.link.transferFunction(0) << "\t"
				<< std::setprecision(3) << l5.link.transferFunction(0) << "\t"
				<< std::setprecision(3) << ydes.transferFunction(0) << "\t"
				<< std::setprecision(3) << abs(error(0)) << "\t" 
				<< std::setprecision(3) << abs(mean_error(0)) << std::endl;
			if(counter > 10 && (abs(mean_error(0)) < 0.05 || (counter > 3000) ))
				break;
			counter++;
		}
		myfile.close();

		REQUIRE( counter < 1000);
		REQUIRE( counter > 1);

  	}
}