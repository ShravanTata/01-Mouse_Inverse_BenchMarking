#include "ReflexController.hh"


#include <string>
#include <math.h>
#include <vector>
#include <fstream>
#include <map>
#include <array>

//#include <boost/algorithm/string.hpp>

#include <dynamics/link.hh>
#include <timer/clock.hh>

#include <sml/types/types.h>
#include <sml/musculoSkeletalSystem/Interneuron.hh>
#include <sml/musculoSkeletalSystem/Motoneuron.hh>
#include <sml/musculoSkeletalSystem/Entity.hh>
#include <sml/musculoSkeletalSystem/Connection.hh>
#include <sml/musculoSkeletalSystem/Joint.hh>
#include <sml/musculoSkeletalSystem/Muscle.hh>
#include <sml/musculoSkeletalSystem/Sensor.hh>

#include <sml/sml-tools/EventManager.hh>
#include <sml/sml-tools/OnlineCtrl.hh>
#include <sml/sml-tools/Settings.hh>

extern EventManager* eventManager;
extern bool debug;
extern SmlParameters parameters;
using namespace std;


ReflexController::ReflexController(): Controller(), time_step(Settings::get<int>("time_step")){

}

void ReflexController::update(){
    step_INsen_to_IN(); 
    step_INcpg_to_IN(); 
    step_IN_to_MN(); 
    step_MN_to_output();
}


CYCLE_PHASE::CyclePhase ReflexController::getLimbState(SIDE::Side side){
    CYCLE_PHASE::CyclePhase limb_state;
    if(side == SIDE::RIGHT){
        limb_state = (CYCLE_PHASE::CyclePhase(eventManager->get<int>(STATES::RIGHT_FOOT)));
    }
    else{
        limb_state = (CYCLE_PHASE::CyclePhase(eventManager->get<int>(STATES::LEFT_FOOT)));
    }
    return limb_state;
}
string ReflexController::getLimbState_str(SIDE::Side side){
    return CYCLE_PHASE::toString(getLimbState(side));
}
bool ReflexController::isGateOpen(Interneuron * interneuron){
    static bool add;
    add=false;
    /*** FEEDBACK ALWAYS ACTIVE */
    if(interneuron->activeDuring() == ACTIVE_DURING::WHOLE_CYCLE) add=true;
    /*** FEEDBACK ACTIVE DURING STANCE / SWING */
    //if(getLimbState_str(interneuron->getOutputEntity()->side).find(interneuron->activeDuring()) != string::npos) add=true;
    if(
        ACTIVE_DURING::isActive(
            interneuron->activeDuring(),
            getLimbState(interneuron->getOutputEntity()->side)
        )
    ){
        add=true;
    }
    /*** FEEDBACK ANTI OVER EXTENSION */
    if(
        interneuron->getOutputEntity()->side == SIDE::LEFT && 
        interneuron->activeDuring() == ACTIVE_DURING::ANGLE_OFFSET
        ) {
            add=true;
    }
    if(
        interneuron->getOutputEntity()->side == SIDE::RIGHT && 
        interneuron->activeDuring() == ACTIVE_DURING::ANGLE_OFFSET
        ) {
            add=true;
    }
    if(add){ return true; }
    else return false;
}
void ReflexController::applyGateNeuron(Interneuron * interneuron){
    //change : was 
    if(isGateOpen(interneuron)) interneuron->getOutputEntity()->add(interneuron->get()); 
    //interneuron->getOutputEntity()->add(interneuron->get());
}


void ReflexController::step_INcpg_to_IN(){
    static Interneuron * in;
    static double o;
    static double a;
    static int INcpg;
    for (INcpg=SENSORY_INTERNEURON::FIRST; INcpg<=SENSORY_INTERNEURON::LAST; INcpg++){
        in = (Interneuron *)INcpgs[INcpg];
        
        //double o=0.0;
        //double a=1.0;
        o=parameters[5][((Interneuron *) in)->file];
        a=1.0+parameters[4][((Interneuron *) in)->file];
        
        in->apply(o,a);
        in->step();
    }
#ifndef REAL_TIME
//        debug == true ? cout << "[ok] : compute cpg interneuron state (Sml.cc)" << endl:true;
#endif
}

void ReflexController::step_INsen_to_IN(){    
    static int INsen;
    static double o;
    static double a;
    for (INsen=SENSORY_INTERNEURON::FIRST; INsen<=SENSORY_INTERNEURON::LAST; INsen++){
        Interneuron * in = (Interneuron *)INsens[INsen];
        o=parameters[7][((Interneuron *) in)->file];
        a=1.0+parameters[6][((Interneuron *) in)->file];
        in->apply(o,a);
    }
    print_debug("[ok] : compute sensory interneuron state (Sml.cc)");
}


double ReflexController::get_IN_contribution(int IN,bool cpg){
    CPGInterneuron * in = (CPGInterneuron *)INcpgs[IN];
    static bool combine = (Settings::get<string>("config_cpg_type") == "combine" || Settings::get<string>("config_cpg_type") == "full");
    static double contribution;
    if (combine)
    {
        if(cpg)
            contribution = in->getParameter_level2();
        else
            contribution = 1-in->getParameter_level2();
            
    }
    else
    {
        if(cpg)
            contribution = 0.0;
        else
            contribution = 1.0;
    }
    return contribution;
}


