#include <sml/types/IO.hh>
#include <string>
using namespace std;
namespace INPUT{
    string toString(int id){
        string result="input not found";
        switch((Inputs) (id)) {
            case ANGLE_HIP_LEFT:
                result="ANGLE_HIP_LEFT";
                break;
            case ANGLE_KNEE_LEFT:
                result="ANGLE_KNEE_LEFT";
                break;
            case ANGLE_ANKLE_LEFT:
                result="ANGLE_ANKLE_LEFT";
                break;
            case ANGLE_HIP_RIGHT:
                result="ANGLE_HIP_RIGHT";
                break;
            case ANGLE_KNEE_RIGHT:
                result="ANGLE_KNEE_RIGHT";
                break;
            case ANGLE_ANKLE_RIGHT:
                result="ANGLE_ANKLE_RIGHT";
                break;
            case NECK_Y:
                result="NECK_Y";
                break;
            case NECK_X:
                result="NECK_X";
                break;
            case THETA_TRUNK:
                result="THETA_TRUNK";
                break;
            case TOE_LEFT_X:
                result="TOE_LEFT_X";
                break;
            case TOE_LEFT_Y:
                result="TOE_LEFT_Y";
                break;
            case TOE_LEFT_Z:
                result="TOE_LEFT_Z";
                break;
            case TOE_RIGHT_X:
                result="TOE_RIGHT_X";
                break;
            case TOE_RIGHT_Y:
                result="TOE_RIGHT_Y";
                break;
            case TOE_RIGHT_Z:
                result="TOE_RIGHT_Z";
                break;
            case HEEL_LEFT_X:
                result="HEEL_LEFT_X";
                break;
            case HEEL_LEFT_Y:
                result="HEEL_LEFT_Y";
                break;
            case HEEL_LEFT_Z:
                result="HEEL_LEFT_Z";
                break;
            case HEEL_RIGHT_X:
                result="HEEL_RIGHT_X";
                break;
            case HEEL_RIGHT_Y:
                result="HEEL_RIGHT_Y";
                break;
            case HEEL_RIGHT_Z:
                result="HEEL_RIGHT_Z";
                break;
            case MIDHIP_X:
                result="MIDHIP_X";
                break;
            case MIDHIP_Y:
                result="MIDHIP_Y";
                break;
            case MIDHIP_Z:
                result="MIDHIP_Z";
                break;
            case SENSOR_HEEL_LEFT:
                result="SENSOR_HEEL_LEFT";
                break;
            case SENSOR_HEEL_RIGHT:
                result="SENSOR_HEEL_RIGHT";
                break;
            case SENSOR_TOE_LEFT:
                result="SENSOR_TOE_LEFT";
                break;
            case SENSOR_TOE_RIGHT:
                result="SENSOR_TOE_RIGHT";
                break;
            //case LEFT_FOOT_ORIENTATION:
                //result="LEFT_FOOT_ORIENTATION";
                //break;
            //case RIGHT_FOOT_ORIENTATION:
                //result="RIGHT_FOOT_ORIENTATION";
            }
        return result;
    }
}

namespace OUTPUT {
    string toString(int id){
        string result="output not found";
        switch((Outputs) (id)) {
            case TORQUE_HIP_LEFT:
                result="TORQUE_HIP_LEFT";
                break;
            case TORQUE_KNEE_LEFT:
                result="TORQUE_KNEE_LEFT";
                break;
            case TORQUE_ANKLE_LEFT:
                result="TORQUE_ANKLE_LEFT";
                break;
            case TORQUE_HIP_RIGHT:
                result="TORQUE_HIP_RIGHT";
                break;
            case TORQUE_KNEE_RIGHT:
                result="TORQUE_KNEE_RIGHT";
                break;
            case TORQUE_ANKLE_RIGHT:
                result="TORQUE_ANKLE_RIGHT";
                break;
            }
        return result;
    }
}