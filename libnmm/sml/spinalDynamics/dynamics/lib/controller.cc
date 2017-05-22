#include "controller.hh"
#include "link.hh"
#include <string>

using namespace std;
void TwitchingController::init(){
    network = new Network();
    network->initClock(param.get<double>("dt"));
    for (UInt i=0; i< param.getVector<string>("muscles").size(); i++){
        impulses.push_back(Impulse(network));
        impulses.back().init(
            getOutput(param.getVector<string>("muscles")[i]),
            network->getClock().getTime()+i/10.0);
    }
}

void TwitchingController::update(){
    
    network->update();
    for (UInt i=0; i< param.getVector<string>("muscles").size(); i++){
        if ( impulses[i].l->parameters.get<bool>("impulse/just_started"))
            cout << param.getVector<string>("muscles")[i] << endl;
        //getOutput(param.getVector<string>("muscles")[i]) = impulses[i].getOutput(0);
    }
};

void TwitchingController::update_WithoutDynamics(){

	network->update();
    double t =network->getTime();
    static double sma_start_time = param.get<double>("sma_start_time");
    static double sma_active_twitch_duration = param.get<double>("sma_twitch_duration");
    static double sma_post_twitch_duration = param.get<double>("sma_post_twitch_duration");
    static double sma_twitch_duration = sma_active_twitch_duration + sma_post_twitch_duration;
    static double sma_twitch_activity_level = param.get<double>("sma_twitch_activity_level");

    static int twitched_muscle = -1;
    bool twitching = false;
    if ( t > sma_start_time + sma_twitch_duration * ( twitched_muscle + 1 ) ){
        twitched_muscle++;
        cout << "stimulating : " << param.getVector<string>("muscles")[twitched_muscle] << endl;
    }
    if ( 
        t > sma_start_time + sma_twitch_duration * ( twitched_muscle ) 
        &&
        t < sma_start_time + sma_twitch_duration * ( twitched_muscle ) + sma_active_twitch_duration
        )

    {
        twitching = true;
    }

    for (UInt i=0; i< param.getVector<string>("muscles").size(); i++){
        if( twitched_muscle != -1 && i == static_cast<UInt>(twitched_muscle) && twitching ){
            getOutput(param.getVector<string>("muscles")[i]) = sma_twitch_activity_level;
        }
        else
            getOutput(param.getVector<string>("muscles")[i]) = 0.0;   
    }

    if( static_cast<UInt>(twitched_muscle) >= param.getVector<string>("muscles").size() ){
        cout << "finished" << endl;
        //eventManager->set<bool>(STATES::STAY_IN_LOOP, false);
    }

}