void ReflexController::step_IN_to_MN(array2d<Connection*,SENSORY_INTERNEURON::NUMBER,MUSCLES::NUMBER> __IN_MN,bool cpg){
    static int jwang = Settings::get<int>("jWang_changes");
    static bool skip;
    static Connection * connection;
    skip = false;
    for (int INsen=SENSORY_INTERNEURON::FIRST; INsen<=SENSORY_INTERNEURON::LAST; INsen++){
        for(int MN=MUSCLES::FIRST; MN<=MUSCLES::LAST; MN++){
            connection = __IN_MN[INsen][MN];
            if(connection != 0){

                if(connection->isGateOpen(this)){
                    if(jwang == 1)
                    {
                        if(getInput<SIDE::Side>("finishing_stance") == SIDE::RIGHT)
                        {
                            if(INsen == SENSORY_INTERNEURON::RIGHT_HAM_TRUNK_PD_STANCE || INsen == SENSORY_INTERNEURON::RIGHT_HF_TRUNK_PD_STANCE || INsen == SENSORY_INTERNEURON::RIGHT_GLU_TRUNK_PD2_STANCE)
                                skip = true;
                        }
                        if(getInput<SIDE::Side>("finishing_stance") == SIDE::LEFT)
                        {
                            if(INsen == SENSORY_INTERNEURON::LEFT_HAM_TRUNK_PD_STANCE || INsen == SENSORY_INTERNEURON::LEFT_HF_TRUNK_PD_STANCE || INsen == SENSORY_INTERNEURON::LEFT_GLU_TRUNK_PD2_STANCE)
                                skip = true;
                        }
                        if(!skip)
                            connection->step(get_IN_contribution(INsen,cpg));
                        skip = false;
                    }
                    else{
                        connection->step(get_IN_contribution(INsen,cpg));
                    }
                }
            }
        }
    }    
}


void ReflexController::step_IN_to_MN(){
    static bool cpg=true;
    static int INsen;
    for(auto& motoneuron: motoneurons)
        motoneuron->input = 0;
    
    for (INsen=SENSORY_INTERNEURON::FIRST; INsen<=SENSORY_INTERNEURON::LAST; INsen++)
        INsens[INsen]->step();
    step_IN_to_MN(__INsen_MN,!cpg);

    for (INsen=SENSORY_INTERNEURON::FIRST; INsen<=SENSORY_INTERNEURON::LAST; INsen++)
        INcpgs[INsen]->step();
    step_IN_to_MN(__INcpg_MN,cpg);


    print_debug("[ok] : compute interneuron state (Sml.cc)");
}


void ReflexController::step_MN_to_output(){
    for(auto& motoneuron: motoneurons){
        motoneuron->apply();
        motoneuron->step();
        motoneuron->muscle = motoneuron->get();
    }
}




