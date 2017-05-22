#include "ParameterManager.hh"
#include "Settings.hh"

extern SmlParameters parameters;

using namespace std;

/**
 * Load parameters from launching gate if the file is not found the launching gate at 1.3 m/s is used
 * 
 */
void ParameterManager::loadLaunchingReflexParameters(string name){
	cout << "External Reflex parameter loading :" << name << endl;
	string path = Settings::get<string>("config")+"launching_gate/" + name.c_str();
	
	parameters.loadParam_fromfile(path,0);
}
/**
 * Load parameters from 
 * - the optimization framework if present 
 * - and from the file parameters.txt if not
 * 
 */
void ParameterManager::loadCpgParameters(){
	// load parameters from file
	loadCpgParametersFromFile(Settings::get<string>("config_cpg_parameters"));
	// overwrite parameters coming from optimization framework
	if(Settings::isOptimization()){
		loadCpgParametersFromOpti();
	}
		
}
void ParameterManager::loadCpgParametersFromOpti(){
	cout << "External CPG parameter loading from optimization framework" << endl;
    parameters.loadParam_fromopti(0);
    parameters.loadParam_fromopti(1);
    parameters.loadParam_fromopti(2);
    parameters.loadParam_fromopti(4);
    parameters.loadParam_fromopti(5);
    parameters.loadParam_fromopti(6);
    parameters.loadParam_fromopti(7);
    
	
}
void ParameterManager::loadCpgParametersFromFile(std::string file){
	cout << "External CPG parameter loading from file " << file << endl;
	string path = Settings::get<string>("config")+"cpg_gate/"+file;
    parameters.loadParam_fromfile(path,0);
	parameters.loadParam_fromfile(path,1);
	parameters.loadParam_fromfile(path,2);
	
}
void ParameterManager::loadReflexParameters(){
	if(Settings::isOptimization()){
		loadReflexParametersFromOpti();
	}
    else{
        loadReflexParametersFromFile(Settings::get<string>("config_feedback_parameters"));
	}
}
void ParameterManager::loadReflexParametersFromOpti(){
	cout << "External Reflex parameter loading from optimization framework" << endl;
	
	parameters.loadParam_fromopti(0);
}

void ParameterManager::loadReflexParametersFromFile(std::string file){
	cout << "External Reflex parameter loading from file " << file << endl;
	string path = Settings::get<string>("config")+"fullReflex_gate/"+file;
	
	parameters.loadParam_fromfile(path,0);
    parameters.loadParam_fromfile(path,1);
}


