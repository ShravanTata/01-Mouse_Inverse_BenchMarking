#include "Sml.hh"
#include "Interneuron.hh"
#include "Motoneuron.hh"
#include "Muscle.hh"
#include "MuscleJoint.hh"
#include "Joint.hh"
#include "Sensor.hh"
#include <sml/sml-tools/Settings.hh>
#include <sml/sml-tools/EventManager.hh>
#include <sml/sml-tools/OnlineCtrl.hh>

#include <boost/xpressive/xpressive.hpp>

extern EventManager* eventManager;
extern OnlineCtrl* onlineCtrl;
extern SmlParameters parameters;
using namespace std;
using namespace boost::xpressive;
using namespace HUMAN;

typedef std::map<std::string, std::map<std::string, Muscle>>::iterator it_muscle;

double rnd(){
    return (double)rand()/RAND_MAX;
}


Sml::Sml(
    ) : 
        Constant(CONSTANT::NUMBER,0.0),
        Input(INPUT::NUMBER,0.0),
        Output(OUTPUT::NUMBER,0.0),
        time_step(Settings::get<int>("time_step"))
    {
    }
double Sml::getTime(){
    return eventManager->get<double>(STATES::TIME);
}
//--------------------------------------------------------------
int Sml::step(){
    //Timer timer;
    //timer.Start();
    InputUpdate();
    /// Muscles to mechanical system
    step_JOINT(dt);
    step_MTU_to_TORQUE(dt);
    step_TORQUE_to_JOINT();
    /// Sensors to Neural control
    step_SENSORS(); 
    /// Neural control to Muscles
    step_SPINE_TO_MTU(dt); 
    /// Perturbation
    step_PERTURBATION();

    eventManager->step();  
    print_debug("[ok] : eventManager (Sml.cc)");
    return (int)eventManager->isRunFinished();
}

//compute l_MTC
void Sml::step_MTU_to_TORQUE(double dt){
    static int muscle_step_number = Settings::get<int>("muscle_step_number");
    static int i;
    for(auto&  muscle : muscles){
        muscle->ApplyForce();
        for(i=0;i<muscle_step_number;i++)
            muscle->step(dt/muscle_step_number);
        
    }
    print_debug("[ok] : compute muscle (Sml.cc)");
}
//compute all the joint angles
void Sml::step_JOINT(double dt){
    for(auto&  joint : joints){
        joint->step(dt);
    }
    trunk->step(dt);
    print_debug("[ok] : compute angle (Sml.cc)");
}


void Sml::step_TORQUE_to_JOINT(){
    static int torque_soft_limit = Settings::get<int>("torque_soft_limit");
    for(auto&  joint : joints){
        if (torque_soft_limit) joint->ComputeTorqueSoftLimit();
        joint->ComputeTorque();
    }
    print_debug("[ok] : compute torque (Sml.cc)");
}

void Sml::step_SENSORS(){
    for(auto &sen: sensors){
        sen.second->step();
    }
    right_foot->updateState(getTime());
    left_foot->updateState(getTime());
    if(right_foot->getState() ==  CYCLE_PHASE::STANCE && right_foot->getLastState() ==  CYCLE_PHASE::SWING && left_foot->getState() ==  CYCLE_PHASE::STANCE)
        finishing_stance = SIDE::LEFT;
    if(left_foot->getState() ==  CYCLE_PHASE::STANCE && left_foot->getLastState() ==  CYCLE_PHASE::SWING && right_foot->getState() ==  CYCLE_PHASE::STANCE){
        finishing_stance = SIDE::RIGHT;
    }
    
    if(right_foot->getState() ==  CYCLE_PHASE::SWING && right_foot->getLastState() ==  CYCLE_PHASE::STANCE)
        trunk->theta_to = trunk->theta;
    
    if(left_foot->getState() ==  CYCLE_PHASE::SWING && left_foot->getLastState() ==  CYCLE_PHASE::STANCE)
        trunk->theta_to = trunk->theta;

    print_debug("[ok] : compute sensors (Sml.cc)");
}

void Sml::step_PERTURBATION(){
    int perturbation = Settings::get<int>("perturbation");
    if (perturbation != 1) return;

    bool finishedPert;
    bool startPert=false;
    bool finishedPertPrinted=false;
    finishedPert = true;
    for (
        //INIT
        unsigned joint=JOINT::FIRST_JOINT,
                output=OUTPUT::FIRST_JOINT;
        output<=OUTPUT::LAST_JOINT;

        joint++,
        output++)
    {
         if(
            perturbation == 1 && 
            perturbator.find(JOINT::Joints(joint))
            )
         {
             if(perturbator[JOINT::Joints(joint)]->doPerturbation() )
             {
                perturbator[JOINT::Joints(joint)]->updatePerturbation();
                Output[output] += perturbator[JOINT::Joints(joint)]->getPerturbation();

                startPert = true;
                finishedPert = false;
             }
         }
    }

    if(finishedPert && !finishedPertPrinted && startPert){
        finishedPertPrinted = true;
        cout << "perturbation on one joint ended" << endl;
        //eventManager->set(STATES::STAY_IN_LOOP,false);
    }
}