void ReflexController::initialiseFeedbacks(SIDE::Side side){
    string cside_str;
    string side_str;
    if(side == SIDE::LEFT){
        cside_str = "right";
        side_str = "left";
    }
    else{
        cside_str = "left";
        side_str = "right";
    }


    if(side == SIDE::RIGHT){
        INsens[SENSORY_INTERNEURON::RIGHT_SOL_Ib_STANCE] = new SensoryInterneuron("sol__mff_stance",ACTIVE_DURING::STANCE,getInput(side_str+"_sol_force"),motoneurons[MUSCLES::toID(side_str+"_sol")],parameters[0]["solsol_wf_right"]);
        INsens[SENSORY_INTERNEURON::RIGHT_TA_II_CYCLE] = new SensoryInterneuron("ta__mlf_stance",ACTIVE_DURING::WHOLE_CYCLE,getInput(side_str+"_ta_length"), motoneurons[MUSCLES::toID(side_str+"_ta")], parameters[0]["tata_wl_right"]);
        INsens[SENSORY_INTERNEURON::RIGHT_SOL_RIGHT_TA_II_STANCE] = new SensoryInterneuron("sol_ta_mff_stance",ACTIVE_DURING::STANCE,getInput(side_str+"_sol_force"), motoneurons[MUSCLES::toID(side_str+"_ta")], parameters[0]["solta_wf_right"],-1.0);
        INsens[SENSORY_INTERNEURON::RIGHT_GAS_Ib_STANCE] = new SensoryInterneuron("gas__mff_stance",ACTIVE_DURING::STANCE,getInput(side_str+"_gas_force"), motoneurons[MUSCLES::toID(side_str+"_gas")], parameters[0]["gasgas_wf_right"]);
        INsens[SENSORY_INTERNEURON::RIGHT_VAS_Ib_STANCE] = new SensoryInterneuron("vas__mff_stance",ACTIVE_DURING::STANCE,getInput(side_str+"_vas_force"), motoneurons[MUSCLES::toID(side_str+"_vas")], parameters[0]["vasvas_wf_right"]);
        if(Settings::get<int>("noAngleOffset")==0)
            INsens[SENSORY_INTERNEURON::RIGHT_VAS_ANGLE_ANGLE_OFFSET] = new SensoryInterneuron("vas_pko_angleoffset",ACTIVE_DURING::ANGLE_OFFSET,getInput(side_str+"_knee_angle"), motoneurons[MUSCLES::toID(side_str+"_vas")], parameters[0]["kphiknee_right"],-1.0);
        else
            INsens[SENSORY_INTERNEURON::RIGHT_VAS_ANGLE_ANGLE_OFFSET] = new SensoryInterneuron("vas_pko_angleoffset",ACTIVE_DURING::STANCE_END,getInput(cside_str+"_foot"), motoneurons[MUSCLES::toID(side_str+"_vas")], parameters[0]["kbodyweight_right"],-1.0);
        INsens[SENSORY_INTERNEURON::RIGHT_VAS_LEFT_FOOT_STANCE_END] = new SensoryInterneuron("vas_gcf_finishingstance",ACTIVE_DURING::STANCE_END,getInput(cside_str+"_foot"), motoneurons[MUSCLES::toID(side_str+"_vas")], parameters[0]["kbodyweight_right"],-1.0);
        INsens[SENSORY_INTERNEURON::RIGHT_HAM_TRUNK_PD_STANCE] = new SensorySensoryInterneuron("ham_gif_stance",ACTIVE_DURING::STANCE,getInput(side_str+"_foot"), getInput("trunk_pd"), motoneurons[MUSCLES::toID(side_str+"_ham")], parameters[0]["kbodyweight_right"]);
        INsens[SENSORY_INTERNEURON::RIGHT_HAM_Ib_SWING] = new SensoryInterneuron("ham__mff_swing",ACTIVE_DURING::SWING,getInput(side_str+"_ham_force"), motoneurons[MUSCLES::toID(side_str+"_ham")], parameters[0]["hamham_wf_right"]);
        INsens[SENSORY_INTERNEURON::RIGHT_GLU_TRUNK_PD2_STANCE] = new SensorySensoryInterneuron("glu_gif_stance",ACTIVE_DURING::STANCE,getInput(side_str+"_foot"), getInput("trunk_pd_0.68"), motoneurons[MUSCLES::toID(side_str+"_glu")], parameters[0]["kbodyweight_right"]);
        INsens[SENSORY_INTERNEURON::RIGHT_GLU_Ib_SWING] = new SensoryInterneuron("glu__mff_swing",ACTIVE_DURING::SWING,getInput(side_str+"_glu_force"), motoneurons[MUSCLES::toID(side_str+"_glu")], parameters[0]["gluglu_wf_right"]);
        INsens[SENSORY_INTERNEURON::RIGHT_HF_TRUNK_PD_STANCE] = new SensorySensoryInterneuron("hf_gif_stance",ACTIVE_DURING::STANCE,getInput(side_str+"_foot"), getInput("trunk_pd"), motoneurons[MUSCLES::toID(side_str+"_hf")], parameters[0]["kbodyweight_right"],-1.0);
        INsens[SENSORY_INTERNEURON::RIGHT_HAM_RIGHT_HF_II_SWING] = new SensoryInterneuron("ham_hf_mlf_swing",ACTIVE_DURING::SWING,getInput(side_str+"_ham_length"), motoneurons[MUSCLES::toID(side_str+"_hf")], parameters[0]["hamhf_wl_right"],-1.0);
        INsens[SENSORY_INTERNEURON::RIGHT_HF_II_SWING] = new SensoryInterneuron("hf__mlf_swing",ACTIVE_DURING::SWING,getInput(side_str+"_hf_length"), motoneurons[MUSCLES::toID(side_str+"_hf")], parameters[0]["hfhf_wl_right"]);
        INsens[SENSORY_INTERNEURON::RIGHT_HF_TRUNK_LEAK_SWING] = new SensoryInterneuron("hf_tl_swing",ACTIVE_DURING::SWING,getInput("trunk_leak"), motoneurons[MUSCLES::toID(side_str+"_hf")], parameters[0]["klean"]);
        if(Settings::get<int>("notf_trunklean") == 0)
            INsens[SENSORY_INTERNEURON::RIGHT_HF_TRUNK_LEAK_SWING]->no_tf=false;
        else
            INsens[SENSORY_INTERNEURON::RIGHT_HF_TRUNK_LEAK_SWING]->no_tf=true;
    }
    if(side == SIDE::LEFT){
        INsens[SENSORY_INTERNEURON::LEFT_SOL_Ib_STANCE] = new SensoryInterneuron("sol__mff_stance",ACTIVE_DURING::STANCE,getInput(side_str+"_sol_force"),motoneurons[MUSCLES::toID(side_str+"_sol")],parameters[0]["solsol_wf_left"]);
        INsens[SENSORY_INTERNEURON::LEFT_TA_II_CYCLE] = new SensoryInterneuron("ta__mlf_stance",ACTIVE_DURING::WHOLE_CYCLE,getInput(side_str+"_ta_length"), motoneurons[MUSCLES::toID(side_str+"_ta")], parameters[0]["tata_wl_left"]);
        INsens[SENSORY_INTERNEURON::LEFT_SOL_LEFT_TA_II_STANCE] = new SensoryInterneuron("sol_ta_mff_stance",ACTIVE_DURING::STANCE,getInput(side_str+"_sol_force"), motoneurons[MUSCLES::toID(side_str+"_ta")], parameters[0]["solta_wf_left"],-1.0);
        INsens[SENSORY_INTERNEURON::LEFT_GAS_Ib_STANCE] = new SensoryInterneuron("gas__mff_stance",ACTIVE_DURING::STANCE,getInput(side_str+"_gas_force"), motoneurons[MUSCLES::toID(side_str+"_gas")], parameters[0]["gasgas_wf_left"]);
        INsens[SENSORY_INTERNEURON::LEFT_VAS_Ib_STANCE] = new SensoryInterneuron("vas__mff_stance",ACTIVE_DURING::STANCE,getInput(side_str+"_vas_force"), motoneurons[MUSCLES::toID(side_str+"_vas")], parameters[0]["vasvas_wf_left"]);
        if(Settings::get<int>("noAngleOffset")==0)
            INsens[SENSORY_INTERNEURON::LEFT_VAS_ANGLE_ANGLE_OFFSET] = new SensoryInterneuron("vas_pko_angleoffset",ACTIVE_DURING::ANGLE_OFFSET,getInput(side_str+"_knee_angle"), motoneurons[MUSCLES::toID(side_str+"_vas")], parameters[0]["kphiknee_left"],-1.0);
        else
            INsens[SENSORY_INTERNEURON::LEFT_VAS_ANGLE_ANGLE_OFFSET] = new SensoryInterneuron("vas_pko_angleoffset",ACTIVE_DURING::STANCE_END,getInput(cside_str+"_foot"), motoneurons[MUSCLES::toID(side_str+"_vas")], parameters[0]["kbodyweight_left"],-1.0);
        INsens[SENSORY_INTERNEURON::LEFT_VAS_RIGHT_FOOT_STANCE_END] = new SensoryInterneuron("vas_gcf_finishingstance",ACTIVE_DURING::STANCE_END,getInput(cside_str+"_foot"), motoneurons[MUSCLES::toID(side_str+"_vas")], parameters[0]["kbodyweight_left"],-1.0);
        INsens[SENSORY_INTERNEURON::LEFT_HAM_TRUNK_PD_STANCE] = new SensorySensoryInterneuron("ham_gif_stance",ACTIVE_DURING::STANCE,getInput(side_str+"_foot"), getInput("trunk_pd"), motoneurons[MUSCLES::toID(side_str+"_ham")], parameters[0]["kbodyweight_left"]);
        INsens[SENSORY_INTERNEURON::LEFT_HAM_Ib_SWING] = new SensoryInterneuron("ham__mff_swing",ACTIVE_DURING::SWING,getInput(side_str+"_ham_force"), motoneurons[MUSCLES::toID(side_str+"_ham")], parameters[0]["hamham_wf_left"]);
        INsens[SENSORY_INTERNEURON::LEFT_GLU_TRUNK_PD2_STANCE] = new SensorySensoryInterneuron("glu_gif_stance",ACTIVE_DURING::STANCE,getInput(side_str+"_foot"), getInput("trunk_pd_0.68"), motoneurons[MUSCLES::toID(side_str+"_glu")], parameters[0]["kbodyweight_left"]);
        INsens[SENSORY_INTERNEURON::LEFT_GLU_Ib_SWING] = new SensoryInterneuron("glu__mff_swing",ACTIVE_DURING::SWING,getInput(side_str+"_glu_force"), motoneurons[MUSCLES::toID(side_str+"_glu")], parameters[0]["gluglu_wf_left"]);
        INsens[SENSORY_INTERNEURON::LEFT_HF_TRUNK_PD_STANCE] = new SensorySensoryInterneuron("hf_gif_stance",ACTIVE_DURING::STANCE,getInput(side_str+"_foot"), getInput("trunk_pd"), motoneurons[MUSCLES::toID(side_str+"_hf")], parameters[0]["kbodyweight_left"],-1.0);
        INsens[SENSORY_INTERNEURON::LEFT_HAM_LEFT_HF_II_SWING] = new SensoryInterneuron("ham_hf_mlf_swing",ACTIVE_DURING::SWING,getInput(side_str+"_ham_length"), motoneurons[MUSCLES::toID(side_str+"_hf")], parameters[0]["hamhf_wl_left"],-1.0);
        INsens[SENSORY_INTERNEURON::LEFT_HF_II_SWING] = new SensoryInterneuron("hf__mlf_swing",ACTIVE_DURING::SWING,getInput(side_str+"_hf_length"), motoneurons[MUSCLES::toID(side_str+"_hf")], parameters[0]["hfhf_wl_left"]);
        INsens[SENSORY_INTERNEURON::LEFT_HF_TRUNK_LEAK_SWING] = new SensoryInterneuron("hf_tl_swing",ACTIVE_DURING::SWING,getInput("trunk_leak"), motoneurons[MUSCLES::toID(side_str+"_hf")], parameters[0]["klean"]);
        if(Settings::get<int>("notf_trunklean") == 0)
            INsens[SENSORY_INTERNEURON::LEFT_HF_TRUNK_LEAK_SWING]->no_tf=false;
        else
            INsens[SENSORY_INTERNEURON::LEFT_HF_TRUNK_LEAK_SWING]->no_tf=true;
    }
    

    
}

