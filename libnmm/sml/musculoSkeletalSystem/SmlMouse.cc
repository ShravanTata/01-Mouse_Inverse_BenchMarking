#include "SmlMouse.hh"

#include "Muscle.hh"
#include "MuscleJoint.hh"
#include "Joint.hh"
#include "Sensor.hh"
#include <sml/sml-tools/Settings.hh>

#include <boost/xpressive/xpressive.hpp>

extern EventManager* eventManager;
extern OnlineCtrl* onlineCtrl;
extern SmlParameters parameters;
using namespace boost::xpressive;
using namespace MOUSE;

typedef std::map<std::string, std::map<std::string, Muscle>>::iterator it_muscle;


SmlMouse::SmlMouse(): 
Constant(CONSTANT::NUMBER,0.0),
Input(INPUT::NUMBER,0.0),
Output(OUTPUT::NUMBER,0.0),
time_step(Settings::get<int>("time_step")){
}

extern bool debug;
void SmlMouse::init(){
    debug = false;
    cout << endl;
    cout << "=========================" << endl;
    cout << "=========================" << endl;
    cout << "Code of branch ver/MOUSE MODELLING" << endl;
    cout << "=========================" << endl;
    cout << "=========================" << endl;

    SENSORY_INTERNEURON::init();
    print_debug("[ok] : SENSORY_INTERNEURON Init (Sml.cc)");

    ConstantInit();
    print_debug("[ok] : ConstantInit Init (Sml.cc)");

    InputInit();
    print_debug("[ok] : Input Init (Sml.cc)");

    // Constant Initilisation
    initialiseConstant();

    // 1. Initialise Joints
    initialiseJoints();
    print_debug("[ok] : joints initialisation (SmlMouse.cc)");

    // 2. Initialise Muscles
    initialiseMuscles();
    print_debug("[ok] : muscles initialisation (SmlMouse.cc)");

    // 3. Initialise Sensors
    initialiseSensors();
    print_debug("[ok] : sensors initialisation (SmlMouse.cc)");

    // 4. Initialise Data Read
    intiailiseDataRead();
    print_debug("[ok] : Data read initialisation (SmlMouse.cc)");

}

// Initialise constant
void SmlMouse::initialiseConstant()
{
    dt = double(time_step)/1000.0;
}

