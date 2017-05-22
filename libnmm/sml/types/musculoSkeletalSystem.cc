#include <sml/types/musculoSkeletalSystem.hh>
#include <sml/types/types.h>
#include <string>
using namespace std;

/*TODO
int SENSORY_INTERNEURON::OUTPUT_MUSCLE(int id){
    std::string str = SENSORY_INTERNEURON::toString(id);
    std::vector<std::string> strs; 
    boost::split(strs, line, boost::is_any_of("_"));
    
    return MUSCLES::toID(strs[0]+" "+strs[1]);
}
int SENSORY_INTERNEURON::ACTIVE_DURING(int id){
    std::string str = SENSORY_INTERNEURON::toString(id);
    std::vector<std::string> strs; 
    boost::split(strs, line, boost::is_any_of("_"));
    
    return ACTIVE_DURING::toID(strs[strs.size()-1]);
}
int SENSORY_INTERNEURON::INPUT_SENSOR(int id){
    std::string str = SENSORY_INTERNEURON::toString(id);
    std::vector<std::string> strs; 
    boost::split(strs, line, boost::is_any_of("_"));
    
    return ACTIVE_DURING::toID(strs[strs.size()-1]);
}
*/
namespace SENSORY_INTERNEURON{
    int CONNECTION_TO_MOTONEURONS[NUMBER][MUSCLES::NUMBER];
    std::map<int,std::string> strs;
    void init(){
        /*
        memset(CONNECTION_TO_MOTONEURONS, false, sizeof(CONNECTION_TO_MOTONEURONS[0][0]) * NUMBER * MUSCLES::NUMBER);
        CONNECTION_TO_MOTONEURONS[RIGHT_HAM_TRUNK_PD_STANCE][MUSCLES::RIGHT_HAM] = true;
        CONNECTION_TO_MOTONEURONS[RIGHT_HF_TRUNK_PD_STANCE][MUSCLES::RIGHT_HF] = true;
        CONNECTION_TO_MOTONEURONS[RIGHT_GLU_TRUNK_PD2_STANCE][MUSCLES::RIGHT_GLU] = true;
        CONNECTION_TO_MOTONEURONS[RIGHT_HF_TRUNK_LEAK_SWING][MUSCLES::RIGHT_HF] = true;
        CONNECTION_TO_MOTONEURONS[LEFT_HAM_TRUNK_PD_STANCE][MUSCLES::LEFT_HAM] = true;
        CONNECTION_TO_MOTONEURONS[LEFT_HF_TRUNK_PD_STANCE][MUSCLES::LEFT_HF] = true;
        CONNECTION_TO_MOTONEURONS[LEFT_GLU_TRUNK_PD2_STANCE][MUSCLES::LEFT_GLU] = true;
        CONNECTION_TO_MOTONEURONS[LEFT_HF_TRUNK_LEAK_SWING][MUSCLES::LEFT_HF] = true;
        CONNECTION_TO_MOTONEURONS[RIGHT_GAS_Ib_STANCE][MUSCLES::RIGHT_GAS] = true;
        CONNECTION_TO_MOTONEURONS[RIGHT_VAS_Ib_STANCE][MUSCLES::RIGHT_VAS] = true;
        CONNECTION_TO_MOTONEURONS[RIGHT_SOL_Ib_STANCE][MUSCLES::RIGHT_SOL] = true;
        CONNECTION_TO_MOTONEURONS[LEFT_GAS_Ib_STANCE][MUSCLES::LEFT_GAS] = true;
        CONNECTION_TO_MOTONEURONS[LEFT_VAS_Ib_STANCE][MUSCLES::LEFT_VAS] = true;
        CONNECTION_TO_MOTONEURONS[LEFT_SOL_Ib_STANCE][MUSCLES::LEFT_SOL] = true;
        CONNECTION_TO_MOTONEURONS[RIGHT_TA_II_CYCLE][MUSCLES::RIGHT_TA] = true;
        CONNECTION_TO_MOTONEURONS[RIGHT_SOL_RIGHT_TA_II_STANCE][MUSCLES::RIGHT_TA] = true;
        CONNECTION_TO_MOTONEURONS[RIGHT_HF_II_SWING][MUSCLES::RIGHT_HF] = true;
        CONNECTION_TO_MOTONEURONS[RIGHT_HAM_RIGHT_HF_II_SWING][MUSCLES::RIGHT_HF] = true;
        CONNECTION_TO_MOTONEURONS[LEFT_TA_II_CYCLE][MUSCLES::LEFT_TA] = true;
        CONNECTION_TO_MOTONEURONS[LEFT_SOL_LEFT_TA_II_STANCE][MUSCLES::LEFT_TA] = true;
        CONNECTION_TO_MOTONEURONS[LEFT_HF_II_SWING][MUSCLES::LEFT_HF] = true;
        CONNECTION_TO_MOTONEURONS[LEFT_HAM_LEFT_HF_II_SWING][MUSCLES::LEFT_HF] = true;
        CONNECTION_TO_MOTONEURONS[RIGHT_VAS_ANGLE_ANGLE_OFFSET][MUSCLES::RIGHT_VAS] = true;
        CONNECTION_TO_MOTONEURONS[LEFT_VAS_ANGLE_ANGLE_OFFSET][MUSCLES::LEFT_VAS] = true;
        CONNECTION_TO_MOTONEURONS[RIGHT_VAS_LEFT_FOOT_STANCE_END][MUSCLES::RIGHT_VAS] = true;
        CONNECTION_TO_MOTONEURONS[LEFT_VAS_RIGHT_FOOT_STANCE_END][MUSCLES::LEFT_VAS] = true;
        CONNECTION_TO_MOTONEURONS[RIGHT_HAM_Ib_SWING][MUSCLES::RIGHT_HAM] = true;
        CONNECTION_TO_MOTONEURONS[RIGHT_GLU_Ib_SWING][MUSCLES::RIGHT_GLU] = true;
        CONNECTION_TO_MOTONEURONS[LEFT_HAM_Ib_SWING][MUSCLES::LEFT_HAM] = true;
        CONNECTION_TO_MOTONEURONS[LEFT_GLU_Ib_SWING][MUSCLES::LEFT_GLU] = true;
        */
        memset(CONNECTION_TO_MOTONEURONS, 0, sizeof(CONNECTION_TO_MOTONEURONS[0][0]) * NUMBER * MUSCLES::NUMBER);

        int ts = Settings::get<double>("delay_short"); //5
        int tm = Settings::get<double>("delay_medium"); //10
        int tl = Settings::get<double>("delay_long"); //20
        
        CONNECTION_TO_MOTONEURONS[RIGHT_HAM_TRUNK_PD_STANCE][MUSCLES::RIGHT_HAM] = ts;
        CONNECTION_TO_MOTONEURONS[RIGHT_HF_TRUNK_PD_STANCE][MUSCLES::RIGHT_HF] = ts;
        CONNECTION_TO_MOTONEURONS[RIGHT_GLU_TRUNK_PD2_STANCE][MUSCLES::RIGHT_GLU] = ts;
        CONNECTION_TO_MOTONEURONS[RIGHT_HF_TRUNK_LEAK_SWING][MUSCLES::RIGHT_HF] = ts;
        CONNECTION_TO_MOTONEURONS[LEFT_HAM_TRUNK_PD_STANCE][MUSCLES::LEFT_HAM] = ts;
        CONNECTION_TO_MOTONEURONS[LEFT_HF_TRUNK_PD_STANCE][MUSCLES::LEFT_HF] = ts;
        CONNECTION_TO_MOTONEURONS[LEFT_GLU_TRUNK_PD2_STANCE][MUSCLES::LEFT_GLU] = ts;
        CONNECTION_TO_MOTONEURONS[LEFT_HF_TRUNK_LEAK_SWING][MUSCLES::LEFT_HF] = ts;
        CONNECTION_TO_MOTONEURONS[RIGHT_GAS_Ib_STANCE][MUSCLES::RIGHT_GAS] = tl;
        CONNECTION_TO_MOTONEURONS[RIGHT_VAS_Ib_STANCE][MUSCLES::RIGHT_VAS] = tm;
        CONNECTION_TO_MOTONEURONS[RIGHT_SOL_Ib_STANCE][MUSCLES::RIGHT_SOL] = tl;
        CONNECTION_TO_MOTONEURONS[LEFT_GAS_Ib_STANCE][MUSCLES::LEFT_GAS] = tl;
        CONNECTION_TO_MOTONEURONS[LEFT_VAS_Ib_STANCE][MUSCLES::LEFT_VAS] = tm;
        CONNECTION_TO_MOTONEURONS[LEFT_SOL_Ib_STANCE][MUSCLES::LEFT_SOL] = tl;
        CONNECTION_TO_MOTONEURONS[RIGHT_TA_II_CYCLE][MUSCLES::RIGHT_TA] = tl;
        CONNECTION_TO_MOTONEURONS[RIGHT_SOL_RIGHT_TA_II_STANCE][MUSCLES::RIGHT_TA] = tl;
        CONNECTION_TO_MOTONEURONS[RIGHT_HF_II_SWING][MUSCLES::RIGHT_HF] = ts;
        CONNECTION_TO_MOTONEURONS[RIGHT_HAM_RIGHT_HF_II_SWING][MUSCLES::RIGHT_HF] = ts;
        CONNECTION_TO_MOTONEURONS[LEFT_TA_II_CYCLE][MUSCLES::LEFT_TA] = tl;
        CONNECTION_TO_MOTONEURONS[LEFT_SOL_LEFT_TA_II_STANCE][MUSCLES::LEFT_TA] = tl;
        CONNECTION_TO_MOTONEURONS[LEFT_HF_II_SWING][MUSCLES::LEFT_HF] = ts;
        CONNECTION_TO_MOTONEURONS[LEFT_HAM_LEFT_HF_II_SWING][MUSCLES::LEFT_HF] = ts;
        CONNECTION_TO_MOTONEURONS[RIGHT_VAS_ANGLE_ANGLE_OFFSET][MUSCLES::RIGHT_VAS] = tm;
        CONNECTION_TO_MOTONEURONS[LEFT_VAS_ANGLE_ANGLE_OFFSET][MUSCLES::LEFT_VAS] = tm;
        CONNECTION_TO_MOTONEURONS[RIGHT_VAS_LEFT_FOOT_STANCE_END][MUSCLES::RIGHT_VAS] = ts;
        CONNECTION_TO_MOTONEURONS[LEFT_VAS_RIGHT_FOOT_STANCE_END][MUSCLES::LEFT_VAS] = ts;
        CONNECTION_TO_MOTONEURONS[RIGHT_HAM_Ib_SWING][MUSCLES::RIGHT_HAM] = ts;
        CONNECTION_TO_MOTONEURONS[RIGHT_GLU_Ib_SWING][MUSCLES::RIGHT_GLU] = ts;
        CONNECTION_TO_MOTONEURONS[LEFT_HAM_Ib_SWING][MUSCLES::LEFT_HAM] = ts;
        CONNECTION_TO_MOTONEURONS[LEFT_GLU_Ib_SWING][MUSCLES::LEFT_GLU] = ts;
        
        
        int GATE_MAP[NUMBER];
        GATE_MAP[RIGHT_HAM_TRUNK_PD_STANCE] = ACTIVE_DURING::STANCE;
        GATE_MAP[RIGHT_HF_TRUNK_PD_STANCE] = ACTIVE_DURING::STANCE;
        GATE_MAP[RIGHT_GLU_TRUNK_PD2_STANCE] = ACTIVE_DURING::STANCE;
        GATE_MAP[RIGHT_HF_TRUNK_LEAK_SWING] = ACTIVE_DURING::SWING;
        GATE_MAP[LEFT_HAM_TRUNK_PD_STANCE] = ACTIVE_DURING::STANCE;
        GATE_MAP[LEFT_HF_TRUNK_PD_STANCE] = ACTIVE_DURING::STANCE;
        GATE_MAP[LEFT_GLU_TRUNK_PD2_STANCE] = ACTIVE_DURING::STANCE;
        GATE_MAP[LEFT_HF_TRUNK_LEAK_SWING] = ACTIVE_DURING::SWING;
        GATE_MAP[RIGHT_GAS_Ib_STANCE] = ACTIVE_DURING::STANCE;
        GATE_MAP[RIGHT_VAS_Ib_STANCE] = ACTIVE_DURING::STANCE;
        GATE_MAP[RIGHT_SOL_Ib_STANCE] = ACTIVE_DURING::STANCE;
        GATE_MAP[LEFT_GAS_Ib_STANCE] = ACTIVE_DURING::STANCE;
        GATE_MAP[LEFT_VAS_Ib_STANCE] = ACTIVE_DURING::STANCE;
        GATE_MAP[LEFT_SOL_Ib_STANCE] = ACTIVE_DURING::STANCE;
        GATE_MAP[RIGHT_TA_II_CYCLE] = ACTIVE_DURING::WHOLE_CYCLE;
        GATE_MAP[RIGHT_SOL_RIGHT_TA_II_STANCE] = ACTIVE_DURING::STANCE;
        GATE_MAP[RIGHT_HF_II_SWING] = ACTIVE_DURING::SWING;
        GATE_MAP[RIGHT_HAM_RIGHT_HF_II_SWING] = ACTIVE_DURING::SWING;
        GATE_MAP[LEFT_TA_II_CYCLE] = ACTIVE_DURING::WHOLE_CYCLE;
        GATE_MAP[LEFT_SOL_LEFT_TA_II_STANCE] = ACTIVE_DURING::STANCE;
        GATE_MAP[LEFT_HF_II_SWING] = ACTIVE_DURING::SWING;
        GATE_MAP[LEFT_HAM_LEFT_HF_II_SWING] = ACTIVE_DURING::SWING;
        GATE_MAP[RIGHT_VAS_ANGLE_ANGLE_OFFSET] = ACTIVE_DURING::ANGLE_OFFSET;
        GATE_MAP[LEFT_VAS_ANGLE_ANGLE_OFFSET] = ACTIVE_DURING::ANGLE_OFFSET;
        GATE_MAP[RIGHT_VAS_LEFT_FOOT_STANCE_END] = ACTIVE_DURING::STANCE_END;
        GATE_MAP[LEFT_VAS_RIGHT_FOOT_STANCE_END] = ACTIVE_DURING::STANCE_END;
        GATE_MAP[RIGHT_HAM_Ib_SWING] = ACTIVE_DURING::SWING;
        GATE_MAP[RIGHT_GLU_Ib_SWING] = ACTIVE_DURING::SWING;
        GATE_MAP[LEFT_HAM_Ib_SWING] = ACTIVE_DURING::SWING;
        GATE_MAP[LEFT_GLU_Ib_SWING] = ACTIVE_DURING::SWING;

        strs = {
        {SENSORY_INTERNEURON::RIGHT_SOL_Ib_STANCE,"right_sol__mff_stance"},
        {SENSORY_INTERNEURON::RIGHT_TA_II_CYCLE,"right_ta__mlf_cycle"},
        {SENSORY_INTERNEURON::RIGHT_SOL_RIGHT_TA_II_STANCE,"right_sol_ta_mff_stance"},
        {SENSORY_INTERNEURON::RIGHT_GAS_Ib_STANCE,"right_gas__mff_stance"},
        {SENSORY_INTERNEURON::RIGHT_VAS_Ib_STANCE,"right_vas__mff_stance"},
        {SENSORY_INTERNEURON::RIGHT_VAS_ANGLE_ANGLE_OFFSET,"right_vas_pko_angleoffset"},
        {SENSORY_INTERNEURON::RIGHT_VAS_LEFT_FOOT_STANCE_END,"right_vas_gcf_finishingstance"},
        {SENSORY_INTERNEURON::RIGHT_HAM_TRUNK_PD_STANCE,"right_ham_gif_stance"},
        {SENSORY_INTERNEURON::RIGHT_HAM_Ib_SWING,"right_ham__mff_swing"},
        {SENSORY_INTERNEURON::RIGHT_GLU_TRUNK_PD2_STANCE,"right_glu_gif_stance"},
        {SENSORY_INTERNEURON::RIGHT_GLU_Ib_SWING,"right_glu__mff_swing"},
        {SENSORY_INTERNEURON::RIGHT_HF_TRUNK_PD_STANCE,"right_hf_gif_stance"},
        {SENSORY_INTERNEURON::RIGHT_HF_II_SWING,"right_hf__mlf_swing"},
        {SENSORY_INTERNEURON::RIGHT_HAM_RIGHT_HF_II_SWING,"right_ham_hf_mlf_swing"},
        {SENSORY_INTERNEURON::RIGHT_HF_TRUNK_LEAK_SWING,"right_hf_tl_swing"},
        {SENSORY_INTERNEURON::LEFT_SOL_Ib_STANCE,"left_sol__mff_stance"},
        {SENSORY_INTERNEURON::LEFT_TA_II_CYCLE,"left_ta__mlf_cycle"},
        {SENSORY_INTERNEURON::LEFT_SOL_LEFT_TA_II_STANCE,"left_sol_ta_mff_stance"},
        {SENSORY_INTERNEURON::LEFT_GAS_Ib_STANCE,"left_gas__mff_stance"},
        {SENSORY_INTERNEURON::LEFT_VAS_Ib_STANCE,"left_vas__mff_stance"},
        {SENSORY_INTERNEURON::LEFT_VAS_ANGLE_ANGLE_OFFSET,"left_vas_pko_angleoffset"},
        {SENSORY_INTERNEURON::LEFT_VAS_RIGHT_FOOT_STANCE_END,"left_vas_gcf_finishingstance"},
        {SENSORY_INTERNEURON::LEFT_HAM_TRUNK_PD_STANCE,"left_ham_gif_stance"},
        {SENSORY_INTERNEURON::LEFT_HAM_Ib_SWING,"left_ham__mff_swing"},
        {SENSORY_INTERNEURON::LEFT_GLU_TRUNK_PD2_STANCE,"left_glu_gif_stance"},
        {SENSORY_INTERNEURON::LEFT_GLU_Ib_SWING,"left_glu__mff_swing"},
        {SENSORY_INTERNEURON::LEFT_HF_TRUNK_PD_STANCE,"left_hf_gif_stance"},
        {SENSORY_INTERNEURON::LEFT_HF_II_SWING,"left_hf__mlf_swing"},
        {SENSORY_INTERNEURON::LEFT_HAM_LEFT_HF_II_SWING,"left_ham_hf_mlf_swing"},
        {SENSORY_INTERNEURON::LEFT_HF_TRUNK_LEAK_SWING,"left_hf_tl_swing"}
        };
    };
    
