#include <stdio.h>
#include <sml/sml.hh>
#include "../sources/EkerbergMouse.hh"
#include <string>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>




using namespace std;

CentralClock * centralclock;
double debug=false;
std::map<std::string, double_int_string> Settings::sets;

extern SmlParameters parameters;

int main(int argc, char* argv[])
{
    
    /**__**/ cout << "1) Loading settings...............";

	if(argc >= 2)
		Settings::filename = argv[1];
	if(argc >= 3){
		Settings::prefix = "_";
		Settings::prefix += argv[2];
	}
	cout << Settings::prefix << endl;
	Settings settings;
	if(argc >= 4){
		Settings::set<std::string>("robot_name", argv[3]);
	}
    Settings::set<int>("notf_trunklean",1);
	Settings::set<int>("buggyJwang_changes",0);
		
		
    cout << Settings::sets["experiment"] << endl;
    /**__**/ cout << "Ok" << endl;
	if(Settings::get<int>("log") == 2){
		ofstream out("../../../log/webots_log.txt", ios::app);
		streambuf *coutbuf = cout.rdbuf(); //save old buf
		cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt!
	}
	srand(time(0));
	
	
    /**__**/ cout << "3) Create CentralClock...............";
	centralclock = new CentralClock(parameters[1]["freq_change"]);
	/**__**/ cout << "Ok" << endl;
    /**__**/ cout << "4) Create Robot...............";
    EkerbergMouse *MrGinggle = new EkerbergMouse();
    
    webots::Supervisor* supervisor = MrGinggle;
    //eventManager = Regis->eventManager;
    /**__**/ cout << "Ok" << endl;

	bool endExp = false;
	while(true) {
		MrGinggle->step();
		supervisor->step(Settings::get<int>("time_step"));

	    if(endExp){
		    supervisor->simulationQuit(EXIT_SUCCESS);    	
	    }
	}  

	return 0;
}
