#include <network_manager.hh>

/*
Using Catch test suits

Documentatino can be found at https://github.com/philsquared/Catch/blob/master/docs/tutorial.md

*/
#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

int main( int argc, char* const argv[] )
{
  // global setup...
  NetworkManager::InitClock(); // initialise global's network clock

  int result = Catch::Session().run( argc, argv );

  // global clean-up...

  return result;
}