void Sml::step_SPINE_TO_MTU(double dt){
}


void Sml::init(){
    cout << endl;
    cout << "=========================" << endl;
    cout << "=========================" << endl;
    cout << "Code of branch ver/serie5" << endl;
    cout << "=========================" << endl;
    cout << "=========================" << endl;


    eventManager = new EventManager(this);
    print_debug("[ok] : EventManager creation (Sml.cc)");
    SENSORY_INTERNEURON::init();
    print_debug("[ok] : SENSORY_INTERNEURON Init (Sml.cc)");
    ConstantInit();
    print_debug("[ok] : ConstantInit Init (Sml.cc)");
    InputInit();
    print_debug("[ok] : Input Init (Sml.cc)");
    /**
     * 
     * miscellaneous
     * 
     */
    //loadParam_level0(parameters[0]);
    //loadParam_level1(parameters[1]);

    
    
    //Constant initialisation
    initialiseConstant();
    initialisePerturbation();
    initialiseOnlineCtrl();
    /**
     * 
     * intialise model features
     * 
     */
    /** 
     *  1/Joints initialisation 
     */
    initialiseJoints();
    print_debug("[ok] : joints initialisation (Sml.cc)");
    /** 
     *  2/Muscles initialisation, act on joints
     */
    initialiseMuscles();
    print_debug("[ok] : muscles initialisation (Sml.cc)");
    /** 
     *  3a/Sensors initialisation (ground, foot, muscles)
     */
    initialiseSensors();
    print_debug("[ok] : sensors initialisation (Sml.cc)");

    initialiseSpinalControl();
    print_debug("[ok] : initialisation of spinal cord controller (Sml.cc");




}

void Sml::initialiseSpinalControl(){

}

void Sml::initialiseOnlineCtrl()
{
    onlineCtrl = new OnlineCtrl(this);
    onlineCtrl->Add("MN_MTU_delay",0.0,5.0,0.0);
    
}

void Sml::initialisePerturbation(){
    if(Settings::get<int>("perturbation") == 1) {
        perturbator = PerturbatorManager(Settings::get<double>("perturbation_start_at"),Settings::get<double>("perturbation_ends_after"));
        // perturbator.addInSerie(JOINT::HIP_LEFT);
        // perturbator.addInSerie(JOINT::KNEE_LEFT);
        // perturbator.addInSerie(JOINT::ANKLE_LEFT);
        perturbator.addInParallel(JOINT::ANKLE_LEFT);
    }
    //perturbator[JOINT::KNEE_LEFT] = new Perturbator(start_time+duration,duration);
    //perturbator[JOINT::ANKLE_LEFT] = new Perturbator(start_time+2*duration,duration);
    
    
    
}


void Sml::initialiseConstant()
{
    fixedViewPoint = false;
    flag = false;
    dt = double(time_step)/1000.0;
}
void Sml::initialiseJoints()
{
    trunk = new Trunk(this,INPUT::THETA_TRUNK, parameters[0]["kref"], parameters[1]["trunk_ref"]);
    for (
        //INIT
        unsigned joint=JOINT::FIRST_JOINT,
                input=INPUT::FIRST_JOINT,
                output=OUTPUT::FIRST_JOINT,
                joint_pos_ref=CONSTANT::FIRST_REF_POS_JOINT,
                joint_pos_min=CONSTANT::FIRST_MIN_POS_JOINT,
                joint_pos_max=CONSTANT::FIRST_MAX_POS_JOINT;
        //CONDITION
        //joint<=JOINT::LAST_JOINT,
        //input<=INPUT::LAST_JOINT,
        //joint_pos_ref<=CONSTANT::LAST_REF_POS_JOINT,
        //joint_pos_min<=CONSTANT::LAST_MIN_POS_JOINT,
        //joint_pos_max<=CONSTANT::LAST_MAX_POS_JOINT,
        output<=OUTPUT::LAST_JOINT;
        //INCREMENT
        joint++,
        input++,
        output++,
        joint_pos_ref++,
        joint_pos_min++,
        joint_pos_max++)
    {
        JointMomentArms::MOMENT_ARM joint_type;
      
        if(input == INPUT::ANGLE_HIP_RIGHT || INPUT::ANGLE_HIP_LEFT)
            {joint_type = JointMomentArms::CONSTANT;
            std::cout << "Check Constant" << std::endl;} 
        else
        {
            joint_type = JointMomentArms::GEYER;
            std::cout << "Check Geyer" << std::endl; 
        }

        joints[joint] = new Joint(
            joint_type,
            &Input[input],
            &Output[output],
            Constant[joint_pos_ref],
            Constant[joint_pos_min], 
            Constant[joint_pos_max]);
        joints[joint]->name = JOINT::toString((JOINT::Joints)joint);
    }
}