void ReflexController::initialiseFeedbacks(){
    //SensorySensoryInterneuron * sss;    
    initialiseFeedbacks(SIDE::RIGHT);
    initialiseFeedbacks(SIDE::LEFT);
}

void ReflexController::initCpgParameters(){
    std::vector<std::string> strs; // the vector containing the key->value pair
    cout << "Cpg scheme variable initialisation";
    ifstream myfile (Settings::get<string>("config")+"cpg_gate/"+Settings::get<string>("config_cpg_scheme"));
    string line ; // the string containing the line
    if (myfile.is_open()){
        while( std::getline( myfile, line ) )
        {
            boost::split(strs, line, boost::is_any_of("\t "));
            string name;
            bool modeled=false;
            if(strs[0][0]!='#'){
                name = strs[0];
                modeled=true;
            }
            else{
                name = strs[0].substr(1);
            }
            
            parameters[2][name]=0.0; // pure feedback
            parameters[3][name]=0.0; // constant cpg
            
            
            if(modeled){
                modelized_sensory_interneurons.push_back(name);
            }
            

            
        }
        
        myfile.close();
    }
    else{
        cout << "ERROR: unable to open file '" << Settings::get<string>("config")+"cpg_gate/" << Settings::get<string>("config_cpg_scheme") << "'. In:Parameters.cc, constructor(string, string)"<<endl;
    }
}
void ReflexController::loadCpgFile(){
    std::vector<std::string> strs; // the vector containing the key->value pair
    cout << "Loading cpg scheme : " << Settings::get<string>("config_cpg_scheme") << endl;
    ifstream myfile (Settings::get<string>("config")+"cpg_gate/"+Settings::get<string>("config_cpg_scheme"));
    string line ; // the string containing the line
    if(Settings::get<string>("config_cpg_type") == "replace")
        cout << "loading in replacing mode" << endl;
    if(Settings::get<string>("config_cpg_type") == "full")
        cout << "loading in full mode" << endl;
    if (myfile.is_open()){
        while( std::getline( myfile, line ) )
        {
            boost::split(strs, line, boost::is_any_of("\t "));
            string name;
            bool modeled=false;
            if(strs[0][0]!='#'){
                name = strs[0];
                modeled=true;
            }
            else{
                name = strs[0].substr(1);
            }
            
            parameters[2][name]=0.0; // pure feedback
            parameters[3][name]=0.0; // constant cpg
            
            if(modeled){
                modelized_sensory_interneurons.push_back(name);
                cout << "\t" << name << " : ";
                if(Settings::get<string>("config_cpg_type") == "replace")
                {
                    string type = "cpg";
                    if(strs.size()>1){
                        if(strs[1] == "cst"){
                            parameters[3][name]=1.0;
                            type = "cst";
                        }
                        else if(strs[1] == "cpg"){
                            parameters[3][name]=0.0;
                        }
                        else{
                            type = "unknown";
                            cout << "unknown cpg replacing type : ''" << strs[1] << "'', should be either ''cst'' or ''cpg''" << endl;
                        }
                    }
                    
                    cout << "\t" <<  parameters[3][name] << ", type=" << type << endl;
                }
                if(Settings::get<string>("config_cpg_type") == "full")
                {
                    string type = "cpg";
                    if(strs.size()>1){
                        if(strs[1] == "cst"){
                            parameters[3][name]=1.0;
                            type = "cst";
                        }
                        else if(strs[1] == "cpg"){
                            parameters[3][name]=0.0;
                        }
                        else{
                            type = "unknown";
                            cout << "unknown cpg type : ''" << strs[1] << "'', should be either ''cst'' or ''cpg''" << endl;
                        }
                    }
                    if(strs.size()>2){
                        parameters[2][name]=convertToDouble(strs[2]);
                    }
                    else{
                        parameters[2][name]=1.0;
                    }
                    
                    cout << "\t" <<  parameters[3][name] << ", type=" << type << ", contribution=" << parameters[2][name] << endl;
                }
            }
            

            
        }
        
        myfile.close();
    }
    else{
        cout << "ERROR: unable to open file '" << Settings::get<string>("config")+"cpg_gate/" << Settings::get<string>("config_cpg_scheme") << "'. In:Parameters.cc, constructor(string, string)"<<endl;
    }
    
}


