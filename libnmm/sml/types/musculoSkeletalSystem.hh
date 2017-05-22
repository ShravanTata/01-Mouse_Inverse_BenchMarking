#ifndef __AIRI_TYPES_MUSCLES_H__
#define __AIRI_TYPES_MUSCLES_H__

namespace MUSCLES{
    typedef enum
    {
        LEFT_HF,
        LEFT_GLU,
        //LEFT_GLU,
        //LEFT_HF,
        LEFT_VAS,
        LEFT_HAM,
        LEFT_GAS,
        LEFT_SOL,
        LEFT_TA,
        RIGHT_HF,
        RIGHT_GLU,
        RIGHT_VAS,
        RIGHT_HAM,
        RIGHT_GAS,
        RIGHT_SOL,
        RIGHT_TA,
        LAST = RIGHT_TA,
        FIRST = LEFT_HF,
    } Muscles;
    const int NUMBER = LAST - FIRST + 1;
    int toID(std::string str);
    bool IS_RIGHT(int i);
    bool IS_LEFT(int i);
    std::string toString(int id);
    /*
    std::map<MUSCLES::Muscles,std::string> Names ={
        { MUSCLES::LEFT_HF, "LEFT_HF" },
        { MUSCLES::LEFT_GLU, "LEFT_GLU" },
        { MUSCLES::LEFT_VAS, "LEFT_VAS" },
        { MUSCLES::LEFT_HAM, "LEFT_HAM" },
        { MUSCLES::LEFT_GAS, "LEFT_GAS" },
        { MUSCLES::LEFT_SOL, "LEFT_SOL" },
        { MUSCLES::LEFT_TA, "LEFT_TA" },
        { MUSCLES::RIGHT_HF, "RIGHT_HF" },
        { MUSCLES::RIGHT_GLU, "RIGHT_GLU" },
        { MUSCLES::RIGHT_VAS, "RIGHT_VAS" },
        { MUSCLES::RIGHT_HAM, "RIGHT_HAM" },
        { MUSCLES::RIGHT_GAS, "RIGHT_GAS" },
        { MUSCLES::RIGHT_SOL, "RIGHT_SOL" },
        { MUSCLES::RIGHT_TA, "RIGHT_TA" },
    };*/

}


namespace MUSCLES_MOUSE{
    typedef enum
    {
        LEFT_PMA,
        LEFT_CF,
        LEFT_SM,
        LEFT_POP,
        LEFT_RF,
        LEFT_TA,
        LEFT_SOL,
        LEFT_LG,
        RIGHT_PMA,
        RIGHT_CF,
        RIGHT_SM,
        RIGHT_POP,
        RIGHT_RF,
        RIGHT_TA,
        RIGHT_SOL,
        RIGHT_LG,
        LAST = RIGHT_LG,
        FIRST = LEFT_PMA,
    } MusclesMouse;
    const int NUMBER = LAST - FIRST + 1;
    int toID(std::string str);
    bool IS_RIGHT(int i);
    bool IS_LEFT(int i);
    std::string toString(int id);
    /*
    std::map<MUSCLES::Muscles,std::string> Names ={
        { MUSCLES::LEFT_HF, "LEFT_HF" },
        { MUSCLES::LEFT_GLU, "LEFT_GLU" },
        { MUSCLES::LEFT_VAS, "LEFT_VAS" },
        { MUSCLES::LEFT_HAM, "LEFT_HAM" },
        { MUSCLES::LEFT_GAS, "LEFT_GAS" },
        { MUSCLES::LEFT_SOL, "LEFT_SOL" },
        { MUSCLES::LEFT_TA, "LEFT_TA" },
        { MUSCLES::RIGHT_HF, "RIGHT_HF" },
        { MUSCLES::RIGHT_GLU, "RIGHT_GLU" },
        { MUSCLES::RIGHT_VAS, "RIGHT_VAS" },
        { MUSCLES::RIGHT_HAM, "RIGHT_HAM" },
        { MUSCLES::RIGHT_GAS, "RIGHT_GAS" },
        { MUSCLES::RIGHT_SOL, "RIGHT_SOL" },
        { MUSCLES::RIGHT_TA, "RIGHT_TA" },
    };*/

}

namespace SENSORS{
    typedef enum
    {
        LEFT_ILI_Ib, // Golgi tendon
        LEFT_ST_Ib,
        LEFT_VAS_Ib,
        LEFT_HAM_Ib,
        LEFT_GAS_Ib,
        LEFT_SOL_Ib,
        LEFT_TA_Ib,
        RIGHT_ILI_Ib,
        RIGHT_ST_Ib,
        RIGHT_VAS_Ib,
        RIGHT_HAM_Ib,
        RIGHT_GAS_Ib,
        RIGHT_SOL_Ib,
        RIGHT_TA_Ib,
        LEFT_ILI_II, // Secondary muscle spindles
        LEFT_ST_II,
        LEFT_VAS_II,
        LEFT_HAM_II,
        LEFT_GAS_II,
        LEFT_SOL_II,
        LEFT_TA_II,
        RIGHT_ILI_II,
        RIGHT_ST_II,
        RIGHT_VAS_II,
        RIGHT_HAM_II,
        RIGHT_GAS_II,
        RIGHT_SOL_II,
        RIGHT_TA_II,
        LEFT_FOOT, // Foot ground sensors
        RIGHT_FOOT,
        LEFT_KNEE_ANGLE, // Angle overextension sensors
        RIGHT_KNEE_ANGLE,
        TRUNK_PD, // Trunk sensors
        TRUNK_PD2,
        TRUNK_LEAK,
        FIRST = LEFT_ILI_Ib,
        LAST = TRUNK_LEAK
    } Sensor;
}

