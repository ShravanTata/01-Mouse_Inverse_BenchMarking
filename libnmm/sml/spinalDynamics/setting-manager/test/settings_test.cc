#include "catch.hpp"

#include <cstdio>
#include <cstdlib>
#include <iostream>          

#include <string>
#include <settings.hh>


using namespace std;
TEST_CASE( "Settings", "[settings]" ) {
	
    SECTION( "Add, Read" ) {
    		Settings::get<int>("Hello") = 1;
            Settings::get<double>("Hello") = 1.0;
            Settings::get<string>("Hello") = "1";

            REQUIRE ( Settings::get<int>("Hello") == 1);
            REQUIRE ( Settings::get<double>("Hello") == 1.0);
            REQUIRE ( Settings::get<string>("Hello") == "1");
    }
    SECTION( "Load from string" ) {
            string str = 
            "- settings: int\n"
            "  a: 1\n"
            "  b: 2\n"
            "- settings: double\n"
            "  a: 1.0\n"
            "  b: 2.0\n"
            "- settings: string\n"
            "  a: 1\n"
            "  b: 2\n";

            Settings::addListFromStream<string>(str);

            REQUIRE ( Settings::get<int>("a") == 1);
            REQUIRE ( Settings::get<double>("a") == 1.0);
            REQUIRE ( Settings::get<string>("a") == "1");
    }
}
