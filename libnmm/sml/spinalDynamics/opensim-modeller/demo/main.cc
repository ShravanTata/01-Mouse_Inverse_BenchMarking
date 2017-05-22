#include <iostream>
#include <model.hh>

using namespace std;    

int main(int argc, char* argv[]) {

	ModelBase m;
	//Arm1Dof2 m;
	//Cube m;
	//Leg6Dof9 m;
	//Leg6Dof9_oneleg m;

	m.cfg("duration",20.0);
	m.cfg("visualize",true);
	m.cfg("visualize/geometry",false);
	m.cfg("visualize/background_type", Visualizer::BackgroundType::GroundAndSky);
	m.cfg("save_simulation_results",false);
	m.cfg("load_parameters",false);

	if (argc == 2){
		m.parameters.loadFromFile(std::string(argv[1]));
	}
	else{
		for(unsigned int i = 1; i<argc; i=i+2){
			if (std::string(argv[i]) == "save_model"){
				m.cfg("save_model",true);
				m.cfg("save_model/filename",std::string(argv[i+1]));
			}
			else if(std::string(argv[i]) == "load_model"){
				m.cfg("load_from_file",true);
				m.cfg("load_from_file/filename",std::string(argv[i+1]));
			}
			else if(std::string(argv[i]) == "save_simulation_results"){
				m.cfg("save_simulation_results",true);
				m.cfg("save_simulation_results/filename",std::string(argv[i+1]));
			}
			else if(std::string(argv[i]) == "load_parameters"){
				m.cfg("load_parameters/filename",std::string(argv[i+1]));
			}
			else{
				cout << "unknown option: " << std::string(argv[i]) << endl;
			}
		}
	}
	m.init();
	//m.simulate(duration);
	m.simulate();

};