namespace SENSORY_INTERNEURON{
    typedef enum
    {
        // the sensory interneurons name format is as follow
        // First element is the output muscle
        // Second is the sensors name (there can be several sensors combined)
        // Third is one this interneuron should be active
        
        // TRUNK RELATED INTERNEURONS
        RIGHT_HAM_TRUNK_PD_STANCE, //HAM_GIF_STANCE_RIGHT,
        RIGHT_HF_TRUNK_PD_STANCE, //HF_GIF_STANCE_RIGHT,
        RIGHT_GLU_TRUNK_PD2_STANCE, //GLU_GIF_STANCE_RIGHT,
        RIGHT_HF_TRUNK_LEAK_SWING, //HF_TL_SWING_RIGHT,
        LEFT_HAM_TRUNK_PD_STANCE, //HAM_GIF_STANCE_RIGHT,
        LEFT_HF_TRUNK_PD_STANCE, //HF_GIF_STANCE_RIGHT,
        LEFT_GLU_TRUNK_PD2_STANCE, //GLU_GIF_STANCE_RIGHT,
        LEFT_HF_TRUNK_LEAK_SWING, //HF_TL_SWING_RIGHT,
        // MUSCLE SENSORS
        RIGHT_GAS_Ib_STANCE, //GAS__MFF_STANCE_RIGHT,
        RIGHT_VAS_Ib_STANCE, //VAS__MFF_STANCE_RIGHT,
        RIGHT_SOL_Ib_STANCE, //SOL__MFF_STANCE_RIGHT,
        LEFT_GAS_Ib_STANCE, //GAS__MFF_STANCE_RIGHT,
        LEFT_VAS_Ib_STANCE, //VAS__MFF_STANCE_RIGHT,
        LEFT_SOL_Ib_STANCE, //SOL__MFF_STANCE_RIGHT,

        RIGHT_TA_II_CYCLE, //TA__MLF_CYCLE_RIGHT,
        RIGHT_SOL_RIGHT_TA_II_STANCE, //SOL_TA_MFF_STANCE_RIGHT,
        RIGHT_HF_II_SWING, //HF__MLF_SWING_RIGHT,
        RIGHT_HAM_RIGHT_HF_II_SWING, //HAM_HF_MLF_SWING_RIGHT,
        LEFT_TA_II_CYCLE, //TA__MLF_CYCLE_RIGHT,
        LEFT_SOL_LEFT_TA_II_STANCE, //SOL_TA_MFF_STANCE_RIGHT,
        LEFT_HF_II_SWING, //HF__MLF_SWING_RIGHT,
        LEFT_HAM_LEFT_HF_II_SWING, //HAM_HF_MLF_SWING_RIGHT,

        RIGHT_VAS_ANGLE_ANGLE_OFFSET, //VAS_PKO_ANGLEOFFSET_RIGHT,
        LEFT_VAS_ANGLE_ANGLE_OFFSET, //VAS_PKO_ANGLEOFFSET_RIGHT,
        
        RIGHT_VAS_LEFT_FOOT_STANCE_END, // VAS_GCF_FINISHINGSTANCE_RIGHT,
        LEFT_VAS_RIGHT_FOOT_STANCE_END, // VAS_GCF_FINISHINGSTANCE_RIGHT,

        RIGHT_HAM_Ib_SWING, //HAM__MFF_SWING_RIGHT,
        RIGHT_GLU_Ib_SWING, //GLU__MFF_SWING_RIGHT,
        LEFT_HAM_Ib_SWING, //HAM__MFF_SWING_RIGHT,
        LEFT_GLU_Ib_SWING, //GLU__MFF_SWING_RIGHT,

        
        FIRST = RIGHT_HAM_TRUNK_PD_STANCE,
        LAST = LEFT_GLU_Ib_SWING,
    } SensoryInterneurons;
    const int NUMBER = LAST - FIRST + 1;
    extern int  GATE_MAP[NUMBER];
    //extern bool CONNECTION_TO_MOTONEURONS[NUMBER][MUSCLES::NUMBER];
    extern int CONNECTION_TO_MOTONEURONS[NUMBER][MUSCLES::NUMBER];
    extern std::map<int,std::string> strs;
    
    void init();
    std::string toString(int id);
}

#endif