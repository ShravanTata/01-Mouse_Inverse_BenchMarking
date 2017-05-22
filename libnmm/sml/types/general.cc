#include <sml/types/general.hh>
#include <string>
#include <iostream>
using namespace std;
namespace JOINT{
    string toString(int id){
        string result="joint not found";
        switch((Joints) (id)){
            case HIP_LEFT:
                result="HIP_LEFT";
                break;
            case KNEE_LEFT:
                result="KNEE_LEFT";
                break;
            case ANKLE_LEFT:
                result="ANKLE_LEFT";
                break;
            case HIP_RIGHT:
                result="HIP_RIGHT";
                break;
            case KNEE_RIGHT:
                result="KNEE_RIGHT";
                break;
            case ANKLE_RIGHT:
                result="ANKLE_RIGHT";
                break;
        }
        return result;
    }
}

string SIDE::toString(int id){
    string result="";
    switch((Side) (id)) {
        case LEFT:
            result="LEFT";
            break;
        case RIGHT:
            result="RIGHT";
            break;
        default:
            result="";
        }
    return result;
}
string CYCLE_PHASE::toString(int id){
    string result="";
    switch((CyclePhase) (id)) {
        case WHOLE_CYCLE:
                result="WHOLE_CYCLE";
                break;
        case STANCE:
                result="STANCE";
                break;
        case STANCE_START:
                result="STANCE_START";
                break;
        case STANCE_END:
                result="STANCE_END";
                break;
        case SWING:
                result="SWING";
                break;
        case SWING_START:
                result="SWING_START";
                break;
        case SWING_END:
                result="SWING_END";
                break;
        case ANGLE_OFFSET:
                result="ANGLE_OFFSET";
                break;
    }
    return result;
}
namespace STOP_REASONS{
    string toString(int id){
        string result="";
        switch((StopReasons) (id)){
            case FALL:
                    result="FALL";
                    break;
            case TRUNK_LEANING_LIMIT:
                    result="TRUNK_LEANING_LIMIT";
                    break;
            case EXPLOD:
                    result="EXPLOD";
                    break;
            case UNSTABLE:
                    result="UNSTABLE";
                    break;
            case TO_SLOW:
                    result="TO_SLOW";
                    break;
            case WRONG_SIDE:
                    result="WRONG_SIDE";
                    break;
            case OTHER:
                    result="OTHER";
                    break;
        }
        return result;
    }
}
namespace ACTIVE_DURING{
    string toString(int id){
        string result="";
        switch((ActiveDuring) (id)) {
            case WHOLE_CYCLE:
                    result="WHOLE_CYCLE";
                    break;
            case STANCE:
                    result="STANCE";
                    break;
            case STANCE_START:
                    result="STANCE_START";
                    break;
            case STANCE_END:
                    result="STANCE_END";
                    break;
            case SWING:
                    result="SWING";
                    break;
            case SWING_START:
                    result="SWING_START";
                    break;
            case SWING_END:
                    result="SWING_END";
                    break;
            case ANGLE_OFFSET:
                    result="ANGLE_OFFSET";
                    break;
        }
        return result;
    }
    int toID(string str){
        int result = -1;
        if(str=="STANCE_START" || str=="stancestart")
            result = ACTIVE_DURING::STANCE_START;
        if(str=="STANCE_END" || str=="stanceend" || str=="finishingstance")
            result = ACTIVE_DURING::STANCE_END;
        if(str=="STANCE" || str=="stance")
            result = ACTIVE_DURING::STANCE;
        if(str=="SWING_START" || str=="swingstart")
            result = ACTIVE_DURING::SWING_START;
        if(str=="SWING_END" || str=="swingend")
            result = ACTIVE_DURING::SWING_END;
        if(str=="SWING" || str=="swing")
            result = ACTIVE_DURING::SWING;
        if(str=="ANGLE_OFFSET" || str=="angleoffset")
            result = ACTIVE_DURING::ANGLE_OFFSET;
        if(str=="WHOLE_CYCLE" || str=="cycle")
            result = ACTIVE_DURING::WHOLE_CYCLE;
        
        return result;
    }

    bool isActive(
                ACTIVE_DURING::ActiveDuring active_on,
                CYCLE_PHASE::CyclePhase current_limb_state
                )
    {
        bool ret = false;
        if(
            active_on == ACTIVE_DURING::STANCE
            &&
            ( current_limb_state == CYCLE_PHASE::STANCE_START
                ||
                current_limb_state == CYCLE_PHASE::STANCE_END
            )
        ) 
            ret = true;
                
        if(
            active_on == ACTIVE_DURING::SWING
            &&
            ( current_limb_state == CYCLE_PHASE::SWING_START
                ||
                current_limb_state == CYCLE_PHASE::SWING_END
            )
        ) 
            ret = true;
            
        if((int)current_limb_state == (int)active_on)
            ret = true;
        return ret;
    }
}

namespace EKERBERG_PHASE{
    std::string ToString(int id)
    {   std::string result;
        switch(id)
        {
            case SWING:
                result = "SWING";
                break;
            case TOUCH_DOWN:
                result = "TOUCH_DOWN";
                break;
            case STANCE:
                result = "STANCE";
                break;
            case LIFT_OFF:
                result = "LIFT_OFF";
                break;
            default :
                result = "not found";
        }
        return result;
    }
}

extern bool debug;

void print_debug(std::string t){
#ifdef REAL_TIME
    return;
#endif
    if (debug){
        std::cout << t << std::endl;
    }
}