void Sml::initialiseMuscles(){
    //initialise muscle
    
    //optimization::Webots &opti = optimization::Webots::Instance();
    //l_slack[m], l_opt[m], v_max[l_opt/s], mass[kg], pennation[rad] , r0[m], joint, 
    
    
    double clockwise = -1.0;
    double cclockwise = 1.0;
    
    
    double length_ratio = Settings::get<double>("rescaling_mass_ratio");
    double force_ratio = Settings::get<double>("rescaling_length_ratio");
    double velocity_ratio = Settings::get<double>("rescaling_velocity_ratio");
    
    double r_SOL       = 0.05* length_ratio;
    double r_TA        = 0.04* length_ratio;
    double r_GAS_ankle = 0.05* length_ratio;
    double r_GAS_knee  = 0.05* length_ratio;
    double r_VAS       = 0.06* length_ratio;
    double r_HAM_knee  = 0.05* length_ratio;
    double r_HAM_hip   = 0.08* length_ratio;
    double r_GLU       = 0.10* length_ratio;
    double r_HF        = 0.10* length_ratio;
    double r_HAB       = 0.06* length_ratio;
    double r_HAD       = 0.03* length_ratio;
    
    /*
    double phi_max_SOL       = 110.0 * (2.0*3.14/360.0);
    double phi_max_TA        = 80.0  * (2.0*3.14/360.0);
    double phi_max_GAS_ankle = 110.0 * (2.0*3.14/360.0);
    double phi_max_GAS_knee  = 140.0 * (2.0*3.14/360.0);
    double phi_max_VAS       = 165.0 * (2.0*3.14/360.0);
    double phi_max_HAM_knee  = 180.0 * (2.0*3.14/360.0);
    
    double phi_ref_SOL       = 80.0  * (2.0*3.14/360.0);
    double phi_ref_TA        = 110.0 * (2.0*3.14/360.0);
    double phi_ref_GAS_ankle = 80.0  * (2.0*3.14/360.0);
    double phi_ref_GAS_knee  = 165.0 * (2.0*3.14/360.0);
    double phi_ref_VAS       = 125.0 * (2.0*3.14/360.0);
    double phi_ref_HAM_knee  = 180.0 * (2.0*3.14/360.0);
    double phi_ref_HAM_hip   = 155.0 * (2.0*3.14/360.0);
    double phi_ref_GLU       = 150.0 * (2.0*3.14/360.0);
    double phi_ref_HF        = 180.0 * (2.0*3.14/360.0);
    double phi_ref_HAB        = 10.0 * (2.0*3.14/360.0);
    double phi_ref_HAD        = 15.0 * (2.0*3.14/360.0);
    */
    double phi_max_SOL = Constant[CONSTANT::MAX_PHI_SOL];
    double phi_max_TA = Constant[CONSTANT::MAX_PHI_TA];
    double phi_max_GAS_ankle = Constant[CONSTANT::MAX_PHI_GAS_ANKLE];
    double phi_max_GAS_knee = Constant[CONSTANT::MAX_PHI_GAS_KNEE];
    double phi_max_VAS = Constant[CONSTANT::MAX_PHI_VAS];
    double phi_max_HAM_knee = Constant[CONSTANT::MAX_PHI_HAM_KNEE];
    
    double phi_ref_SOL = Constant[CONSTANT::REF_PHI_SOL];
    double phi_ref_TA = Constant[CONSTANT::REF_PHI_TA];
    double phi_ref_GAS_ankle = Constant[CONSTANT::REF_PHI_GAS_ANKLE];
    double phi_ref_GAS_knee = Constant[CONSTANT::REF_PHI_GAS_KNEE];
    double phi_ref_VAS = Constant[CONSTANT::REF_PHI_VAS];
    double phi_ref_HAM_knee = Constant[CONSTANT::REF_PHI_HAM_KNEE];
    double phi_ref_HAM_hip = Constant[CONSTANT::REF_PHI_HAM_HIP];
    double phi_ref_GLU = Constant[CONSTANT::REF_PHI_GLU];
    double phi_ref_HF = Constant[CONSTANT::REF_PHI_HF];
    double phi_ref_HAB = Constant[CONSTANT::REF_PHI_HAB];
    double phi_ref_HAD = Constant[CONSTANT::REF_PHI_HAD];
    

    
    Muscle  * m;
    
    /**
     * 
     * clockwise muscle will have a flag -1 
     * 
     */
    
    string name;
    parametersSet& p0 = parameters[0];
    double* muscle_parameters[7][9] = {
    //  hf      glu     vas     ham     gas     sol     ta      hab     had
        {&p0["hf_fmax"],&p0["glu_fmax"],&p0["vas_fmax"],&p0["ham_fmax"],&p0["gas_fmax"],&p0["sol_fmax"],&p0["ta_fmax"],&p0["hab_fmax"],&p0["had_fmax"]},  // fmax
        {&p0["hf_l_opt"],&p0["glu_l_opt"],&p0["vas_l_opt"],&p0["ham_l_opt"],&p0["gas_l_opt"],&p0["sol_l_opt"],&p0["ta_l_opt"],&p0["hab_l_opt"],&p0["had_l_opt"]},
        {&p0["hf_l_slack"],&p0["glu_l_slack"],&p0["vas_l_slack"],&p0["ham_l_slack"],&p0["gas_l_slack"],&p0["sol_l_slack"],&p0["ta_l_slack"],&p0["hab_l_slack"],&p0["had_l_slack"]},
        {&p0["hf_v_max"],&p0["glu_v_max"],&p0["vas_v_max"],&p0["ham_v_max"],&p0["gas_v_max"],&p0["sol_v_max"],&p0["ta_v_max"],&p0["hab_v_max"],&p0["had_v_max"]},
        {&p0["hf_mass"],&p0["glu_mass"],&p0["vas_mass"],&p0["ham_mass"],&p0["gas_mass"],&p0["sol_mass"],&p0["ta_mass"],&p0["hab_mass"],&p0["had_mass"]},// masses
        {&p0["hf_pennation"],&p0["glu_pennation"],&p0["vas_pennation"],&p0["ham_pennation"],&p0["gas_pennation"],&p0["sol_pennation"],&p0["ta_pennation"],&p0["hab_pennation"],&p0["had_pennation"]},// pennation
        {&p0["hf_typeIfiber"],&p0["glu_typeIfiber"],&p0["vas_typeIfiber"],&p0["ham_typeIfiber"],&p0["gas_typeIfiber"],&p0["sol_typeIfiber"],&p0["ta_typeIfiber"],&p0["hab_typeIfiber"],&p0["had_typeIfiber"]},// typeIfiber
    };
    double* muscle_parameters_left[7][9] = {
    //  hf      glu     vas     ham     gas     sol     ta      hab     had
        {&p0["hf_fmax_left"],&p0["glu_fmax_left"],&p0["vas_fmax_left"],&p0["ham_fmax_left"],&p0["gas_fmax_left"],&p0["sol_fmax_left"],&p0["ta_fmax_left"],&p0["hab_fmax_left"],&p0["had_fmax_left"]},  // fmax
        {&p0["hf_l_opt_left"],&p0["glu_l_opt_left"],&p0["vas_l_opt_left"],&p0["ham_l_opt_left"],&p0["gas_l_opt_left"],&p0["sol_l_opt_left"],&p0["ta_l_opt_left"],&p0["hab_l_opt_left"],&p0["had_l_opt_left"]},
        {&p0["hf_l_slack_left"],&p0["glu_l_slack_left"],&p0["vas_l_slack_left"],&p0["ham_l_slack_left"],&p0["gas_l_slack_left"],&p0["sol_l_slack_left"],&p0["ta_l_slack_left"],&p0["hab_l_slack_left"],&p0["had_l_slack_left"]},
        {&p0["hf_v_max_left"],&p0["glu_v_max_left"],&p0["vas_v_max_left"],&p0["ham_v_max_left"],&p0["gas_v_max_left"],&p0["sol_v_max_left"],&p0["ta_v_max_left"],&p0["hab_v_max_left"],&p0["had_v_max_left"]},
        {&p0["hf_mass_left"],&p0["glu_mass_left"],&p0["vas_mass_left"],&p0["ham_mass_left"],&p0["gas_mass_left"],&p0["sol_mass_left"],&p0["ta_mass_left"],&p0["hab_mass_left"],&p0["had_mass_left"]},// masses
        {&p0["hf_pennation_left"],&p0["glu_pennation_left"],&p0["vas_pennation_left"],&p0["ham_pennation_left"],&p0["gas_pennation_left"],&p0["sol_pennation_left"],&p0["ta_pennation_left"],&p0["hab_pennation_left"],&p0["had_pennation_left"]},// pennation
        {&p0["hf_typeIfiber_left"],&p0["glu_typeIfiber_left"],&p0["vas_typeIfiber_left"],&p0["ham_typeIfiber_left"],&p0["gas_typeIfiber_left"],&p0["sol_typeIfiber_left"],&p0["ta_typeIfiber_left"],&p0["hab_typeIfiber_left"],&p0["had_typeIfiber_left"]},// typeIfiber
    };
    double* muscle_parameters_right[7][9] = {
    //  hf      glu     vas     ham     gas     sol     ta      hab     had
        {&p0["hf_fmax_right"],&p0["glu_fmax_right"],&p0["vas_fmax_right"],&p0["ham_fmax_right"],&p0["gas_fmax_right"],&p0["sol_fmax_right"],&p0["ta_fmax_right"],&p0["hab_fmax_right"],&p0["had_fmax_right"]},  // fmax
        {&p0["hf_l_opt_right"],&p0["glu_l_opt_right"],&p0["vas_l_opt_right"],&p0["ham_l_opt_right"],&p0["gas_l_opt_right"],&p0["sol_l_opt_right"],&p0["ta_l_opt_right"],&p0["hab_l_opt_right"],&p0["had_l_opt_right"]},
        {&p0["hf_l_slack_right"],&p0["glu_l_slack_right"],&p0["vas_l_slack_right"],&p0["ham_l_slack_right"],&p0["gas_l_slack_right"],&p0["sol_l_slack_right"],&p0["ta_l_slack_right"],&p0["hab_l_slack_right"],&p0["had_l_slack_right"]},
        {&p0["hf_v_max_right"],&p0["glu_v_max_right"],&p0["vas_v_max_right"],&p0["ham_v_max_right"],&p0["gas_v_max_right"],&p0["sol_v_max_right"],&p0["ta_v_max_right"],&p0["hab_v_max_right"],&p0["had_v_max_right"]},
        {&p0["hf_mass_right"],&p0["glu_mass_right"],&p0["vas_mass_right"],&p0["ham_mass_right"],&p0["gas_mass_right"],&p0["sol_mass_right"],&p0["ta_mass_right"],&p0["hab_mass_right"],&p0["had_mass_right"]},// masses
        {&p0["hf_pennation_right"],&p0["glu_pennation_right"],&p0["vas_pennation_right"],&p0["ham_pennation_right"],&p0["gas_pennation_right"],&p0["sol_pennation_right"],&p0["ta_pennation_right"],&p0["hab_pennation_right"],&p0["had_pennation_right"]},// pennation
        {&p0["hf_typeIfiber_right"],&p0["glu_typeIfiber_right"],&p0["vas_typeIfiber_right"],&p0["ham_typeIfiber_right"],&p0["gas_typeIfiber_right"],&p0["sol_typeIfiber_right"],&p0["ta_typeIfiber_right"],&p0["hab_typeIfiber_right"],&p0["had_typeIfiber_right"]},// typeIfiber
    };
    static double muscle_parameters_default[7][9] = {
    //  hf      glu     vas     ham     gas     sol     ta      hab     had
        {2000,  1500,   6000,   3000,   1500,   4000,   800,    3000,   4500},  // fmax
        {0.11,  0.11,   0.08,   0.10,   0.05,   0.04,   0.06,   0.09,   0.1},   // l_opts
        {0.10,  0.13,   0.23,   0.31,   0.40,   0.26,   0.24,   0.07,   0.18},  // l_slacks
        {12.0,  12.0,   12.0,   12.0,   12.0,   6.0,    12.0,   12.0,   12.0},  // v_maxes
        {1.87,  1.4,    2.91,   1.82,   0.45,   1.36,   0.29,   1.145,  1.91},  // masses
        {0.5,   0.5,    0.7,    0.7,    0.7,    0.5,    0.7,    1.0,    1.0},   // pennation
        {0.5,   0.5,    0.5,    0.44,   0.54,   0.81,   0.7,    0.5,    0.5},   // typeIfiber
    };
    for (int i = 0; i<7; i++){
        for (int j = 0; j<9; j++){
                if (*muscle_parameters[i][j] == 0.0) // load default parameters if needed
                    *muscle_parameters[i][j] = muscle_parameters_default[i][j];
                
                if (Settings::get<int>("assymetric_gait") == 0){ // If symmetric gait then point to parameters without _side subscript
                    muscle_parameters_left[i][j] = muscle_parameters[i][j];
                    muscle_parameters_right[i][j] = muscle_parameters[i][j];
                }
                else{ // If assymetric gait then point to parameters for each side
                    if (*muscle_parameters_left[i][j] == 0.0 )
                        *muscle_parameters_left[i][j] = *muscle_parameters[i][j];
                
                    if (*muscle_parameters_right[i][j] == 0.0 )
                        *muscle_parameters_right[i][j] = *muscle_parameters[i][j];
                }
            
        }
    }

    
    double* fmax[9];
    double* l_opts[9];
    double* l_slacks[9];
    double* v_maxes[9];
    double* masses[9];
    
    double* pennation[9];
    double* typeIfiber[9];
    
    for(int i=0;i<9;i++){

        fmax[i] = muscle_parameters_left[0][i];
        l_opts[i] = muscle_parameters_left[1][i];
        l_slacks[i] = muscle_parameters_left[2][i];
        v_maxes[i] = muscle_parameters_left[3][i];
        masses[i] = muscle_parameters_left[4][i];
        pennation[i] = muscle_parameters_left[5][i];
        typeIfiber[i] = muscle_parameters_left[6][i];
        
        if(Settings::get<int>("coman_mass_ratio"))
            *fmax[i] *= force_ratio;
        
        if(Settings::get<int>("coman_length_ratio"))
            *l_opts[i] *= length_ratio;
        
        if(Settings::get<int>("coman_length_ratio"))
            *l_slacks[i] *= length_ratio;
        
        if(Settings::get<int>("coman_velocity_ratio"))
            *v_maxes[i] *= velocity_ratio;
        
        if(Settings::get<int>("coman_mass_ratio"))
            *masses[i] *= force_ratio;
    }
    name = "hf";
    m = new Muscle(SIDE::LEFT,name,*(l_slacks[0]),*(l_opts[0]),*(v_maxes[0]),*(fmax[0]), *(pennation[0]),*(typeIfiber[0])); //hf
    m->musclejoints.push_back( new MuscleJoint(joints[JOINT::HIP_LEFT], m ,r_HF,phi_ref_HF));
    muscles[MUSCLES::LEFT_HF] = m;
    
    name = "glu";    
    m = new Muscle(SIDE::LEFT,name,*(l_slacks[1]),*(l_opts[1]),*(v_maxes[1]),*(fmax[1]), *(pennation[1]),*(typeIfiber[1])); //glu
    m->musclejoints.push_back( new MuscleJoint(joints[JOINT::HIP_LEFT], m ,-r_GLU,phi_ref_GLU));
    muscles[MUSCLES::LEFT_GLU] = m;
    
    name = "vas";
    m = new Muscle(SIDE::LEFT,name,*(l_slacks[2]),*(l_opts[2]),*(v_maxes[2]),*(fmax[2]), *(pennation[2]),*(typeIfiber[2])); //vas
    m->musclejoints.push_back( new MuscleJoint(joints[JOINT::KNEE_LEFT], m ,r_VAS,phi_ref_VAS));
    muscles[MUSCLES::LEFT_VAS] = m;
    
    name = "ham";
    m = new Muscle(SIDE::LEFT,name,*(l_slacks[3]),*(l_opts[3]),*(v_maxes[3]),*(fmax[3]), *(pennation[3]),*(typeIfiber[3])); //ham
    m->musclejoints.push_back( new MuscleJoint(joints[JOINT::HIP_LEFT],m,-r_HAM_hip, phi_ref_HAM_hip) );
    m->musclejoints.push_back( new MuscleJoint(joints[JOINT::KNEE_LEFT],m,-r_HAM_knee, phi_ref_HAM_knee, phi_max_HAM_knee) );
    muscles[MUSCLES::LEFT_HAM] = m;
    
    name = "gas";
    m = new Muscle(SIDE::LEFT,name,*(l_slacks[4]),*(l_opts[4]),*(v_maxes[4]),*(fmax[4]), *(pennation[4]),*(typeIfiber[4])); //gas
    m->musclejoints.push_back( new MuscleJoint(joints[JOINT::ANKLE_LEFT],m,-r_GAS_ankle, phi_ref_GAS_ankle, phi_max_GAS_ankle));
    m->musclejoints.push_back( new MuscleJoint(joints[JOINT::KNEE_LEFT],m,-r_GAS_knee, phi_ref_GAS_knee, phi_max_GAS_knee) );
    muscles[MUSCLES::LEFT_GAS] = m;
    
    name = "sol";
    m = new Muscle(SIDE::LEFT,name,*(l_slacks[5]),*(l_opts[5]),*(v_maxes[5]),*(fmax[5]), *(pennation[5]),*(typeIfiber[5])); //sol
    m->musclejoints.push_back( new MuscleJoint(joints[JOINT::ANKLE_LEFT], m ,-r_SOL,phi_ref_SOL));
    muscles[MUSCLES::LEFT_SOL] = m;
    
    name = "ta";
    m = new Muscle(SIDE::LEFT,name,*(l_slacks[6]),*(l_opts[6]),*(v_maxes[6]),*(fmax[6]), *(pennation[6]),*(typeIfiber[6])); //ta
    m->musclejoints.push_back( new MuscleJoint(joints[JOINT::ANKLE_LEFT], m ,r_TA,phi_ref_TA));
    muscles[MUSCLES::LEFT_TA] = m;
    
    for(int i=0;i<9;i++){
        fmax[i] = muscle_parameters_right[0][i];
        l_opts[i] = muscle_parameters_right[1][i];
        l_slacks[i] = muscle_parameters_right[2][i];
        v_maxes[i] = muscle_parameters_right[3][i];
        masses[i] = muscle_parameters_right[4][i];
        pennation[i] = muscle_parameters_right[5][i];
        typeIfiber[i] = muscle_parameters_right[6][i];
        
        if(Settings::get<int>("coman_mass_ratio"))
            *fmax[i] *= force_ratio;
        
        if(Settings::get<int>("coman_length_ratio"))
            *l_opts[i] *= length_ratio;
        
        if(Settings::get<int>("coman_length_ratio"))
            *l_slacks[i] *= length_ratio;
        
        if(Settings::get<int>("coman_velocity_ratio"))
            *v_maxes[i] *= velocity_ratio;
        
        if(Settings::get<int>("coman_mass_ratio"))
            *masses[i] *= force_ratio;
    }
    name = "hf";
    m = new Muscle(SIDE::RIGHT,name,*(l_slacks[0]),*(l_opts[0]),*(v_maxes[0]),*(fmax[0]), *(pennation[0]),*(typeIfiber[0])); //hf
    m->musclejoints.push_back( new MuscleJoint(joints[JOINT::HIP_RIGHT],m,r_HF,phi_ref_HF));
    muscles[MUSCLES::RIGHT_HF] = m;
    
    name = "glu";    
    m = new Muscle(SIDE::RIGHT,name,*(l_slacks[1]),*(l_opts[1]),*(v_maxes[1]),*(fmax[1]), *(pennation[1]),*(typeIfiber[1])); //glu
    m->musclejoints.push_back( new MuscleJoint(joints[JOINT::HIP_RIGHT],m,-r_GLU,phi_ref_GLU));
    muscles[MUSCLES::RIGHT_GLU] = m;
    
    name = "vas";
    m = new Muscle(SIDE::RIGHT,name,*(l_slacks[2]),*(l_opts[2]),*(v_maxes[2]),*(fmax[2]), *(pennation[2]),*(typeIfiber[2])); //vas
    m->musclejoints.push_back( new MuscleJoint(joints[JOINT::KNEE_RIGHT],m,r_VAS,phi_ref_VAS));
    muscles[MUSCLES::RIGHT_VAS] = m;
    
    name = "ham";
    m = new Muscle(SIDE::RIGHT,name,*(l_slacks[3]),*(l_opts[3]),*(v_maxes[3]),*(fmax[3]), *(pennation[3]),*(typeIfiber[3])); //ham
    m->musclejoints.push_back( new MuscleJoint(joints[JOINT::HIP_RIGHT],m,-r_HAM_hip, phi_ref_HAM_hip) );
    m->musclejoints.push_back( new MuscleJoint(joints[JOINT::KNEE_RIGHT],m,-r_HAM_knee, phi_ref_HAM_knee, phi_max_HAM_knee) );
    muscles[MUSCLES::RIGHT_HAM] = m;
    
    name = "gas";
    m = new Muscle(SIDE::RIGHT,name,*(l_slacks[4]),*(l_opts[4]),*(v_maxes[4]),*(fmax[4]), *(pennation[4]),*(typeIfiber[4])); //gas
    m->musclejoints.push_back( new MuscleJoint(joints[JOINT::ANKLE_RIGHT],m,-r_GAS_ankle, phi_ref_GAS_ankle, phi_max_GAS_ankle));
    m->musclejoints.push_back( new MuscleJoint(joints[JOINT::KNEE_RIGHT],m,-r_GAS_knee, phi_ref_GAS_knee, phi_max_GAS_knee) );
    muscles[MUSCLES::RIGHT_GAS] = m;

    
    name = "sol";
    m = new Muscle(SIDE::RIGHT,name,*(l_slacks[5]),*(l_opts[5]),*(v_maxes[5]),*(fmax[5]), *(pennation[5]),*(typeIfiber[5])); //sol
    m->musclejoints.push_back( new MuscleJoint(joints[JOINT::ANKLE_RIGHT],m,-r_SOL,phi_ref_SOL));
    muscles[MUSCLES::RIGHT_SOL] = m;
    
    name = "ta";
    m = new Muscle(SIDE::RIGHT,name,*(l_slacks[6]),*(l_opts[6]),*(v_maxes[6]),*(fmax[6]), *(pennation[6]),*(typeIfiber[6])); //ta
    m->musclejoints.push_back( new MuscleJoint(joints[JOINT::ANKLE_RIGHT],m,r_TA,phi_ref_TA));
    muscles[MUSCLES::RIGHT_TA] = m;
    
}