// 1. Initialise Joints
void SmlMouse::initialiseJoints()
{

    for (

        unsigned joint = JOINT::FIRST_JOINT,
        input = INPUT::FIRST_JOINT,
        output = OUTPUT::FIRST_JOINT,
        joint_pos_ref = CONSTANT::FIRST_REF_POS_JOINT,
        joint_pos_min = CONSTANT::FIRST_MIN_POS_JOINT,
        joint_pos_max = CONSTANT::FIRST_MAX_POS_JOINT;

        output <= OUTPUT::FIRST_JOINT+2;

        joint++,
        input++,
        output++,
        joint_pos_ref++,
        joint_pos_min++,
        joint_pos_max++)

    {

        JointMomentArms::MOMENT_ARM joint_type;

        if((input == INPUT::ANGLE_HIP_RIGHT) || (input == INPUT::ANGLE_HIP_LEFT))
            {//Change
            //joint_type = JointMomentArms::CONSTANT;
                joint_type = JointMomentArms::GEYER;
                std::cout << "Check Geyer : CHANGED" << std::endl;} 
                else
                {
                    joint_type = JointMomentArms::GEYER;
                    std::cout << "Check Geyer" << std::endl; 
                }

                std::cout << JOINT::toString((JOINT::Joints)joint) << " : Min : " << joint_pos_min << " Max : " << joint_pos_max << std::endl; 

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

// 2. Initialise Muscles
        void SmlMouse::initialiseMuscles()
        {
            double clockwise = -1.0;
            double cclockwise = 1.0;

            double length_ratio = Settings::get<double>("rescaling_mass_ratio");
            double force_ratio = Settings::get<double>("rescaling_length_ratio");
            double velocity_ratio = Settings::get<double>("rescaling_velocity_ratio");

    // PMA
            double r_PMA        = 0.002373;

            double phi_ref_PMA = Constant[CONSTANT::REF_PHI_PMA];

    //Change
            double phi_max_PMA = Constant[CONSTANT::MAX_PHI_PMA];

    // CF 

            double r_CF     = 0.0028708;

            double phi_ref_CF = Constant[CONSTANT::REF_PHI_CF];

    //Change
            double phi_max_CF = Constant[CONSTANT::MAX_PHI_CF];


    // SM - BiArticular

            double r_SM_hip = 0.003982;

            double r_SM_knee = 0.003934;

            double phi_ref_SM_hip = Constant[CONSTANT::REF_PHI_SM_HIP];

    //Change
            double phi_max_SM_hip = Constant[CONSTANT::MAX_PHI_SM_HIP];

            double phi_ref_SM_knee = Constant[CONSTANT::REF_PHI_SM_KNEE];

            double phi_max_SM_knee = Constant[CONSTANT::MAX_PHI_SM_KNEE];

    // POP

            double r_POP = 0.00087184;

            double phi_ref_POP = Constant[CONSTANT::REF_PHI_POP];

            double phi_max_POP = Constant[CONSTANT::MAX_PHI_POP];

    // RF

            double r_RF = 0.0015904;

            double phi_ref_RF = Constant[CONSTANT::REF_PHI_RF];

            double phi_max_RF = Constant[CONSTANT::MAX_PHI_RF];

    // TA

            double r_TA = 0.0007748;

            double phi_ref_TA = Constant[CONSTANT::REF_PHI_TA];

            double phi_max_TA = Constant[CONSTANT::MAX_PHI_TA];

    // SOL

            double r_SOL = 0.001840;

            double phi_ref_SOL = Constant[CONSTANT::REF_PHI_SOL];

            double phi_max_SOL = Constant[CONSTANT::MAX_PHI_SOL];   

    // LG

            double r_LG_knee = 0.001221;

            double r_LG_ankle = 0.001996;

            double phi_ref_LG_knee = Constant[CONSTANT::REF_PHI_LG_KNEE];

            double phi_ref_LG_ankle = Constant[CONSTANT::REF_PHI_LG_ANKLE];

            double phi_max_LG_knee = Constant[CONSTANT::MAX_PHI_LG_KNEE]; 

            double phi_max_LG_ankle = Constant[CONSTANT::MAX_PHI_LG_ANKLE]; 


    // Create a pointer for muscles
            Muscle  * m;

            string name;
            parametersSet& p0 = parameters[0];

            double* muscle_parameters[7][8] = {
    //   PMA                 CF                SM             POP               RF          TA          SOL         LG
                {&p0["pma_fmax"],&p0["cf_fmax"],&p0["sm_fmax"],&p0["pop_fmax"],&p0["rf_fmax"],&p0["ta_fmax"],&p0["sol_fmax"],&p0["lg_fmax"]},
                {&p0["pma_l_opt"],&p0["cf_l_opt"],&p0["sm_l_opt"],&p0["pop_l_opt"],&p0["rf_l_opt"],&p0["ta_l_opt"],&p0["sol_l_opt"],&p0["lg_l_opt"]},
                {&p0["pma_l_slack"],&p0["cf_l_slack"],&p0["sm_l_slack"],&p0["pop_l_slack"],&p0["rf_l_slack"],&p0["ta_l_slack"],&p0["sol_l_slack"],&p0["lg_l_slack"]},
                {&p0["pma_v_max"],&p0["cf_v_max"],&p0["sm_v_max"],&p0["pop_v_max"],&p0["rf_v_max"],&p0["ta_v_max"],&p0["sol_v_max"],&p0["lg_v_max"]},
                {&p0["pma_mass"],&p0["cf_mass"],&p0["sm_mass"],&p0["pop_mass"],&p0["rf_mass"],&p0["ta_mass"],&p0["sol_mass"],&p0["lg_mass"]},
                {&p0["pma_pennation"],&p0["cf_pennation"],&p0["sm_pennation"],&p0["pop_pennation"],&p0["rf_pennation"],&p0["ta_pennation"],&p0["sol_pennation"],&p0["lg_pennation"]},
                {&p0["pma_typeIfiber"],&p0["cf_typeIfiber"],&p0["sm_typeIfiber"],&p0["pop_typeIfiber"],&p0["rf_typeIfiber"],&p0["ta_typeIfiber"],&p0["sol_typeIfiber"],&p0["lg_typeIfiber"]},
            };//CHANGE
            static double muscle_parameters_default[7][8] = {
    //pma           cf          sm          pop         rf          ta          sol         lg
    { 1.338,        0.554,      1.916,      0.307,      4.162,      2.422,      0.591,      3.784   }, // fmax
    { 0.00697,      0.01137,    0.01165,    0.00206,    0.00534,    0.0049,     0.00316,    0.00541 }, // l_opts
    { 0.00501,      0.00307,    0.00409,    0.00203,    0.00853,    0.0118,     0.0074,     0.01389}, // l_slacks
    { 12,           12,         12,         12,         12,         12,         12,         12 }, // v_maxes
    { 0.00003285,   0.00002216, 0.00007861, 0.00000222, 0.00000782, 0.000004217, 0.000000658, 0.0000072 }, // masses
    { 0.7288,       1,          1,          1,          0.7226,     1,     1,     0.6984 }, // pennation
    { 0.5,          0.5,        0.5,        0.5,        0.5,        0.5,        0.5,        0.5 }, // typeIfiber
};


for (int i = 0; i<7; i++){
    for (int j = 0; j<8; j++){
                if (*muscle_parameters[i][j] == 0.0) // load default parameters 
                    *muscle_parameters[i][j] = muscle_parameters_default[i][j];            
            }
        }


        double* fmax[8];
        double* l_opts[8];
        double* l_slacks[8];
        double* v_maxes[8];
        double* masses[8];

        double* pennation[8];
        double* typeIfiber[8];  

        for(int i=0;i<8;i++){

            fmax[i] = muscle_parameters[0][i];
            l_opts[i] = muscle_parameters[1][i];
            l_slacks[i] = muscle_parameters[2][i];
            v_maxes[i] = muscle_parameters[3][i];
            masses[i] = muscle_parameters[4][i];
            pennation[i] = muscle_parameters[5][i];
            typeIfiber[i] = muscle_parameters[6][i];

        }


    name = "pma";
    m = new Muscle(SIDE::LEFT,name,*(l_slacks[0]),*(l_opts[0]),*(v_maxes[0]),*(fmax[0]), *(pennation[0]),*(typeIfiber[0])); //pma
    m->musclejoints.push_back( new MuscleJoint(joints[JOINT::HIP_LEFT], m ,-r_PMA,phi_ref_PMA,phi_max_PMA));
    muscles[MUSCLES_MOUSE::LEFT_PMA] = m;
    
    name = "cf";    
    m = new Muscle(SIDE::LEFT,name,*(l_slacks[1]),*(l_opts[1]),*(v_maxes[1]),*(fmax[1]), *(pennation[1]),*(typeIfiber[1])); //cf
    m->musclejoints.push_back( new MuscleJoint(joints[JOINT::HIP_LEFT], m ,r_CF,phi_ref_CF,phi_max_CF));
    muscles[MUSCLES_MOUSE::LEFT_CF] = m;

    name = "sm";
    m = new Muscle(SIDE::LEFT, name,*(l_slacks[2]),*(l_opts[2]),*(v_maxes[2]),*(fmax[2]), *(pennation[2]),*(typeIfiber[2])); // sm
    m->musclejoints.push_back(new MuscleJoint(joints[JOINT::HIP_LEFT], m ,r_SM_hip,phi_ref_SM_hip,phi_max_SM_hip));
    m->musclejoints.push_back( new MuscleJoint(joints[JOINT::KNEE_LEFT], m ,r_SM_knee,phi_ref_SM_knee,phi_max_SM_knee));
    muscles[MUSCLES_MOUSE::LEFT_SM] = m;

    name = "pop";
    m = new Muscle(SIDE::LEFT, name,*(l_slacks[3]),*(l_opts[3]),*(v_maxes[3]),*(fmax[3]), *(pennation[3]),*(typeIfiber[3])); // pop
    m->musclejoints.push_back(new MuscleJoint(joints[JOINT::KNEE_LEFT], m ,r_POP,phi_ref_POP,phi_max_POP));
    muscles[MUSCLES_MOUSE::LEFT_POP] = m;

    name = "rf";
    m = new Muscle(SIDE::LEFT, name,*(l_slacks[4]),*(l_opts[4]),*(v_maxes[4]),*(fmax[4]), *(pennation[4]),*(typeIfiber[4])); // rf
    m->musclejoints.push_back(new MuscleJoint(joints[JOINT::KNEE_LEFT], m ,-r_RF,phi_ref_RF,phi_max_RF));
    muscles[MUSCLES_MOUSE::LEFT_RF] = m;

    name = "ta";
    m = new Muscle(SIDE::LEFT, name,*(l_slacks[5]),*(l_opts[5]),*(v_maxes[5]),*(fmax[5]), *(pennation[5]),*(typeIfiber[5])); // ta
    m->musclejoints.push_back(new MuscleJoint(joints[JOINT::ANKLE_LEFT], m ,-r_TA,phi_ref_TA,phi_max_TA));
    muscles[MUSCLES_MOUSE::LEFT_TA] = m;

    name = "sol";
    m = new Muscle(SIDE::LEFT, name,*(l_slacks[6]),*(l_opts[6]),*(v_maxes[6]),*(fmax[6]), *(pennation[6]),*(typeIfiber[6])); // sol
    m->musclejoints.push_back(new MuscleJoint(joints[JOINT::ANKLE_LEFT], m ,r_SOL,phi_ref_SOL,phi_max_SOL));
    muscles[MUSCLES_MOUSE::LEFT_SOL] = m;

    name = "lg";
    m = new Muscle(SIDE::LEFT, name,*(l_slacks[7]),*(l_opts[7]),*(v_maxes[7]),*(fmax[7]), *(pennation[7]),*(typeIfiber[7])); // lg
    m->musclejoints.push_back(new MuscleJoint(joints[JOINT::KNEE_LEFT], m ,r_LG_knee,phi_ref_LG_knee,phi_max_LG_knee));
    m->musclejoints.push_back( new MuscleJoint(joints[JOINT::ANKLE_LEFT], m ,r_LG_ankle,phi_ref_LG_ankle,phi_max_LG_ankle));
    muscles[MUSCLES_MOUSE::LEFT_LG] = m;

}

// 3. Initialise Sensors
void SmlMouse::initialiseSensors()
{

    // Muscle Sensors
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

}

// 4. Initialise Data Read
void SmlMouse::intiailiseDataRead(){
    // Open file to log data
    dataRead.open("/home/tatarama/Stata-PhD/Data/Results/Results_Default.txt");
    dataRead << "Time \t";

    for(auto& joint : joints)
        dataRead << joint->getName() << "\t";

    for(auto& muscle:muscles)
        dataRead << muscle->getName() << "\t";

    for(int j = 0; j < 10;j++)
        dataRead << "Torque-" << j << "\t";

    dataRead << "\n";

    time = 0.0;
}

double SmlMouse::getTime(){
    return eventManager->get<double>(STATES::TIME);
}

//compute l_MTC
void SmlMouse::step_MTU_to_TORQUE(double dt){
    static int muscle_step_number = Settings::get<int>("muscle_step_number");
    static int i;
    for(auto&  muscle : muscles){
        muscle->ApplyForce();
        for(i=0;i<muscle_step_number;i++){
            // std::cout << "Muscle Name : " << i <<std::endl; 
            muscle->step(dt/muscle_step_number);
        }
    }
    print_debug("[ok] : compute muscle (Sml.cc)");
}

//compute all the joint angles
void SmlMouse::step_JOINT(double dt){
    for(auto&  joint : joints){
        joint->step(dt);
    }
    print_debug("[ok] : compute angle (Sml.cc)");
}

void SmlMouse::step_TORQUE_to_JOINT(){
    static int torque_soft_limit = Settings::get<int>("torque_soft_limit");
    for(auto&  joint : joints){
        if (torque_soft_limit) joint->ComputeTorqueSoftLimit();
        joint->ComputeTorque();
    }
    print_debug("[ok] : compute torque (Sml.cc)");
}

void SmlMouse::step_SENSORS(){
    for(auto &sen: sensors){
        sen.second->step();
    }
    print_debug("[ok] : compute sensors (Sml.cc)");
}

void SmlMouse::step_SPINE_TO_MTU(double dt){

}

void SmlMouse::initialiseSpinalControl(){

}

int SmlMouse::step(){
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

    //for (auto joint : joints)
    //std::cout <<  "Angle of " << joint->getName() << " : " << joint->getAngle()*180/3.14 << std::endl;

    //for (unsigned int j = 0;j<8;j++)
    //std::cout << muscles[5]->getName() << " L_MTC \t : " << muscles[5]->getL_MTC() << std::endl;

    dataRead << time << "\t";
    time += dt;

    // TO PRINT MUSCLE FORCE : temp
    //std::cout << muscles[7]->getName() << " "<< muscles[7]->getForce() << std::endl;

    muscles[MUSCLES_MOUSE::toID("LEFT_SOL")];

    for (auto& joint : joints) 
     {dataRead << joint->getAngle()*180/3.14 << "\t";
      //std::cout << joint->getName() << " : " << joint->getAngle()*180/3.14 << std::endl;
}

for (unsigned int j = 0;j<8;j++)
    {dataRead << muscles[j]->getL_CE() <<"\t";}

dataRead << muscles[0]->musclejoints[0]->getTorque() << "\t";
dataRead << muscles[1]->musclejoints[0]->getTorque() << "\t";
dataRead << muscles[2]->musclejoints[0]->getTorque() << "\t";
dataRead << muscles[2]->musclejoints[1]->getTorque() << "\t";
dataRead << muscles[3]->musclejoints[0]->getTorque() << "\t";
dataRead << muscles[4]->musclejoints[0]->getTorque() << "\t";
dataRead << muscles[5]->musclejoints[0]->getTorque() << "\t";
dataRead << muscles[6]->musclejoints[0]->getTorque() << "\t";
dataRead << muscles[7]->musclejoints[0]->getTorque() << "\t";
dataRead << muscles[7]->musclejoints[1]->getTorque() << "\t";

dataRead << muscles[0]->musclejoints[0]->getMomentArm() << "\t";
dataRead << muscles[1]->musclejoints[0]->getMomentArm() << "\t";
dataRead << muscles[2]->musclejoints[0]->getMomentArm() << "\t";
dataRead << muscles[2]->musclejoints[1]->getMomentArm() << "\t";
dataRead << muscles[3]->musclejoints[0]->getMomentArm() << "\t";
dataRead << muscles[4]->musclejoints[0]->getMomentArm() << "\t";
dataRead << muscles[5]->musclejoints[0]->getMomentArm() << "\t";
dataRead << muscles[6]->musclejoints[0]->getMomentArm() << "\t";
dataRead << muscles[7]->musclejoints[0]->getMomentArm() << "\t";
dataRead << muscles[7]->musclejoints[1]->getMomentArm() << "\t";

for (unsigned int j = 0;j<8;j++)
    dataRead << muscles[j]->getForce() <<"\t";

dataRead << "\n";


// }  
//     dataRead << Input[0] << "\n";

return 0;
}

SmlMouse::~SmlMouse(){

    dataRead.close();
}