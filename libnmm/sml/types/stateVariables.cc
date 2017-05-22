#include <sml/types/stateVariables.hh>
#include <string>
using namespace std;
using namespace STATES;
string STATES::toString(int id){
    string result;
    switch(id){
        case LEFT_FOOT:
            result="left_foot";
            break;
        case RIGHT_FOOT:
            result="right_foot";
            break;
        case CYCLE_LENGTH:
            result="cycle_length";
            break;
        case LEFT_STEP_LENGTH:
            result="left_step_length";
            break;
        case RIGHT_STEP_LENGTH:
            result="right_step_length";
            break;
        case LEFT_CYCLE_DURATION:
            result="left_cycle_duration";
            break;
        case CURRENT_LEFT_CYCLE_DURATION:
            result="current_left_cycle_duration";
            break;
        case LEFT_STANCE_DURATION:
            result="left_stance_duration";
            break;
        case LEFT_STANCE_END_DURATION:
            result="left_stance_end_duration";
            break;
        case LEFT_SWING_DURATION:
            result="left_swing_duration";
            break;
        case RIGHT_CYCLE_DURATION:
            result="right_cycle_duration";
            break;
        case RIGHT_STANCE_DURATION:
            result="right_stance_duration";
            break;
        case RIGHT_STANCE_END_DURATION:
            result="right_stance_end_duration";
            break;
        case RIGHT_SWING_DURATION:
            result="right_swing_duration";
            break;
        case LEFT_GOLDEN_RATIO_0:
            result="left_golden_ratio_0";
            break;
        case LEFT_GOLDEN_RATIO_1:
            result="left_golden_ratio_1";
            break;
        case LEFT_GOLDEN_RATIO_2:
            result="left_golden_ratio_2";
            break;
        case RIGHT_GOLDEN_RATIO_0:
            result="right_golden_ratio_0";
            break;
        case RIGHT_GOLDEN_RATIO_1:
            result="right_golden_ratio_1";
            break;
        case RIGHT_GOLDEN_RATIO_2:
            result="right_golden_ratio_2";
            break;
        case FALLED:
            result="falled";
            break;
        case POSITION_X:
            result="position_x";
            break;
        case OLD_POSITION_X:
            result="old_position_x";
            break;
        case POSITION_Y:
            result="position_y";
            break;
        case OLD_POSITION_Y:
            result="old_position_y";
            break;
        case IN_VELOCITY:
            result="in_velocity";
            break;
        case OLD_IN_VELOCITY:
            result="old_in_velocity";
            break;
        case IN_ACCELERATION:
            result="in_acceleration";
            break;
        case IN_VELOCITY_FILTERED:
            result="in_velocity_filtered";
            break;
        case IN_ACCELERATION_FILTERED:
            result="in_acceleration_filtered";
            break;
        case MEAN_VELOCITY:
            result="mean_velocity";
            break;
        case VAR_VELOCITY:
            result="var_velocity";
            break;
        case MEAN_ACCELERATION:
            result="mean_acceleration";
            break;
        case VAR_ACCELERATION:
            result="var_acceleration";
            break;
        //Global variables (accumulated state)
        case CYCLE_COUNT:
            result="cycle_count";
            break;
        case RESET_AT:
            result="reset_at";
            break;
        case HEIGHT:
            result="height";
            break;
        case DISTANCE:
            result="distance";
            break;
        case TIME:
            result="time";
            break;
        case ENERGY:
            result="energy";
            break;
        case ENERGY_W:
            result="energy_w";
            break;
        case ENERGY_M:
            result="energy_m";
            break;
        case ENERGY_OVEREXTENSION:
            result="energy_overextension";
            break;
        case STEADY_STATE_REACHEDAT:
            result="steady_state_reachedat";
            break;
        //Cycle related variables (averaged state)
        case MEAN_CYCLE_LENGTH:
            result="mean_cycle_length";
            break;
        case VAR_CYCLE_LENGTH:
            result="var_cycle_length";
            break;
        case MEAN_DOUBLESTANCE_DURATION:
            result="mean_doublestance_duration";
            break;
        case VAR_DOUBLESTANCE_DURATION:
            result="var_doublestance_duration";
            break;
        case MEAN_LEFT_CYCLE_DURATION:
            result="mean_left_cycle_duration";
            break;
        case MEAN_LEFT_STANCE_DURATION:
            result="mean_left_stance_duration";
            break;
        case MEAN_LEFT_STANCE_END_DURATION:
            result="mean_left_stance_end_duration";
            break;
        case MEAN_LEFT_SWING_DURATION:
            result="mean_left_swing_duration";
            break;
        case MEAN_RIGHT_CYCLE_DURATION:
            result="mean_right_cycle_duration";
            break;
        case MEAN_RIGHT_STANCE_DURATION:
            result="mean_right_stance_duration";
            break;
        case MEAN_RIGHT_STANCE_END_DURATION:
            result="mean_right_stance_end_duration";
            break;
        case MEAN_RIGHT_SWING_DURATION:
            result="mean_right_swing_duration";
            break;
        case VAR_LEFT_CYCLE_DURATION:
            result="var_left_cycle_duration";
            break;
        case VAR_LEFT_STANCE_DURATION:
            result="var_left_stance_duration";
            break;
        case VAR_LEFT_STANCE_END_DURATION:
            result="var_left_stance_end_duration";
            break;
        case VAR_LEFT_SWING_DURATION:
            result="var_left_swing_duration";
            break;
        case VAR_RIGHT_CYCLE_DURATION:
            result="var_right_cycle_duration";
            break;
        case VAR_RIGHT_STANCE_DURATION:
            result="var_right_stance_duration";
            break;
        case VAR_RIGHT_STANCE_END_DURATION:
            result="var_right_stance_end_duration";
            break;
        case VAR_RIGHT_SWING_DURATION:
            result="var_right_swing_duration";
            break;
        default:
            result="state variable not found";
    }
    return result;
}