void Sml::initialiseSensors(){
    /**
     * Ground sensors (left_foot , right_foot sensors)
     */
    hdisttocm[SIDE::LEFT] = 0.0;
    hdisttocm[SIDE::RIGHT] = 0.0;
    hdisttocm[SIDE::LEFT] = 0.0;
    hdisttocm[SIDE::RIGHT] = 0.0;
    TouchSensor * sensor_heel;
    TouchSensor * sensor_toe;
    sensor_heel = new TouchSensor(this,INPUT::SENSOR_HEEL_LEFT,time_step);
    sensor_toe = new TouchSensor(this,INPUT::SENSOR_TOE_LEFT,time_step);
    left_foot = new Foot(sensor_heel,sensor_toe,SIDE::LEFT,parameters[1]["offset_change_reflex"]);
    sensors["left_heel_sensor"] = sensor_heel; // we add them to the sensors table so that we ensure
    sensors["left_toe_sensor"] = sensor_toe;   // that the step function will be run.

    sensor_heel = new TouchSensor(this,INPUT::SENSOR_HEEL_RIGHT,time_step);
    sensor_toe = new TouchSensor(this,INPUT::SENSOR_TOE_RIGHT,time_step);
    right_foot = new Foot(sensor_heel,sensor_toe,SIDE::RIGHT,parameters[1]["offset_change_reflex"]);
    sensors["right_heel_sensor"] = sensor_heel;
    sensors["right_toe_sensor"] = sensor_toe;    


    sensors["left_foot"] = left_foot;
    sensors["right_foot"] = right_foot;
    
    /**
     * Muscle sensors
     */
    MuscleLengthSensor * mls;
    MuscleForceSensor * mfs;
    zero_length_offset = 0.0;
    one_gain = 1.0;
    for(auto& muscle: muscles){
        mfs = new MuscleForceSensor(muscle, one_gain);
        auto it = parameters[0].find(muscle->name+"_bl");
        if(it != parameters[0].end())
            mls = new MuscleLengthSensor(muscle, it->second, one_gain);
        else
            mls = new MuscleLengthSensor(muscle, zero_length_offset, one_gain);
        sensors[muscle->getName()+"_length"] = mls;
        sensors[muscle->getName()+"_force"] = mfs;
    }
    
    /**
     * Trunk sensors (stability sensors)
     */
    TrunkPositionSensor1 * tps1;
    tps1 = new TrunkPositionSensor1(trunk, parameters[0]["kp"], parameters[0]["kd"]);
    sensors["trunk_pd"] = tps1;
    
    TrunkPositionSensor2 * tps2;
    tps2 = new TrunkPositionSensor2(trunk, parameters[0]["kp"], parameters[0]["kd"]);
    sensors["trunk_pd_0.68"] = tps2;
    TrunkTakeOffSensorOld * tts;
    tts = new TrunkTakeOffSensorOld(trunk);
    sensors["trunk_leak"] = tts;
    
    /**
     * Angle sensors for the knee
     */
    phi_knee_off = 2.97;
    AngleSensor * as;
    
    if(Settings::get<int>("kphiknee_off") == 1){
        as = new AngleSensor(joints[JOINT::KNEE_RIGHT], parameters[0]["kphiknee_off"]);
        sensors["right_knee_angle"] = as;
        as = new AngleSensor(joints[JOINT::KNEE_LEFT], parameters[0]["kphiknee_off"]);
        sensors["left_knee_angle"] = as;
    }
    else
    {
        as = new AngleSensor(joints[JOINT::KNEE_RIGHT], phi_knee_off);
        sensors["right_knee_angle"] = as;
        as = new AngleSensor(joints[JOINT::KNEE_LEFT], phi_knee_off);
        sensors["left_knee_angle"] = as;
    }

}