    std::string toString(int id){
        if (id >= SENSORY_INTERNEURON::FIRST && id <= SENSORY_INTERNEURON::LAST)
            return strs[id];
        else
            return "sensory interneuron not found";
    }
}

namespace MUSCLES{
    bool IS_LEFT(int i){
        if (i <= LEFT_TA && i >= LEFT_HF)
            return true;
        else
            return false;
    }
    bool IS_RIGHT(int i){
        if (i <= RIGHT_TA && i >= RIGHT_HF)
            return true;
        else
            return false;
    }
    std::string toString(int id){
        string result;
        switch(id){
            case LEFT_HF:
                result="LEFT_HF";
                break;
            case LEFT_GLU:
                result="LEFT_GLU";
                break;
            case LEFT_VAS:
                result="LEFT_VAS";
                break;
            case LEFT_HAM:
                result="LEFT_HAM";
                break;
            case LEFT_GAS:
                result="LEFT_GAS";
                break;
            case LEFT_SOL:
                result="LEFT_SOL";
                break;
            case LEFT_TA:
                result="LEFT_TA";
                break;
            case RIGHT_HF:
                result="RIGHT_HF";
                break;
            case RIGHT_GLU:
                result="RIGHT_GLU";
                break;
            case RIGHT_VAS:
                result="RIGHT_VAS";
                break;
            case RIGHT_HAM:
                result="RIGHT_HAM";
                break;
            case RIGHT_GAS:
                result="RIGHT_GAS";
                break;
            case RIGHT_SOL:
                result="RIGHT_SOL";
                break;
            case RIGHT_TA:
                result="RIGHT_TA";
                break;
            default:
                result="not found";
        }
        return result;
    }
    int toID(string str){
        int result = -1;
        if(str=="LEFT_HF" || str=="left_hf")
            result = LEFT_HF;
        if(str=="LEFT_GLU" || str=="left_glu")
            result = LEFT_GLU;
        if(str=="LEFT_VAS" || str=="left_vas")
            result = LEFT_VAS;
        if(str=="LEFT_HAM" || str=="left_ham")
            result = LEFT_HAM;
        if(str=="LEFT_GAS" || str=="left_gas")
            result = LEFT_GAS;
        if(str=="LEFT_SOL" || str=="left_sol")
            result = LEFT_SOL;
        if(str=="LEFT_TA" || str=="left_ta")
            result = LEFT_TA;
        if(str=="RIGHT_HF" || str=="right_hf")
            result = RIGHT_HF;
        if(str=="RIGHT_GLU" || str=="right_glu")
            result = RIGHT_GLU;
        if(str=="RIGHT_VAS" || str=="right_vas")
            result = RIGHT_VAS;
        if(str=="RIGHT_HAM" || str=="right_ham")
            result = RIGHT_HAM;
        if(str=="RIGHT_GAS" || str=="right_gas")
            result = RIGHT_GAS;
        if(str=="RIGHT_SOL" || str=="right_sol")
            result = RIGHT_SOL;
        if(str=="RIGHT_TA" || str=="right_ta")
            result = RIGHT_TA;
        return result;
    }
}