void ReflexController::initialiseCpgs(std::map<std::string, Sensor *> sensors){
    Foot * left_foot = static_cast<Foot *>(sensors["left_foot"]);
    Foot * right_foot = static_cast<Foot *>(sensors["right_foot"]);
    CPGInterneuron * cs;    
    /*
     * load interneurons to be modelized
     */
    initCpgParameters();
    INcpgs[SENSORY_INTERNEURON::RIGHT_SOL_Ib_STANCE] = new CPGInterneuron(ACTIVE_DURING::STANCE,time_step, motoneurons[MUSCLES::RIGHT_SOL], right_foot, "sol__mff_stance");
    INcpgs[SENSORY_INTERNEURON::RIGHT_TA_II_CYCLE] = new CPGInterneuron(ACTIVE_DURING::WHOLE_CYCLE,time_step, motoneurons[MUSCLES::RIGHT_TA], right_foot, "ta__mlf_cycle");
    INcpgs[SENSORY_INTERNEURON::RIGHT_SOL_RIGHT_TA_II_STANCE] = new CPGInterneuron(ACTIVE_DURING::STANCE,time_step, motoneurons[MUSCLES::RIGHT_TA], right_foot, "sol_ta_mff_stance");
    INcpgs[SENSORY_INTERNEURON::RIGHT_GAS_Ib_STANCE] = new CPGInterneuron(ACTIVE_DURING::STANCE,time_step, motoneurons[MUSCLES::RIGHT_GAS], right_foot, "gas__mff_stance");
    INcpgs[SENSORY_INTERNEURON::RIGHT_VAS_Ib_STANCE] = new CPGInterneuron(ACTIVE_DURING::STANCE,time_step, motoneurons[MUSCLES::RIGHT_VAS], right_foot, "vas__mff_stance");
    INcpgs[SENSORY_INTERNEURON::RIGHT_VAS_ANGLE_ANGLE_OFFSET] = new CPGInterneuron(ACTIVE_DURING::ANGLE_OFFSET, time_step, motoneurons[MUSCLES::RIGHT_VAS], right_foot, "vas_pko_angleoffset");
    INcpgs[SENSORY_INTERNEURON::RIGHT_VAS_LEFT_FOOT_STANCE_END] = new CPGInterneuron(ACTIVE_DURING::STANCE_END, time_step, motoneurons[MUSCLES::RIGHT_VAS], right_foot, "vas_gcf_finishingstance");
    INcpgs[SENSORY_INTERNEURON::RIGHT_HAM_TRUNK_PD_STANCE] = new CPGInterneuron(ACTIVE_DURING::STANCE,time_step, motoneurons[MUSCLES::RIGHT_HAM], right_foot, "ham_gif_stance");
    INcpgs[SENSORY_INTERNEURON::RIGHT_HAM_Ib_SWING] = new CPGInterneuron(ACTIVE_DURING::SWING,time_step, motoneurons[MUSCLES::RIGHT_HAM], right_foot, "ham__mff_swing");
    INcpgs[SENSORY_INTERNEURON::RIGHT_GLU_TRUNK_PD2_STANCE] = new CPGInterneuron(ACTIVE_DURING::STANCE,time_step, motoneurons[MUSCLES::RIGHT_GLU], right_foot, "glu_gif_stance");
    INcpgs[SENSORY_INTERNEURON::RIGHT_GLU_Ib_SWING] = new CPGInterneuron(ACTIVE_DURING::SWING,time_step, motoneurons[MUSCLES::RIGHT_GLU], right_foot, "glu__mff_swing");
    INcpgs[SENSORY_INTERNEURON::RIGHT_HF_TRUNK_PD_STANCE] = new CPGInterneuron(ACTIVE_DURING::STANCE,time_step, motoneurons[MUSCLES::RIGHT_HF], right_foot, "hf_gif_stance");
    INcpgs[SENSORY_INTERNEURON::RIGHT_HAM_RIGHT_HF_II_SWING] = new CPGInterneuron(ACTIVE_DURING::SWING,time_step, motoneurons[MUSCLES::RIGHT_HF], right_foot, "ham_hf_mlf_swing");
    INcpgs[SENSORY_INTERNEURON::RIGHT_HF_II_SWING] = new CPGInterneuron(ACTIVE_DURING::SWING,time_step, motoneurons[MUSCLES::RIGHT_HF], right_foot, "hf__mlf_swing");
    INcpgs[SENSORY_INTERNEURON::RIGHT_HF_TRUNK_LEAK_SWING] = new CPGInterneuron(ACTIVE_DURING::SWING,time_step, motoneurons[MUSCLES::RIGHT_HF], right_foot, "hf_tl_swing");
    INcpgs[SENSORY_INTERNEURON::LEFT_SOL_Ib_STANCE] = new CPGInterneuron(ACTIVE_DURING::STANCE,time_step, motoneurons[MUSCLES::LEFT_SOL], left_foot,"sol__mff_stance");
    INcpgs[SENSORY_INTERNEURON::LEFT_TA_II_CYCLE] = new CPGInterneuron(ACTIVE_DURING::WHOLE_CYCLE,time_step, motoneurons[MUSCLES::LEFT_TA], left_foot,"ta__mlf_cycle");
    INcpgs[SENSORY_INTERNEURON::LEFT_SOL_LEFT_TA_II_STANCE] = new CPGInterneuron(ACTIVE_DURING::STANCE,time_step, motoneurons[MUSCLES::LEFT_TA], left_foot,"sol_ta_mff_stance");
    INcpgs[SENSORY_INTERNEURON::LEFT_GAS_Ib_STANCE] = new CPGInterneuron(ACTIVE_DURING::STANCE,time_step, motoneurons[MUSCLES::LEFT_GAS], left_foot,"gas__mff_stance");
    INcpgs[SENSORY_INTERNEURON::LEFT_VAS_Ib_STANCE] = new CPGInterneuron(ACTIVE_DURING::STANCE,time_step, motoneurons[MUSCLES::LEFT_VAS], left_foot,"vas__mff_stance");
    INcpgs[SENSORY_INTERNEURON::LEFT_VAS_ANGLE_ANGLE_OFFSET] = new CPGInterneuron(ACTIVE_DURING::ANGLE_OFFSET, time_step, motoneurons[MUSCLES::LEFT_VAS], left_foot,"vas_pko_angleoffset");
    INcpgs[SENSORY_INTERNEURON::LEFT_VAS_RIGHT_FOOT_STANCE_END] = new CPGInterneuron(ACTIVE_DURING::STANCE_END, time_step, motoneurons[MUSCLES::LEFT_VAS], left_foot,"vas_gcf_finishingstance");
    INcpgs[SENSORY_INTERNEURON::LEFT_HAM_TRUNK_PD_STANCE] = new CPGInterneuron(ACTIVE_DURING::STANCE,time_step, motoneurons[MUSCLES::LEFT_HAM], left_foot,"ham_gif_stance");
    INcpgs[SENSORY_INTERNEURON::LEFT_HAM_Ib_SWING] = new CPGInterneuron(ACTIVE_DURING::SWING,time_step, motoneurons[MUSCLES::LEFT_HAM], left_foot,"ham__mff_swing");
    INcpgs[SENSORY_INTERNEURON::LEFT_GLU_TRUNK_PD2_STANCE] = new CPGInterneuron(ACTIVE_DURING::STANCE,time_step, motoneurons[MUSCLES::LEFT_GLU], left_foot,"glu_gif_stance");
    INcpgs[SENSORY_INTERNEURON::LEFT_GLU_Ib_SWING] = new CPGInterneuron(ACTIVE_DURING::SWING,time_step, motoneurons[MUSCLES::LEFT_GLU], left_foot,"glu__mff_swing");
    INcpgs[SENSORY_INTERNEURON::LEFT_HF_TRUNK_PD_STANCE] = new CPGInterneuron(ACTIVE_DURING::STANCE,time_step, motoneurons[MUSCLES::LEFT_HF], left_foot,"hf_gif_stance");
    INcpgs[SENSORY_INTERNEURON::LEFT_HAM_LEFT_HF_II_SWING] = new CPGInterneuron(ACTIVE_DURING::SWING,time_step, motoneurons[MUSCLES::LEFT_HF], left_foot,"ham_hf_mlf_swing");
    INcpgs[SENSORY_INTERNEURON::LEFT_HF_II_SWING] = new CPGInterneuron(ACTIVE_DURING::SWING,time_step, motoneurons[MUSCLES::LEFT_HF], left_foot,"hf__mlf_swing");
    INcpgs[SENSORY_INTERNEURON::LEFT_HF_TRUNK_LEAK_SWING] = new CPGInterneuron(ACTIVE_DURING::SWING,time_step, motoneurons[MUSCLES::LEFT_HF], left_foot,"hf_tl_swing");
}


