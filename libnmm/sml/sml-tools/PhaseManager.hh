#ifndef __Phase_HH__
#define __Phase_HH__

#include <vector>
#include <sstream>
#include <fstream>

#include <sml/musculoSkeletalSystem/CentralClock.hh>
#include <sml/sml-tools/Settings.hh>
#include <sml/sml-tools/ParameterManager.hh>
#include <sml/musculoSkeletalSystem/Sml.hh>
#include <sml/musculoSkeletalSystem/GeyerSml.hh>




/*!
 * \file Phase.hh
 * \author efx
 * \version 0.1
 * \brief Experiment phase description
 */
using namespace std;

class Phase{
public:
    bool last;
    string name;
    Sml * sml;
    int nb_cycle;
    bool restart_state;
    Phase(string name, Sml * sml, int nb_cycle);
    Phase(string name, Sml * sml, int nb_cycle, string option);
    void print_start();
    void print_end();
    virtual bool rule(vector<Phase *>::iterator * it) = 0;
    virtual void loadParameters() = 0;
    
};
class InitialPhase : public Phase{
public:
    InitialPhase(Sml * sml);
    bool rule(vector<Phase *>::iterator * it);
    void loadParameters();
};

class InitialPhaseFinder : public Phase{
public:
    InitialPhaseFinder(Sml * sml);
    bool rule(vector<Phase *>::iterator * it);
    void loadParameters();
};
class FullReflexPhase : public Phase{
public:
    string parameters;
    FullReflexPhase(Sml * sml, string parameters, int nb_cycle);
    FullReflexPhase(Sml * sml, int nb_cycle);
    FullReflexPhase(Sml * sml, string parameters, int nb_cycle, string option);
    FullReflexPhase(Sml * sml, int nb_cycle, string option,string parameters);
    FullReflexPhase(Sml * sml, int nb_cycle, string option);
    bool rule(vector<Phase *>::iterator * it);
    void loadParameters();
};
class SemiReflexPhase : public Phase{
public:
    bool end;
    string parameters;
    SemiReflexPhase(Sml * sml, std::string parameters, int nb_cycle);
    SemiReflexPhase(Sml * sml, int nb_cycle);
    SemiReflexPhase(Sml * sml, string parameters, int nb_cycle, string option);
    SemiReflexPhase(Sml * sml, int nb_cycle, std::string option);
    SemiReflexPhase(Sml * sml, int nb_cycle, std::string parameters, std::string option);
    bool rule(vector<Phase *>::iterator * it);
    void loadCpg();
    void loadParameters();
};



class PhaseManager {
public:
	static vector<Phase *> phases;
	static vector<Phase *>::iterator phase;
	static void init(Sml* sml);
	static void update();
};