/**
 * 
 * 
 * DEPRECATED
 * 
 */
// double Sml::getHDistToCM(SIDE::Side side){
//     return hdisttocm[side];
// }
// void Sml::setHDistToCM(){
//     setHDistToCM(SIDE::LEFT);
//     setHDistToCM(SIDE::RIGHT);
// }
// void Sml::setHDistToCM(SIDE::Side side){
//     Joint * hip;
//     Joint * knee;
//     double hip_knee = 0.5;
//     double knee_ankle = 0.5;
//     double foot_length = 0.16;
//     if(side == SIDE::RIGHT){
//         hip = joints[JOINT::HIP_RIGHT];
//         knee = joints[JOINT::KNEE_RIGHT];
//     }
//     else{
//         hip = joints[JOINT::HIP_LEFT];
//         knee = joints[JOINT::KNEE_LEFT];
//     }
//     hdisttocm[side]  = 
//     (
//         hip_knee*sin(hip->getAngle())+
//         knee_ankle*(
//             cos(knee->getAngle())*sin(hip->getAngle()+trunk->theta)+
//             sin(knee->getAngle())*cos(hip->getAngle()+trunk->theta)
//         )
//     )/foot_length;
    
// }

// void Sml::setVDistToCM(){
//     setVDistToCM(SIDE::LEFT);
//     setVDistToCM(SIDE::RIGHT);
// }
// void Sml::setVDistToCM(SIDE::Side side){
//     Joint * hip;
//     Joint * knee;