void ReflexController::initialiseMotoneurons(){
    motoneurons[MUSCLES::LEFT_SOL]  = new Motoneuron(SIDE::LEFT,"sol",getOutput(MUSCLES::toString(MUSCLES::LEFT_SOL)),time_step);
    motoneurons[MUSCLES::LEFT_TA]   = new Motoneuron(SIDE::LEFT,"ta",getOutput(MUSCLES::toString(MUSCLES::LEFT_TA)),time_step);
    motoneurons[MUSCLES::LEFT_GAS]  = new Motoneuron(SIDE::LEFT,"gas",getOutput(MUSCLES::toString(MUSCLES::LEFT_GAS)),time_step);
    motoneurons[MUSCLES::LEFT_VAS]  = new Motoneuron(SIDE::LEFT,"vas",getOutput(MUSCLES::toString(MUSCLES::LEFT_VAS)),time_step);
    motoneurons[MUSCLES::LEFT_HAM]  = new Motoneuron(SIDE::LEFT,"ham",getOutput(MUSCLES::toString(MUSCLES::LEFT_HAM)),time_step);
    motoneurons[MUSCLES::LEFT_GLU]  = new Motoneuron(SIDE::LEFT,"glu",getOutput(MUSCLES::toString(MUSCLES::LEFT_GLU)),time_step);
    motoneurons[MUSCLES::LEFT_HF]   = new Motoneuron(SIDE::LEFT,"hf",getOutput(MUSCLES::toString(MUSCLES::LEFT_HF)),time_step);

    motoneurons[MUSCLES::RIGHT_SOL] = new Motoneuron(SIDE::RIGHT,"sol",getOutput(MUSCLES::toString(MUSCLES::RIGHT_SOL)),time_step);
    motoneurons[MUSCLES::RIGHT_TA]  = new Motoneuron(SIDE::RIGHT,"ta",getOutput(MUSCLES::toString(MUSCLES::RIGHT_TA)),time_step);
    motoneurons[MUSCLES::RIGHT_GAS] = new Motoneuron(SIDE::RIGHT,"gas",getOutput(MUSCLES::toString(MUSCLES::RIGHT_GAS)),time_step);
    motoneurons[MUSCLES::RIGHT_VAS] = new Motoneuron(SIDE::RIGHT,"vas",getOutput(MUSCLES::toString(MUSCLES::RIGHT_VAS)),time_step);
    motoneurons[MUSCLES::RIGHT_HAM] = new Motoneuron(SIDE::RIGHT,"ham",getOutput(MUSCLES::toString(MUSCLES::RIGHT_HAM)),time_step);
    motoneurons[MUSCLES::RIGHT_GLU] = new Motoneuron(SIDE::RIGHT,"glu",getOutput(MUSCLES::toString(MUSCLES::RIGHT_GLU)),time_step);
    motoneurons[MUSCLES::RIGHT_HF]  = new Motoneuron(SIDE::RIGHT,"hf",getOutput(MUSCLES::toString(MUSCLES::RIGHT_HF)),time_step);
}


