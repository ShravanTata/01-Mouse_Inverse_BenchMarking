#include "CentralClock.hh"
#include "Interneuron.hh"
#include "Joint.hh"
#include "Muscle.hh"
#include "Sensor.hh"

#include <sml/sml-tools/Settings.hh>
#include <boost/algorithm/string.hpp>

#include <efx/rungekutta.h>

using namespace std;
extern EventManager* eventManager;
extern CentralClock * centralclock;
extern SmlParameters parameters;
extern bool debug;



double weight_const = 0.0;

double interpolate(double pos, std::vector<double> & y){
    static double a;
    static double b;
    a=(y[ceil(pos)]-y[floor(pos)]);
    b=y[floor(pos)]-a*floor(pos);
   
    return a*pos+b;
}


double Interneuron::tf(double x){
    // tf from entities overwritten because we don't implement inhibitory connection for now:
    // --> The inhibitory connection are defined by factor given in the construction of the interneuron
    // so here we trick the transfer function.
    static double y;
    static int type;
    type = 0;
	if (no_tf)
		return -Entity::tf(-x)+Entity::tf(x);
	else
		return factor*(Entity::tf(factor*x));
}

vector<double> CPGInterneuron::dydt(double t, vector<double> y, vector<double> param, vector<double> input){
	//dydt(1) = w;
	//dydt(2) = tau*(in(y(1))-y(2))+(in(y(1)+h/2)-in(y(1)-h/2))/(h)*dydt(1);
    static double f,tau,dt,offset,amplitude;
    static int cpg_strong_sync = Settings::get<int>("cpg_strong_sync");
    static vector<double> dydt(y.size(),0.0);
	f   = param[0];
	tau = param[1];
	dt  = param[2];
	offset = param[3];
	amplitude = param[4];
	
	dydt[0] = f;
	if(cpg_strong_sync==1){
        if(fmod(y[0],1.0) >= 0.9)
            dydt[0] = 10*f*exp(-10*f*t+9-log(10));
	}
	dydt[1] = tau*(
		offset+
		amplitude*getG(y[0])-y[1])+
		amplitude*getDG(y[0])*dydt[0];
	return dydt;
}
double CPGInterneuron::getG(double val){
	static double Knot = teacher.size();
	return interpolate(fmod(val,period)*Knot,teacher);
}
double CPGInterneuron::getDG(double val){
	static double Knot = teacher_d.size();
	return interpolate(fmod(val,period)*Knot,teacher_d);
}
CPGInterneuron::CPGInterneuron(
    ACTIVE_DURING::ActiveDuring active_during, 
    int time_step,Entity * output_neuron, 
    Foot * foot, 
    string file): 
        Interneuron(output_neuron, active_during, file), 
        dt(double(time_step)/1000.0),
        t(0.0),
        tau(200.0),
        freq(1.0),
        period(1.0),
        file(file),
        foot(foot)
        {
    parameter_level2 = &parameters[2][file];
	parameter_level3 = &parameters[3][file];
    
	string path = Settings::get<string>("config")+"cpg_gate/cpg_data/" + Settings::get<string>("config_cpg_data") + "/" + file + ".txt";
    ifstream myfile (Settings::get<string>("config")+"cpg_gate/cpg_data/" + Settings::get<string>("config_cpg_data") + "/" + file + ".txt");
    string line ; // the string containing the line
    vector<string> strs; // the vector containing the key->value pair
    if (myfile.is_open()){
        getline( myfile, line );
        boost::split(strs, line, boost::is_any_of("\t "));
            offset = convertToDouble(strs[0]);
			if(offset < 0)
				factor = -1.0;

		amplitude = convertToDouble(strs[1]);
		if(strs.size()==3)
			mean = amplitude*(convertToDouble(strs[2]))+offset;
		else
			mean = 0;
		while( getline( myfile, line ) )
		{
			boost::split(strs, line, boost::is_any_of("\t "));
			if(strs.size()==2)
			{
				teacher.push_back(convertToDouble(strs[0]));
				teacher_d.push_back(convertToDouble(strs[1]));
			}
			else if(strs.size()==1)
			{
				teacher.push_back(convertToDouble(strs[0]));
				if(teacher.size()!=1)
				{
					double dt = 1./teacher.size();
					double val = (teacher[teacher.size()-1]-teacher[teacher.size()-2])/dt;
					teacher_d.push_back(val);
				}
			}
			else
				cout << "Error: number of element in CpgInterneurons cpg file wrong" << endl;
        }
        if(foot->getName().find("LEFT") != string::npos){
            yi.push_back(0.0);
            yi.push_back(offset+amplitude*teacher[0]);
        }
        else{
            yi.push_back(0.5);
            yi.push_back(offset+amplitude*teacher[0]);
        }
        myfile.close();
    }
    else{
        cout<<"ERROR: unable to open file '" << file << "'. In:Interneuron.hh, constructor(string, string)"<< endl;
		cout<<"\t" << path << endl;
        exit (EXIT_FAILURE);
    }
}
double CPGInterneuron::getParameter_level2(){
	return *parameter_level2;
}
double CPGInterneuron::getParameter_level3(){
	return *parameter_level3;
}
void CPGInterneuron::initialize(){
	initialize(offset, amplitude);
}
void CPGInterneuron::initialize(double offset, double amplitude){
    yi[0] = 0.0;
    yi[1] = offset+amplitude*teacher[0];
    t=0.0;
	f = foot->duration_last_step;
}
bool CPGInterneuron::isCst(){
	return getParameter_level3()==1.0;
}
void CPGInterneuron::update_state(){
	freq = centralclock->getFrequency();//1./duration;
}
void CPGInterneuron::apply(double o, double a){
	apply();

    static int supraspinal_separatedCtrl = Settings::get<int>("supraspinal_separatedCtrl");
    if(supraspinal_separatedCtrl == 0){
        // Parameter adaptation
        if(isCst()){
            o=parameters[1]["offset_change_cst"];
            a=parameters[1]["amp_change_cst"];
        }
        else{
            o=parameters[1]["offset_change_cpg"];
            a=parameters[1]["amp_change_cpg"];
        }
    }
	state = o+a*state;
}
void CPGInterneuron::apply(){
    static int cpg_strong_sync = Settings::get<int>("cpg_strong_sync");
    static int cpg_biPhasic = Settings::get<int>("cpg_biPhasic");
    static vector<double> param(5,0.0);
	if(isCst()){
		state = mean;
		return;
	}
	
	
	//update_state();
    if(cpg_biPhasic){
        if(active_during == ACTIVE_DURING::SWING){
            if(foot->justGetOffGround()){
                if(cpg_strong_sync==1) initialize();
            }
        }
        else{
            if(foot->justTouchTheGround()){
                if(cpg_strong_sync==1) initialize();
            }
        }
    }
    else if(foot->justTouchTheGround()){
		if(cpg_strong_sync==1) initialize();
	}

	param[0] = freq;
	param[1] = tau;
	param[2] = dt;
	param[3] = offset;
	param[4] = amplitude;
	yi = rungeKutta4(t,dt,yi,param,teacher,(Derivatives *)this);
	//yi = rungeKutta4(t,dt,yi,param,teacher,(*dydtF));
	t+=dt;
	state = yi[1];
}

void SensorySensoryInterneuron::apply(){
    static int jwang = Settings::get<int>("jWang_changes");
    if(jwang == 1)
        state = factor*weight*sensor2;
    else
        state = factor*weight*sensor1*sensor2;
}
void SensorySensoryInterneuron::apply(double o, double a){
	apply();
	state = o+a*state;
}
double SensorySensoryInterneuron::tf(double x){
    return Interneuron::tf(x);
}
void SensoryInterneuron::apply(){
	state = factor*weight*sensor;
}
void SensoryInterneuron::apply(double o, double a){
	apply();
    static int supraspinal_separatedCtrl = Settings::get<int>("supraspinal_separatedCtrl");
    if(supraspinal_separatedCtrl == 0){
        o=parameters[1]["offset_change_reflex"];
        a=parameters[1]["amp_change_reflex"];
    }
	state = o+a*state;
}