//     bool global = true;
//     double foot_length = 0.16;
//     double vdisttocm=0.0;
//     if ( global ){
//         double ankle_pos;
//         if(side == SIDE::RIGHT){
//             ankle_pos = Input[INPUT::ANGLE_ANKLE_RIGHT];
//         }
//         else{
//             ankle_pos = Input[INPUT::ANGLE_ANKLE_LEFT];
//         }
        
//         vdisttocm = Input[INPUT::NECK_Y] - ankle_pos;
//     }
        
//     if ( ! global ) {
//         double hip_knee = 0.5;
//         double knee_ankle = 0.5;
        
//         if(side == SIDE::RIGHT){
//             hip = joints[JOINT::HIP_RIGHT];
//             knee = joints[JOINT::KNEE_RIGHT];
//         }
//         else{
//             hip = joints[JOINT::HIP_LEFT];
//             knee = joints[JOINT::KNEE_LEFT];
//         }
        
//         vdisttocm = 
//         (
//             hip_knee*cos(hip->getAngle())+
//             knee_ankle*(
//                 sin(knee->getAngle())*cos(hip->getAngle()+trunk->theta)+
//                 cos(knee->getAngle())*sin(hip->getAngle()+trunk->theta)
//             )
//         )/foot_length;
//     }
//     if(side == SIDE::LEFT)
//         vdisttocm_left = vdisttocm;
//     else
//         vdisttocm_right = vdisttocm;
// }