namespace MUSCLES_MOUSE{
    bool IS_LEFT(int i){
        if (i <= LEFT_LG && i >= LEFT_PMA)
            return true;
        else
            return false;
    }
    bool IS_RIGHT(int i){
        if (i <= RIGHT_LG && i >= RIGHT_PMA)
            return true;
        else
            return false;
    }
    std::string toString(int id){
        string result;
        switch(id){
            case LEFT_PMA:
                result="LEFT_PMA";
                break;
            case LEFT_CF:
                result="LEFT_CF";
                break;
            case LEFT_SM:
                result="LEFT_SM";
                break;
            case LEFT_POP:
                result="LEFT_POP";
                break;
            case LEFT_RF:
                result="LEFT_RF";
                break;
            case LEFT_TA:
                result="LEFT_TA";
                break;
            case LEFT_SOL:
                result="LEFT_SOL";
                break;
            case LEFT_LG:
                result="LEFT_LG";
                break;
            case RIGHT_PMA:
                result="RIGHT_PMA";
                break;
            case RIGHT_CF:
                result="RIGHT_CF";
                break;
            case RIGHT_SM:
                result="RIGHT_SM";
                break;
            case RIGHT_POP:
                result="RIGHT_POP";
                break;
            case RIGHT_RF:
                result="RIGHT_RF";
                break;
            case RIGHT_TA:
                result="RIGHT_TA";
                break;
            case RIGHT_SOL:
                result="RIGHT_SOL";
                break;
            case RIGHT_LG:
                result="RIGHT_LG";
                break;
            default:
                result="not found";
        }
        return result;
    }
    int toID(string str){
        int result = -1;
        if(str=="LEFT_PMA" || str=="left_pma")
            result = LEFT_PMA;
        if(str=="LEFT_CF" || str=="left_cf")
            result = LEFT_CF;
        if(str=="LEFT_SM" || str=="left_sm")
            result = LEFT_SM;
        if(str=="LEFT_POP" || str=="left_pop")
            result = LEFT_POP;
        if(str=="LEFT_RF" || str=="left_rf")
            result = LEFT_RF;
        if(str=="LEFT_TA" || str=="left_ta")
            result = LEFT_TA;
        if(str=="LEFT_SOL" || str=="left_sol")
            result = LEFT_SOL;
        if(str=="LEFT_LG" || str=="left_lg")
            result = LEFT_LG;

        if(str=="RIGHT_PMA" || str=="right_pma")
            result = RIGHT_PMA;
        if(str=="RIGHT_CF" || str=="right_cf")
            result = RIGHT_CF;
        if(str=="RIGHT_SM" || str=="right_sm")
            result = RIGHT_SM;
        if(str=="RIGHT_POP" || str=="right_pop")
            result = RIGHT_POP;
        if(str=="RIGHT_RF" || str=="right_rf")
            result = RIGHT_RF;
        if(str=="RIGHT_TA" || str=="right_ta")
            result = RIGHT_TA;
        if(str=="RIGHT_SOL" || str=="right_sol")
            result = RIGHT_SOL;
        if(str=="RIGHT_LG" || str=="right_lg")
            result = RIGHT_LG;
        return result;
    }
}