/* 
 * load all feedbacks (as defined in initialiseFeedbacks) in the interneurones network so that they act on motoneurons
 */
void ReflexController::loadFeedback(SIDE::Side side){
    bool (*correct_side)(int);
    if(side == SIDE::RIGHT){
#ifndef REAL_TIME
        cout << "right feedback_loaded" <<  endl;
#endif
        feedback_right_loaded = true;
        correct_side = MUSCLES::IS_RIGHT;
    }
    else if(side == SIDE::LEFT){
#ifndef REAL_TIME
        cout << "left feedback_loaded" <<  endl;
#endif
        correct_side = MUSCLES::IS_LEFT;
        feedback_left_loaded = true;
    }
    static int delay = Settings::get<int>("delay");
    for (int INsen=SENSORY_INTERNEURON::FIRST; INsen<=SENSORY_INTERNEURON::LAST; INsen++){
        for(int MN=MUSCLES::FIRST; MN<=MUSCLES::LAST; MN++){
            if(correct_side(MN) && SENSORY_INTERNEURON::CONNECTION_TO_MOTONEURONS[INsen][MN] != 0.0){
                if(delay == 1){
                    int delay = SENSORY_INTERNEURON::CONNECTION_TO_MOTONEURONS[INsen][MN];
                    __INsen_MN[INsen][MN] = new DiscreteDelayConnection(INsens[INsen],motoneurons[MN],delay);
                }
                else{
                    __INsen_MN[INsen][MN] = new NoDelayConnection(INsens[INsen],motoneurons[MN]);
                }
                //__INsen_MN[INsen][MN] = new FirstOrderConnection(INsens[INsen],motoneurons[MN]);
            }   
        }
    }
}
// Load cpg replace feedback
void ReflexController::loadCpg(SIDE::Side side){
    bool (*correct_side)(int);
    if(side == SIDE::RIGHT){
#ifndef REAL_TIME
        cout << "right cpgs loaded" <<  endl;
#endif
        cpg_right_loaded = true;
        correct_side = MUSCLES::IS_RIGHT;
    }
    else if(side == SIDE::LEFT){
#ifndef REAL_TIME
        cout << "left cpgs loaded" <<  endl;
#endif
        correct_side = MUSCLES::IS_LEFT;
        cpg_left_loaded = true;
    }
    
    
#ifndef REAL_TIME
    if(Settings::get<string>("config_cpg_type") == "combine"){
        cout << "Combinaison strategy" << endl;
    }
    else if(Settings::get<string>("config_cpg_type") == "replace"){
        cout << "Replacing strategy" << endl;
    }
    else{
        cout << "Unknown strategy, nothing will happen" << endl;
#endif
    }
    static string config_cpg_type = Settings::get<string>("config_cpg_type");
    for (int INcpg=SENSORY_INTERNEURON::FIRST; INcpg<=SENSORY_INTERNEURON::LAST; INcpg++){
        for(int MN=MUSCLES::FIRST; MN<=MUSCLES::LAST; MN++){
            if(correct_side(MN) && SENSORY_INTERNEURON::CONNECTION_TO_MOTONEURONS[INcpg][MN]){
                if(config_cpg_type == "combine"){
                    __INcpg_MN[INcpg][MN] = new NoDelayConnection(INcpgs[INcpg],motoneurons[MN]);
                }
                else if(config_cpg_type == "replace"){
                    __INsen_MN[INcpg][MN] = new NoDelayConnection(INcpgs[INcpg],motoneurons[MN]);
                }
#ifndef REAL_TIME
                cout << "\t loading : " << SENSORY_INTERNEURON::toString(INcpg) << endl;
#endif
            }   
        }
    }
}

