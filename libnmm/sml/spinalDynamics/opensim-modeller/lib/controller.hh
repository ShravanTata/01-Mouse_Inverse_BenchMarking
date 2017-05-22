#include <OpenSim/OpenSim.h>
#include <OpenSim/Simulation/Control/Controller.h>
#include <setting-manager/parameters.hh>

#include <map>
#include <fstream>

using namespace OpenSim;
using namespace std;
using namespace SimTK;



//using namespace OpenSim::ActuatorPowerProbe;

/**
 * @brief Implements a simple twitching controller
 * @details Each muscles of the model is contracted and the sensory response is saved in a file.
 */
class TwitchingController : public Controller {
OpenSim_DECLARE_CONCRETE_OBJECT(TwitchingController, Controller);

public:
//=============================================================================
// PROPERTIES
//=============================================================================
	/** @name Property declarations 
	These are the serializable properties associated with a TwitchingController.*/
	/**@{**/  	
	OpenSim_DECLARE_PROPERTY(duration, double, 
		"Duration of the stimulation" );
	OpenSim_DECLARE_PROPERTY(start, double, 
		"Starting time of the stimulation" );
	OpenSim_DECLARE_PROPERTY(amplitude, double, 
		"Amplitude of the stimulation" );
	OpenSim_DECLARE_PROPERTY(stimulation_time, double, 
		"Duration between stimulation" );

	ofstream* file_a;
	ofstream* file_sII;
	ofstream* file_sIa;
	Parameters* parameters;

//=============================================================================
// METHODS
//=============================================================================
	//--------------------------------------------------------------------------
	// CONSTRUCTION AND DESTRUCTION
	//--------------------------------------------------------------------------
	/** Default constructor. */
	TwitchingController();


	TwitchingController(double duration, double amplitude, double stimulation_time, double start);

	double get_duration();

	double get_A(const SimTK::State& s, double i) const;

	double get_sIa(const SimTK::State& s, const Muscle* musc) const;

	double get_sII(const SimTK::State& s, const Muscle* musc) const;

	/** Compute the controls for actuators (muscles)
	 *  This method defines the behavior of the TwitchingController 
	 *
	 * @param s			system state 
	 * @param controls	writable model controls
	 */
	void computeControls(const SimTK::State& s, SimTK::Vector &controls) const override;


	void init_file();


private:
	// Connect properties to local pointers.  */
	void constructProperties();
	// ModelComponent interface to connect this component to its model
	void extendConnectToModel(Model& model);




	//=============================================================================
};	// END of class TwitchingController
