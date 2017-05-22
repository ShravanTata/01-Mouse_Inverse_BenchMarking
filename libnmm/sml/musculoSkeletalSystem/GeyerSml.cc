#include "GeyerSml.hh"

#include "Interneuron.hh"
#include "Motoneuron.hh"
#include "Muscle.hh"
#include "Joint.hh"
#include "Sensor.hh"
#include <sml/sml-tools/Settings.hh>
#include <sml/sml-tools/EventManager.hh>
#include <sml/sml-tools/OnlineCtrl.hh>

#include <boost/xpressive/xpressive.hpp>

extern EventManager* eventManager;
extern SmlParameters parameters;
using namespace std;
using namespace boost::xpressive;



void GeyerSml::initialiseSpinalControl(){
    for(auto&kv : sensors){
        geyerController.inputs.get<const double*>(kv.first) = &kv.second->get();
    } 
    geyerController.inputs.get<const SIDE::Side*>("finishing_stance") = &finishing_stance;

    for(int i=0; i< muscles.size(); i++){
        geyerController.outputs.get<double*>(MUSCLES::toString(i)) = &muscles[i]->stim;
    }
    geyerController.initialiseMotoneurons();

    geyerController.initialiseFeedbacks();

    geyerController.initialiseCpgs(sensors);
}


/* 
 * load all feedbacks (as defined in initialiseFeedbacks) in the interneurones network so that they act on motoneurons
 */
void GeyerSml::loadFeedback(SIDE::Side side){
    geyerController.loadFeedback(side);
}
// Load cpg replace feedback
void GeyerSml::loadCpg(SIDE::Side side){
    geyerController.loadCpg(side);
}

/* 
 * replace feedbacks by their cpgs counterpart (for cpgs defined in initialiseCpgs)
 */
void GeyerSml::loadPartialCpg(SIDE::Side side){
    geyerController.loadPartialCpg(side);
}

void GeyerSml::step_SPINE_TO_MTU(double dt){
    geyerController.update();
    static bool stance;
    static double pre_stimulation;
    for(auto& muscle: muscles){
        stance = false;
        if( muscle->side == SIDE::LEFT && left_foot->inStance())
            stance = true;
        if( muscle->side == SIDE::RIGHT && right_foot->inStance())
            stance = true;
        
        string side;
        if (muscle->side == SIDE::LEFT)
            side = "left";
        else
            side = "right";
        
        pre_stimulation = parameters[0][muscle->name+"_activitybasal_"+side] == 0 ? 0.01 : parameters[0][muscle->name+"_activitybasal_"+side];
        // DEPRECATED
        // if (stance)
        // {
        //     if ( muscle->name == "vas" )
        //         pre_stimulation = parameters[0]["vas_activitybasal_stance_"+side] != 0 ? parameters[0]["vas_activitybasal_stance_"+side] : pre_stimulation ;
        //     else if ( muscle->name == "ham" ||
        //          muscle->name == "gas" ||
        //          muscle->name == "hf")
        //         pre_stimulation = parameters[0]["bal_activitybasal_stance_"+side] != 0 ? parameters[0]["bal_activitybasal_stance_"+side] : pre_stimulation;
        // }
        muscle->stim += pre_stimulation;
    }
    if(right_foot->inStance() && left_foot->inStance()){
        
        if(finishing_stance == SIDE::RIGHT){ //double support and right finish the stance
            muscles[MUSCLES::RIGHT_GLU]->stim -= parameters[0]["deltas"];
            muscles[MUSCLES::RIGHT_HF]->stim += parameters[0]["deltas"]; 
        }
        if(finishing_stance == SIDE::LEFT){ //double support and left finish the stance
            muscles[MUSCLES::LEFT_GLU]->stim -= parameters[0]["deltas"];
            muscles[MUSCLES::LEFT_HF]->stim += parameters[0]["deltas"];
        }
    }
    print_debug("[ok] : compute muscles stimulation (Sml.cc)");
}