// /* 
//  * replace feedbacks by their cpgs counterpart (for cpgs defined in initialiseCpgs)
//  */
void ReflexController::loadPartialCpg(SIDE::Side side){
    bool (*correct_side)(int);
    if(side == SIDE::RIGHT){
#ifndef REAL_TIME
        cout << "right cpgs loaded" <<  endl;
#endif
        cpg_right_loaded = true;
        correct_side = MUSCLES::IS_RIGHT;
    }
    else if(side == SIDE::LEFT){
#ifndef REAL_TIME
        cout << "left cpgs loaded" <<  endl;
#endif
        correct_side = MUSCLES::IS_LEFT;
        cpg_left_loaded = true;
    }
    static string config_cpg_type = Settings::get<string>("config_cpg_type");
#ifndef REAL_TIME
    if(config_cpg_type == "combine"){
        cout << "Combinaison strategy" << endl;
    }
    else if(config_cpg_type == "replace"){
        cout << "Replacing strategy" << endl;
    }
    else if(config_cpg_type == "full"){
        cout << "Full strategy" << endl;
    }
    else{
        cout << "Unknown strategy, nothing will happen" << endl;
    }
#endif
    
    for (int INcpg=SENSORY_INTERNEURON::FIRST; INcpg<=SENSORY_INTERNEURON::LAST; INcpg++){
        for(int MN=MUSCLES::FIRST; MN<=MUSCLES::LAST; MN++){
            if(correct_side(MN) && SENSORY_INTERNEURON::CONNECTION_TO_MOTONEURONS[INcpg][MN]){
                auto cc = count_if(begin(modelized_sensory_interneurons), 
                    end(modelized_sensory_interneurons), 
                    [&](const string& str) {
                      return SENSORY_INTERNEURON::toString(INcpg).find(str) != std::string::npos;
                    });
                if( cc != 0 ){
                    if(config_cpg_type == "combine" || config_cpg_type == "full"){
                        __INcpg_MN[INcpg][MN] = new NoDelayConnection(INcpgs[INcpg],motoneurons[MN]);
                    }
                    else if(config_cpg_type == "replace"){
                        __INsen_MN[INcpg][MN] = new NoDelayConnection(INcpgs[INcpg],motoneurons[MN]);
                    }
#ifndef REAL_TIME
                    cout << "\t loading : " << SENSORY_INTERNEURON::toString(INcpg) << endl;
#endif
                }
            }   
        }
    }
}
