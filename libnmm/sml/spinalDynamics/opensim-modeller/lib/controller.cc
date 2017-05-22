#include <controller.hh>

TwitchingController::TwitchingController(){
	constructProperties();
}

TwitchingController::TwitchingController(double duration, double amplitude, double stimulation_time, double start){
	constructProperties();
	set_start(start);
	set_duration(duration);
	set_amplitude(amplitude);
	set_stimulation_time(stimulation_time);
}
double TwitchingController::get_duration()
{
	double i = getActuatorSet().getSize();
	return i*get_stimulation_time()+get_start();
}
double TwitchingController::get_A(const SimTK::State& s, double i) const
{
	double time = s.getTime();
	double start_time = i*get_stimulation_time()+get_start();
	double end_time = (i+1)*get_stimulation_time()+get_start();
	double control = 0.0;

	// Are we doing smth ?
	if ( start_time < time && time < end_time){				
		if ( time < start_time + get_duration()){
			control = get_amplitude();	
		}
	}
	return control;
}
double TwitchingController::get_sIa(const SimTK::State& s, const Muscle* musc) const
{ // speed
    double speed = musc->getLengtheningSpeed(s);
    // unnormalize muscle's maximum contraction velocity (fib_lengths/sec) 
    double max_speed = musc->getOptimalFiberLength()*musc->getMaxContractionVelocity();
    return 0.5*(fabs(speed)+speed)/max_speed;
}
double TwitchingController::get_sII(const SimTK::State& s, const Muscle* musc) const
{ // length
    double length = musc->getLength(s);
    // unnormalize muscle's maximum contraction velocity (fib_lengths/sec) 
    double max_length = musc->getOptimalFiberLength();
    return 0.5*(fabs(length)+length)/max_length;
}
void TwitchingController::computeControls(const SimTK::State& s, SimTK::Vector &controls) const
{
	// get time
	double time = s.getTime();

	// get the list of actuators assigned to the reflex controller
	const Set<Actuator>& actuators = getActuatorSet();

	if(parameters->get<bool>("save_results")){
		*file_a << time << "\t";
		*file_sIa << time << "\t";
		*file_sII << time << "\t";
	}

	for(int i=0; i<actuators.getSize(); ++i){

		// Calculate control value


		const Muscle *musc = dynamic_cast<const Muscle*>(&actuators[i]);

		double control = get_A(s,(double)i);




		SimTK::Vector actControls(1,control);
		// add reflex controls to whatever controls are already in place.
		musc->addInControls(actControls, controls);


		// Dump to file
		if(parameters->get<bool>("save_results")){
		*file_a << control << "\t";
		*file_sIa << get_sIa(s,musc) << "\t";
		*file_sII << get_sII(s,musc) << "\t";
		}
	}
	if(parameters->get<bool>("save_results")){
		*file_a << endl;
		*file_sIa << endl;
		*file_sII << endl;
	}
}

void TwitchingController::constructProperties()
{
	parameters = new Parameters();
	parameters->get<bool>("save_results") = false;
	constructProperty_start(8.0);
	constructProperty_duration(0.1);
	constructProperty_amplitude(0.3);
	constructProperty_stimulation_time(8.0);
}

void TwitchingController::extendConnectToModel(Model& model)
{
	Super::extendConnectToModel(model);

	// get the list of actuators assigned to the reflex controller
	Set<Actuator>& actuators = updActuators();

	int cnt=0;
 
	while(cnt < actuators.getSize()){
	Muscle *musc = dynamic_cast<Muscle*>(&actuators[cnt]);
	// control muscles only
	if(!musc){
	cout << "TwitchingController:: WARNING- controller assigned a non-muscle actuator ";
	cout << actuators[cnt].getName() << " which will be ignored." << endl;
	actuators.remove(cnt);
	}else
	cnt++;
	}
}

void TwitchingController::init_file(){
	parameters->get<bool>("save_results") = true;

	file_a = new ofstream();
	file_sII = new ofstream();
	file_sIa = new ofstream();
	
	file_a->open("/tmp/output_a.txt");
	file_sII->open("/tmp/output_sII.txt");
	file_sIa->open("/tmp/output_sIa.txt");


	// get the list of actuators assigned to the reflex controller
	const Set<Actuator>& actuators = getActuatorSet();

	*file_a << "time" << "\t";
	*file_sII << "time" << "\t";
	*file_sIa << "time" << "\t";
	for(int i=0; i<actuators.getSize(); ++i){

		// Calculate control value


		const Muscle *musc = dynamic_cast<const Muscle*>(&actuators[i]);
		*file_a << musc->getName() << "\t";
		*file_sII << musc->getName() << "\t";
		*file_sIa << musc->getName() << "\t";
	}
	*file_a << endl;
	*file_sII << endl;
	*file_sIa << endl;
}