/**

THIS CLASS IS NOT NECESSARY AS SOON AS WE CAN IMPLEMENT OUR OWN DISPATCHER
--> DRAWBACK: We are then dependent on liboptimization. not a big deal really
**/
/*
class SystematicStudyPhase : public Phase{
public:
	class Step {
	public:
		Step(){};
		virtual bool update() = 0;
		virtual void goNext(vector<Step *>::iterator * it){
			(*it)++;
		}
	};
	class Step_loadParameters : public Step {
	public:
		int step;
		vector<double*> parameters_value;
		vector<string> parameters_name;
		vector<double> parameters_end;

		int parameter_number;
		class Parameter2{
		public:
			double* value;
			double initial_value;
			double end_value;
			string name;
			int current_step;
			int step;
			Parameter2(double * value, double end, string name, int step);
//				cout << name << " " << initial_value << endl;
			};
			void init(){
				*value = initial_value;
				current_step = -1;
				cout << "\tparameter " << name << " loaded (=" << get() <<  ")" << endl;
			}
			bool increment(){
				current_step++;
				if(current_step > step)
					return false;
				*value = initial_value+get_currentStepSize()*current_step;
				cout << "\t\tstep " << current_step << "/" << step << " ( " << *value << " ) " << endl;
				return true;
				
			}
			double get(){
				return *value;
			}
			double get_currentStepSize(){
				return (end_value-initial_value)/(step);
			}
		};
		std::vector<Parameter2*> parameters;
		std::vector<Parameter2*>::iterator parameter;

		Step_loadParameters(vector<double*> parameters_value,vector<string> parameters_name,vector<double> parameters_end,int step);
				step(step), 
				parameters_value(parameters_value), 
				parameters_name(parameters_name), 
				parameters_end(parameters_end)
		{
			if(parameters_value.size() != parameters_end.size())
				cerr << "Error in Step_loadParameters constructor, parameters and end range size does not match " << endl;
			parameter_number = parameters_value.size();
			for(unsigned int i=0; i<parameters_value.size();i++)
			{
				parameters.push_back(new Parameter2(parameters_value[i],parameters_end[i], parameters_name[i],step));
			}
			parameter = parameters.begin();
		}
		bool update(){
			cout << "Step: load parameters:";
			// If no more steps, get next parameter
			if(!(*parameter)->increment())
				return get_nextParameter();
			else
				cout << "\t" << (*parameter)->name << endl;

			return true;
		}
		bool get_nextParameter(){
			(*parameter)->init();
			// If no more parameters return false
			if(++parameter == parameters.end()){
				cout << "\tno more parameter, finished" << endl;
				return false;
			}
			else{
				(*parameter)->increment();
				return true;
			}
		}
		int get_parameterNum(){
			return parameter - parameters.begin();
		}
		string get_parametersState(){
			string str;
			stringstream ss;
			for(auto& p: parameters)
			{
				ss << p->current_step << "\t";
			}
			for(auto& p: parameters)
			{
				ss << p->get() << "\t";
			}
			return ss.str();
		}
		bool isStateReached(std::vector<int> state){
			for(unsigned int i=0; i<state.size();i++)
				if(state[i] != parameters[i]->current_step)
					return false;
			return true;
		}
	};

	class Step_loadParametersRecursive : public Step_loadParameters {
	public:

		Step_loadParametersRecursive(vector<double*> parameters_value,vector<string> parameters_name,vector<double> parameters_end,int step);
				Step_loadParameters(parameters_value,parameters_name,parameters_end,step)
		{
		}
		bool update(){
			cout << "Step: load parameters" << endl;
			if(parameter+1 == parameters.end()){
				if(!(*parameter)->increment()){
					(*parameter)->init();
					parameter--;
					return update();
				}
				else{
					//parameter++;
					//return update();
				}
			}
			else{
				if(!(*parameter)->increment()){
					if(parameter == parameters.begin()){
						return false;
					}
					(*parameter)->init();
					parameter--;
					return update();
				}
				else{
					parameter++;
					return update();
				}
				
			}
			return true;
		}

	};
	
	class Step_waitStabilization : public Step {
	public:
		bool start;
		Step_waitStabilization();
		bool update(){
			if(start){
			cout << "Step: wait stabilization" << endl;
			eventManager->resetState();
			eventManager->set<bool>(STATES::IS_LAUNCHING_PHASE,false);
			start = false;
			}
			return true;
		}
		void goNext(vector<Step *>::iterator * it){
			if(eventManager->get<bool>(STATES::STEADY_STATE)){
				(*it)++;
				start = true;
			}
		}
	};
	class Step_recordData : public Step {
	public:
		SystematicStudyPhase* phase;
		Step_loadParameters* loadParameters;
		int step_number;
		int step_start;
		bool start;
		
		Step_recordData(Step_loadParameters* loadParameters, int step_number=10, SystematicStudyPhase* phase=0);
		bool update(){
			if ( start ){
				cout << "Step: recording data" << endl;
				step_start = eventManager->get<int>(STATES::CYCLE_COUNT);
				start = false;
			}
			return true;
		}
		void goNext(vector<Step *>::iterator * it){
			if(step_start + step_number < eventManager->get<int>(STATES::CYCLE_COUNT)){
			//if(true){
				(*it)++;
				start = true;
				cout << "\trecord state to file : " << Settings::get<string>("config_systematic_parameters")+Settings::get<string>("systematic_search_prefix") << endl;
				phase->saveState();
				//recordToFile();
			}
		}
        
		void recordToFile(){ 
			//TODO
			// save robot_state 
			// save phase_state
			string folder = "../../../systematicsearch_data/";
			cout << counter << " " << loadParameters->get_parametersState() << endl;
			//return;
			ofstream myfile_paramState;
			ofstream myfile_fitness;
			
			myfile_paramState.open (folder+Settings::get<string>("config_systematic_parameters")+Settings::get<string>("systematic_search_prefix")+"_state", ios::out | ios::app); 
			myfile_fitness.open (folder+Settings::get<string>("config_systematic_parameters")+Settings::get<string>("systematic_search_prefix")+"_fitness", ios::out | ios::app); 
				
			//          * format
			//             0:  feedback name
			//             1:  parameter number
			//             2:  parameter current step
			//             3:  parameter value
			//             4:  falled 
			//             5:  falled reason
			//             6:  stabilized_after
			//             7: mean_speed
			//             8: instantaneous speed
			//             9: energy
			//             10: energy_overextension
			//             11: distance
			//             12: duration
			//             13: double stance duration left
			//             14: swing duration left
			//             15: stance duration left
			//             16: double stance duration right
			//             17: swing duration right
			//             18: stance duration right
			//             19: cycle length
			//             20: step length left
			//             21: step length right
			
			myfile_paramState << 
				counter << "\t" << loadParameters->get_parametersState() <<
				eventManager->get<bool>(STATES::STAY_IN_LOOP) << "\t" << 
				eventManager->get<int>(STATES::STOP_REASON) << "\t" <<
			endl;
			myfile_fitness << 
				eventManager->get<int>(STATES::STEADY_STATE_REACHEDAT) << "\t" <<
				eventManager->get<double>(STATES::MEAN_VELOCITY) << "\t" <<
				eventManager->get<double>(STATES::IN_VELOCITY_FILTERED) << "\t" <<
				eventManager->get<double>(STATES::ENERGY) << "\t" <<
				eventManager->get<double>(STATES::ENERGY_OVEREXTENSION) << "\t" <<
				eventManager->get<double>(STATES::DISTANCE) << "\t" <<
				eventManager->get<double>(STATES::TIME)-eventManager->get<double>(STATES::TIME_AT_RESET) << "\t" <<
				eventManager->get<double>(STATES::MEAN_LEFT_STANCE_END_DURATION) << "\t" <<
				eventManager->get<double>(STATES::MEAN_LEFT_SWING_DURATION) << "\t" <<
				eventManager->get<double>(STATES::MEAN_LEFT_STANCE_DURATION) << "\t" <<
				eventManager->get<double>(STATES::MEAN_RIGHT_STANCE_END_DURATION) << "\t" <<
				eventManager->get<double>(STATES::MEAN_RIGHT_SWING_DURATION) << "\t" <<
				eventManager->get<double>(STATES::MEAN_RIGHT_STANCE_DURATION) << "\t" <<
				eventManager->get<double>(STATES::MEAN_CYCLE_LENGTH) << "\t" <<
				eventManager->get<double>(STATES::MEAN_LEFT_STEP_LENGTH) << "\t" <<
				eventManager->get<double>(STATES::MEAN_RIGHT_STEP_LENGTH) << "\t" <<
			endl;
			myfile_paramState.close();
			myfile_fitness.close();
			
		}
	};


	static int counter;
	vector<Step *> steps;
	vector<Step *>::iterator step;
	std::vector<int> previousSol_stage;
	bool previousSol;
	string parameters;
    SystematicStudyPhase(Sml * sml, int nb_cycle, std::string option)
        :Phase("SystematicStudyPhase",exp, nb_cycle, option), parameters(""){counter=0;	};
	SystematicStudyPhase(Sml * sml, string parameters, int nb_cycle, std::string option)
        :Phase("SystematicStudyPhase",exp, nb_cycle, option), parameters(parameters){counter=0;	};
    void loadParameters(){
		double step_systematic = Settings::get<int>("systematic_search_step_number"); // number of step between default parameter value and end parameter value
		vector<double *> parameters_value;
		vector<double> parameters_end;
		vector<string> parameters_name;
		
		std::vector<std::string> strs; // the vector containing the key->value pair
		if(parameters=="")
			parameters = Settings::get<string>("config_systematic_parameters");
		
		string file = Settings::get<string>("config")+"systematic_search/" + parameters;
		ifstream myfile (file);
		string line ; // the string containing the line
		if (myfile.is_open()){
			while( std::getline( myfile, line ) )
			{
				boost::split(strs, line, boost::is_any_of("\t "));
				parameters_name.push_back(strs[1]);
				double * value;
				if(strs[0] == "level0")
					value = &smlparameters[0][strs[1]];
				else if(strs[0] == "level1")
					value = &smlparameters[1][strs[1]];
				else if(strs[0] == "level2")
					value = &smlparameters[2][strs[1]];
				else if(strs[0] == "level3")
					value = &smlparameters[3][strs[1]];
				else
					cout << "unknown parameter set " << strs[0] << " will not work" << endl;
				
				parameters_value.push_back(value);
				
				if(strs.size()<3){
					cout << "error no end value set for parameters : " << strs[1] << endl;
				}
				else{
					parameters_end.push_back(convertToDouble(strs[2]));
				}
				if(strs.size()==4){
					*value = convertToDouble(strs[3]);
				}
			}
			myfile.close();
		}
		else{
			cout<<"ERROR: unable to open file '" << file << "'. In:Parameters.cc, constructor(string, string)"<<endl;
			cout<< "\t loading sample parameter " << endl;
			parameters_value = { &smlparameters[1]["freq_change"],&smlparameters[1]["stance_end"] };
			parameters_end = { 1.6, -3.0};
			parameters_name = { "freq_change", "stance_end"};
		}
		
		if(Settings::get<string>("systematic_search_type") == "normal")
			steps.push_back(new Step_loadParameters(parameters_value, parameters_name, parameters_end, step_systematic));
		else
			steps.push_back(new Step_loadParametersRecursive(parameters_value, parameters_name, parameters_end, step_systematic));
	}
    void init(){
		loadParameters();
		steps.push_back(new Step_waitStabilization());
		
		double step_number = Settings::get<int>("systematic_search_recordStep"); // number of step during which we record data
		steps.push_back(new Step_recordData((Step_loadParameters*)steps[0],step_number,exp));
		eventManager->doIt<SystematicStudyPhase>(&(eventManager->onSimulationEnd), &SystematicStudyPhase::saveState, *exp);
		// Do we have saved data ?
		checkIfPreviousState();
    }
    void checkIfPreviousState(){
		string folder = "../../../systematicsearch_data/";
		string file = folder+Settings::get<string>("config_systematic_parameters")+Settings::get<string>("systematic_search_prefix")+"_saved";
		ifstream myfile (file);
		cout << file << endl;
		string line ; // the string containing the line
		vector<string> strs; // the vector containing the key->value pair
		if (myfile.is_open()){
			cout << "previous systematic search found.. continuing" << endl;
			previousSol = true;
			getline( myfile, line );
			std::vector<string> strs;
			boost::split(strs, line, boost::is_any_of("\t "));
			for(auto& str:strs){
				if(str!=""){
				std::stringstream ss(str);
				int x;
				ss >> x;  
				previousSol_stage.push_back(x);
				}
			}
			myfile.close();
		}
		else
		{
			cout << "no previous systematic search found.. start new" << endl;
			previousSol = false;
		}
		
	}
	void saveState(){
		string folder = "../../../systematicsearch_data/";
		string file = folder+Settings::get<string>("config_systematic_parameters")+Settings::get<string>("systematic_search_prefix")+"_saved";
		cout << "saving systematicSearch state" << endl;
		ofstream* myfile = new ofstream();
		remove(file.c_str());
		myfile->open(file);
		
		
		for(auto& kv:((Step_loadParameters*)steps[0])->parameters){
			*myfile << kv->current_step << " ";
		}
		myfile->close();
		
		// recordCurrentData
		cout << "recording data" << endl;
		((Step_recordData*)steps[steps.size()-1])->recordToFile();
	}
    bool rule(vector<Phase *>::iterator * it){
		static bool end = false;
        if(exp->body->right_foot->justTouchTheGround() && eventManager->get<int>(STATES::CYCLE_COUNT)>=nb_cycle && !end)
        {
            exp->print_start();
            end = true;
			init();
			step = steps.begin();
			exp->print_end();
        }
        if(end)
        {
			if(previousSol)
			{
				while(previousSol){
					((Step_loadParameters*)steps[0])->update();
					cout << ((Step_loadParameters*)steps[0])->get_parametersState() << endl;
					counter++;
					if(((Step_loadParameters*)steps[0])->isStateReached(previousSol_stage)){
						previousSol = false;
					}
				}
			}
			else{
				if(!(*step)->update()){
					(*it)++;
					Settings::set<int>("restart_simulation_at_end",0);
					eventManager->set<bool>(STATES::STAY_IN_LOOP,false);
				}
				(*step)->goNext(&step);
				if(step == steps.end()){
					counter++;
					step = steps.begin();
				}
			}
        }
        return false;
    }
};
int SystematicStudyPhase::counter = 0;
*/
#endif