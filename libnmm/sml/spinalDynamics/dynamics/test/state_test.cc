#include "catch.hpp"

#include <cstdio>
#include <cstdlib>
#include <iostream>          

#include <string>
#include <setting-manager/settings.hh>
#include <state.hh>
#include <network_manager.hh>
#include <network_factory.hh>
#include <link_library.hh>

TEST_CASE( "State Operations", "[vector]" ) {
    State& x0 = *(new State(10));
    REQUIRE ( x0.dimension() == 10);

	State& x1 = *(new State(2.0,1.0));
	State& x2 = *(new State(2.0,1.0));



	REQUIRE ( x1.dimension() == x2.dimension() );

	REQUIRE ( x1(0) == 2.0 );
	REQUIRE ( x1(1) == 1.0 );
    

    SECTION( "Copy assignment" ) {
    		State& x3 = *(new State(x1));
    		REQUIRE ( x3(0) == x1(0) );
    		REQUIRE ( x3(1) == x1(1) );

    		x3(0) += 1;

    		REQUIRE ( x3(0) != x1(0));
    }

    // SECTION( "Copy constructor" ) {
    // 		State x3(x1);
    // 		REQUIRE ( x3(0) == x1(0) );
    // 		REQUIRE ( x3(1) == x1(1) );

    // 		x3(0) += 1;

    // 		REQUIRE ( x3(0) != x1(0));
    // }

    SECTION( "Addition" ) {
    		State x3 = x1+x2;
    		REQUIRE ( x3(0) == x1(0)+x2(0) );
    		REQUIRE ( x3(1) == x1(1)+x2(1) );

    		State x4(x1);
    		x4+=x2;
    		REQUIRE ( x4(0) == x1(0)+x2(0) );
    		REQUIRE ( x4(1) == x1(1)+x2(1) );
    }
    SECTION( "Substraction" ) {
    		State x3 = x1;
            x3-=x2;
    		REQUIRE ( x3(0) == x1(0)-x2(0) );
    		REQUIRE ( x3(1) == x1(1)-x2(1) );

    		State x4(x1);
    		x4=-x2;

    		REQUIRE ( x4(0) == -x2(0) );
    		REQUIRE ( x4(1) == -x2(1) );

    }
    SECTION( "Multiplication by a scalar" ) {
    		State x3 = x1*5;
    		REQUIRE ( x3(0) == x1(0)*5 );
    		REQUIRE ( x3(1) == x1(1)*5 );

    		State x4(x2);
    		x4*=5;
    		REQUIRE ( x4(0) == 5*x2(0) );
    		REQUIRE ( x4(1) == 5*x2(1) );
    }
    SECTION( "Transfer function") {
            State x1 = State(2.0,1.0);
            std::function<Real (const Real&)> f = [](const Real& x) -> Real 
            {
                return 2*x;
            };
            State x2 = x1;

            REQUIRE( x2.transferFunction(0) == x2(0) );
            REQUIRE( x2.transferFunction(1) == x2(1) );

            x2.transferFunction.set(new State::TransferFunctions::AnyFunc(f));
            REQUIRE( x2.transferFunction(0) == f(x2(0)) );
            REQUIRE( x2.transferFunction(1) == f(x2(1)) );
            REQUIRE( x2.transferFunction(0) == x1(0)*2 );
            REQUIRE( x2.transferFunction(1) == x1(1)*2 );


    }

    SECTION( "Sate Less class test"){
            State& a = NetworkFactory::NewState(1.0);
            State& b = NetworkFactory::NewState(1.0);
            State& c = NetworkFactory::NewState(1.0);

            State& d_state = NetworkFactory::NewState(0.0);
            State& d_state_less = NetworkFactory::NewStateLess(0.0);

            Link& l1 = NetworkFactory::NewLink<LinkAdd>(a,d_state);
            Link& l2 = NetworkFactory::NewLink<LinkAdd>(b,d_state);
            Link& l3 = NetworkFactory::NewLink<LinkAdd>(c,d_state);

            Link& l4 = NetworkFactory::NewLink<LinkAdd>(a,d_state_less);
            Link& l5 = NetworkFactory::NewLink<LinkAdd>(b,d_state_less);
            Link& l6 = NetworkFactory::NewLink<LinkAdd>(c,d_state_less);            

            REQUIRE(d_state(0) == 0.0);
            NetworkManager::Update();
            REQUIRE(d_state(0) == 3.0);
            REQUIRE(d_state_less(0) == 3.0);
            NetworkManager::Update();
            REQUIRE(d_state(0) == 6.0);
            REQUIRE(d_state_less(0) == 3.0);
    }

    SECTION( "Memory management") {

            static State& y = NetworkFactory::NewState(1.0); // output of the system

            
            y(0) = 2.0;
            NetworkManager::Update();

            //REQUIRE( y(0,0) == 1.0);
            REQUIRE( y(0,0) == 2.0);
            using namespace std;

            
            for ( double i = 0.0; i <= 30; i+=1.0){
                y(0) = i;
                NetworkManager::Update();
            }
            

            REQUIRE( y(0,0) == 30.0);
            REQUIRE( y(0,MEMORY) == 30.0-1.0*MEMORY);

            // auto f = [](const State & X) -> State 
            // {
            //  return 2*X+1; 
            // };

            // y.transferFunction.set(f);

            // REQUIRE ( y.transferFunction(0,MEMORY) == 2*(y(0,MEMORY))+1 );






    }
}