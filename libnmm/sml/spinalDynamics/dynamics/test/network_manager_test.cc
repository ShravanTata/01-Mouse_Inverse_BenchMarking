#include "catch.hpp"

#include <cstdio>
#include <cstdlib>
#include <iostream>


#include <state.hh>
#include <link_library.hh>
#include <network_factory.hh>
#include <network_manager.hh>

#include <system.hh>

using namespace std;
TEST_CASE( "Network Management", "[networkManager]") {

	SECTION( "State creation / deletion"){
		UInt fullNetSize = NetworkManager::GetNetwork().fullNet.stateNum();
		UInt subNet_startfromlessState = NetworkManager::GetNetwork().subNet_startfromlessState.stateNum();
		UInt subNet_dontstartfromlessState = NetworkManager::GetNetwork().subNet_dontstartfromlessState.stateNum();


		State& s = NetworkManager::AddState(new State(0.0));
		State& sl = NetworkManager::AddState(new StateLess(0.0));

		REQUIRE( NetworkManager::GetNetwork().fullNet.stateNum() == fullNetSize+2 );
		REQUIRE( NetworkManager::GetNetwork().subNet_dontstartfromlessState.stateNum() ==  subNet_dontstartfromlessState + 1);
		REQUIRE( NetworkManager::GetNetwork().subNet_startfromlessState.stateNum() == subNet_startfromlessState + 1 );

		NetworkManager::RemoveState(&s);
		NetworkManager::RemoveState(&sl);
		
		REQUIRE( NetworkManager::GetNetwork().fullNet.stateNum() == fullNetSize );
		REQUIRE( NetworkManager::GetNetwork().subNet_startfromlessState.stateNum() == subNet_startfromlessState );
		REQUIRE( NetworkManager::GetNetwork().subNet_dontstartfromlessState.stateNum() == subNet_dontstartfromlessState );

		//TODO 1 : simplify writing
		//State& = NetworkManager::GetNetwork().NewState(0.0);
		//StateLess& sl = NetworkManager::GetNetwork().NewStateLess(0.0);
		//TODO 2 : overload NetworkManager::GetNetwork().RemoveState with NetworkManager::GetNetwork().RemoveState(UInt)
	}
	SECTION( "NetworkFactory for state creation"){
		UInt fullNetSize = NetworkManager::GetNetwork().fullNet.stateNum();
		UInt subNet_startfromlessState = NetworkManager::GetNetwork().subNet_startfromlessState.stateNum();
		UInt subNet_dontstartfromlessState = NetworkManager::GetNetwork().subNet_dontstartfromlessState.stateNum();


		State& s1 = NetworkFactory::NewState(0.0);
		State& s2 = NetworkFactory::NewStateLess(0.0,1.0,2.0,3.0);

		REQUIRE( s2(0) == 0.0);
		REQUIRE( s2(1) == 1.0);
		REQUIRE( s2(2) == 2.0);
		REQUIRE( s2(3) == 3.0);

		REQUIRE( NetworkManager::GetNetwork().fullNet.stateNum() == fullNetSize+2 );
		REQUIRE( NetworkManager::GetNetwork().subNet_startfromlessState.stateNum() == subNet_startfromlessState+1 );
		REQUIRE( NetworkManager::GetNetwork().subNet_dontstartfromlessState.stateNum() == subNet_dontstartfromlessState+1 );
	}

	SECTION( "Simple link creation"){
		State& a = NetworkFactory::NewState(1.0);
		State& b = NetworkFactory::NewState(0.0);

		Link& l1 = NetworkManager::AddLink(new StandardLink(a,b));

		REQUIRE (b(0) == 0.0);
		REQUIRE (a(0) == 1.0);
		for ( int i=1; i<=10;i++)
			NetworkManager::Update();
		
		REQUIRE (b(0) == 10.0);
		REQUIRE (a(0) == 1.0);
	}
	SECTION( "Sate Less Link Test"){
		/**
			  +
			A ------------> B(less_state)
			^	 			|+
			|				|
			-----------------

		*/

		State& a = NetworkFactory::NewState(1.0);
		State& b = NetworkFactory::NewStateLess(0.0);

		Link& l1 = NetworkManager::AddLink(new StandardLink(a,b));
		Link& l2 = NetworkFactory::NewLink<StandardLink>(b,a);


		REQUIRE ( a(0) == 1.0 );
		REQUIRE ( b(0) == 0.0 );
		NetworkManager::Update();
		REQUIRE ( a(0) == 1.0 );
		REQUIRE ( b(0) == 1.0 );
		NetworkManager::Update();
		REQUIRE ( a(0) == 2.0 );
		REQUIRE ( b(0) == 1.0 );
		NetworkManager::Update();
		REQUIRE ( a(0) == 3.0 );
		REQUIRE ( b(0) == 2.0 );
		NetworkManager::Update();
		REQUIRE ( a(0) == 5.0 );
		REQUIRE ( b(0) == 3.0 );
		NetworkManager::Update();
		REQUIRE ( a(0) == 8.0 );
		REQUIRE ( b(0) == 5.0 );
		NetworkManager::Update();
		REQUIRE ( a(0) == 13.0 );
		REQUIRE ( b(0) == 8.0 );
	}
	std::function<Real (const Real&)> fcos = [](const Real& x) -> Real
    {
    	static Real t_start = NetworkManager::GetNetwork().getClock().getTime();
    	Real t = NetworkManager::GetNetwork().getClock().getTime();
     	return cos(t-t_start);
    };

	SECTION( "No influence on order"){
		static State& temp = NetworkFactory::NewState(10.0); // output of the system
   		static State& y = NetworkFactory::NewState(10.0); // output of the system
   		static State& ydes = NetworkFactory::NewState(2.0); // what we want ( a sine wave)

        //ydes.transferFunction.set(new State::TransferFunctions::AnyFunc (fcos));
   		static State& error = NetworkFactory::NewStateLess(0.0);
		
		static Link& l1 = NetworkFactory::NewLink<LinkAdd>(ydes, error);
		static Link& l2 = NetworkFactory::NewLink<LinkSub>(y, error);

		static Link& l3 = NetworkFactory::NewLink<LinkD0>(error, y);
		l3.parameters.get<double>("p0") = 0.1;

		REQUIRE( l1.link(0) == 2.0);
		REQUIRE( l2.link(0) == 10.0);
		REQUIRE( l3.link(0) == 0.0);
		REQUIRE( error(0) == 0.0);
		REQUIRE( y(0) == 10.0);
		NetworkManager::Update();
		REQUIRE( l1.link(0) == 2.0);
		REQUIRE( l2.link(0) == -10.0);
		REQUIRE( error(0) == -8.0);
		REQUIRE( y(0) == 10.0);
		NetworkManager::Update();
		REQUIRE( l2.link(0) == -10.0);
		REQUIRE( error.transferFunction(0) == 2.0-10.0);
		REQUIRE( l3.link(0) == 0.1*error(0));


		REQUIRE( error(0) == 2.0-10.0);

		REQUIRE( l3.link(0) == 0.1*error(0));
		REQUIRE( y(0) == 10.0 + 0.1*(2.0-10.0) );
	}

	static int count = 0;
	std::function<Real (const Real&)> printF = [&](const Real& x) -> Real
    {

    	count += 1;
    	return x;
    };

	SECTION("Network"){
		static State& a = NetworkFactory::NewState(0.0);
		static State& b = NetworkFactory::NewState(0.0);
		static Link& l = NetworkFactory::NewLink<LinkAdd>(a, b);

		

		a.transferFunction.set(new State::TransferFunctions::AnyFunc (printF));
		

		REQUIRE ( count == 0 );
		NetworkManager::Update();
		REQUIRE ( count == 1 );
		NetworkManager::RemoveLink(&l);
		NetworkManager::Update();
		REQUIRE ( count == 1 );

		NetworkManager::StartTemporaryNetwork();
		static Link& l2 = NetworkFactory::NewLink<LinkAdd>(a, b);
		NetworkManager::UpdateTemporaryNetwork();
		REQUIRE ( count == 2 );
		NetworkManager::UpdateTemporaryNetwork();
		NetworkManager::Update();
		REQUIRE ( count == 4 );
		NetworkManager::EndTemporaryNetwork();
		NetworkManager::UpdateTemporaryNetwork();
		NetworkManager::Update();
		REQUIRE ( count == 5 );
		NetworkManager::RemoveLink(&l2);
	};

    SECTION("Local Network"){
    	Network* localNet = new Network();
    	localNet->initClock();
		static State& c = NetworkFactory::NewState(localNet,0.0);
		static State& d = NetworkFactory::NewState(localNet,0.0);
		static Link& l3 = NetworkFactory::NewLink<LinkAdd>(localNet, c, d);

		
		count = 0;
		c.transferFunction.set(new State::TransferFunctions::AnyFunc (printF));
		

		REQUIRE ( count == 0 );
		NetworkManager::Update();
		localNet->update();
		REQUIRE ( count == 1 );
		localNet->removeLink(&l3);
		NetworkManager::Update();
		REQUIRE ( count == 1 );

		localNet->startTemporaryNetwork();
		static Link& l4 = NetworkFactory::NewLink<LinkAdd>(localNet, c, d);
		localNet->updateTemporaryNetwork();
		REQUIRE ( count == 2 );
		localNet->updateTemporaryNetwork();
		localNet->update();
		REQUIRE ( count == 4 );
		localNet->endTemporaryNetwork();
		localNet->updateTemporaryNetwork();
		localNet->update();
		REQUIRE ( count == 5 );
    };
    SECTION("Test NetworkFactory::NewState Templated method"){
    	static State& test = NetworkFactory::NewState<State>(1.0);
    	static State& test_less = NetworkFactory::NewState<StateLess>(1.0);

    	UInt dim = 10;

    	static State& test_less2 = NetworkFactory::NewEmptyState<StateLess>(dim);

    	REQUIRE(test(0) == 1.0);
    	REQUIRE(test_less(0) == 1.0);
    	NetworkManager::Update();
    	REQUIRE(test(0) == 1.0);
    	REQUIRE(test_less(0) == 0.0);

		REQUIRE(test_less.dimension() == 1);
		REQUIRE(test_less2.dimension() == 10);

    };

	SECTION( "NewTime global clock"){
		Network local_net;
		Time my_clock(local_net);
		my_clock.init();
		local_net.setClock(my_clock);



		REQUIRE(local_net.getTime() == 0.0);
		REQUIRE(local_net.getTime() == my_clock.getTime());
		local_net.update();
		REQUIRE(local_net.getTime() == my_clock.getTime());
		REQUIRE(local_net.getTime() == local_net.getTimeStep());
	}

	SECTION( "NewTime global clock : creation via the Network object"){
		Network local_net;
		//NetworkManager::InitClock(local_net);
		local_net.initClock();
		Time& my_clock = local_net.getClock();



		REQUIRE(local_net.getTime() == 0.0);
		REQUIRE(local_net.getTime() == my_clock.getTime());
		local_net.update();
		REQUIRE(local_net.getTime() == my_clock.getTime());
		REQUIRE(local_net.getTime() == local_net.getTimeStep());
	}
	SECTION( "NewTime global clock : creation via NetworkManager"){
		Network local_net;
		NetworkManager::InitClock(local_net);
		Time& my_clock = local_net.getClock();



		REQUIRE(local_net.getTime() == 0.0);
		REQUIRE(local_net.getTime() == my_clock.getTime());
		local_net.update();
		REQUIRE(local_net.getTime() == my_clock.getTime());
		REQUIRE(local_net.getTime() == local_net.getTimeStep());
	}
 }
