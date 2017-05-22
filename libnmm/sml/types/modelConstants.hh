#ifndef __AIRI_TYPES_MODEL_CONSTANTS_H__
#define __AIRI_TYPES_MODEL_CONSTANTS_H__

namespace HUMAN{

    namespace CONSTANT{
        typedef enum
        {
            /*****************
               Segment constants
             *****************/
            LENGTH_TRUNK,
            LENGTH_THIGH,
            LENGTH_SHIN,
            LENGTH_ANKLE,
            LENGTH_FOOT,
            WEIGHT_TRUNK,
            WEIGHT_THIGH,
            WEIGHT_SHIN,
            WEIGHT_ANKLE,
            WEIGHT_FOOT,
            MODEL_WEIGHT,
            MODEL_HEIGHT,
            /*****************
               Joint constants 
             *****************/
            REF_POS_HIP_LEFT,
            REF_POS_KNEE_LEFT,
            REF_POS_ANKLE_LEFT,
            REF_POS_HIP_RIGHT,
            REF_POS_KNEE_RIGHT,
            REF_POS_ANKLE_RIGHT,
            MAX_POS_HIP_LEFT,
            MAX_POS_KNEE_LEFT,
            MAX_POS_ANKLE_LEFT,
            MAX_POS_HIP_RIGHT,
            MAX_POS_KNEE_RIGHT,
            MAX_POS_ANKLE_RIGHT,
            MIN_POS_HIP_LEFT,
            MIN_POS_KNEE_LEFT,
            MIN_POS_ANKLE_LEFT,
            MIN_POS_HIP_RIGHT,
            MIN_POS_KNEE_RIGHT,
            MIN_POS_ANKLE_RIGHT,
            REF_POS_HIPCOR_LEFT,
            REF_POS_HIPCOR_RIGHT,
            MAX_POS_HIPCOR_LEFT,
            MAX_POS_HIPCOR_RIGHT,
            MIN_POS_HIPCOR_LEFT,
            MIN_POS_HIPCOR_RIGHT,
            /*****************
               Muscle constants 
             *****************/
            // Mono-articular muscle, variables order R,PHI_MAX,PHI_REF,L_OPT,L_SLACK,V_MAX,MASS,PENNATION PERCENTAGE, TYPE1FIBER PERCENTAGE, DIRECTION
            R_SOL,
            R_TA,
            R_VAS,
            R_GLU,
            R_HF,
            R_HAD,
            R_HAB,
            MAX_PHI_SOL,
            MAX_PHI_TA,
            MAX_PHI_VAS,
            MAX_PHI_GLU,
            MAX_PHI_HF,
            MAX_PHI_HAD,
            MAX_PHI_HAB,
            REF_PHI_SOL,
            REF_PHI_TA,
            REF_PHI_VAS,
            REF_PHI_GLU,
            REF_PHI_HF,
            REF_PHI_HAD,
            REF_PHI_HAB,
            OPT_L_SOL,
            OPT_L_TA,
            OPT_L_VAS,
            OPT_L_GLU,
            OPT_L_HF,
            OPT_L_HAD,
            OPT_L_HAB,
            SLACK_L_SOL,
            SLACK_L_TA,
            SLACK_L_VAS,
            SLACK_L_GLU,
            SLACK_L_HF,
            SLACK_L_HAD,
            SLACK_L_HAB,
            MAX_V_SOL,
            MAX_V_TA,
            MAX_V_VAS,
            MAX_V_GLU,
            MAX_V_HF,
            MAX_V_HAD,
            MAX_V_HAB,
            MASS_SOL,
            MASS_TA,
            MASS_VAS,
            MASS_GLU,
            MASS_HF,
            MASS_HAD,
            MASS_HAB,
            PENNATION_SOL,
            PENNATION_TA,
            PENNATION_VAS,
            PENNATION_GLU,
            PENNATION_HF,
            PENNATION_HAD,
            PENNATION_HAB,
            TYPE1FIBER_SOL,
            TYPE1FIBER_TA,
            TYPE1FIBER_VAS,
            TYPE1FIBER_GLU,
            TYPE1FIBER_HF,
            TYPE1FIBER_HAD,
            TYPE1FIBER_HAB,
            DIRECTION_SOL,
            DIRECTION_TA,
            DIRECTION_VAS,
            DIRECTION_GLU,
            DIRECTION_HF,
            DIRECTION_HAD,
            DIRECTION_HAB,
            // bi-articular muscle, variables order R,PHI_MAX,PHI_REF,L_OPT,L_SLACK,V_MAX,MASS,PENNATION PERCENTAGE, TYPE1FIBER PERCENTAGE, DIRECTION
            // R,PHI_MAX, PHI_REF have two variable one for each joint on which it acts (order : distal-proximal)
            
    
    
            R_GLU_KNEE,
            R_GAS_ANKLE,
            R_GAS_KNEE,
            R_HAM_KNEE,
            R_HAM_HIP,
            MAX_PHI_GLU_KNEE,
            MAX_PHI_GAS_ANKLE,
            MAX_PHI_GAS_KNEE,
            MAX_PHI_HAM_KNEE,
            MAX_PHI_HAM_HIP,
            REF_PHI_GLU_KNEE,
            REF_PHI_GAS_ANKLE,
            REF_PHI_GAS_KNEE,
            REF_PHI_HAM_KNEE,
            REF_PHI_HAM_HIP,
            OPT_L_GAS,
            OPT_L_HAM,
            SLACK_L_GAS,
            SLACK_L_HAM,
            MAX_V_GAS,
            MAX_V_HAM,
            MASS_GAS,
            MASS_HAM,
            PENNATION_GAS,
            PENNATION_HAM,
            TYPE1FIBER_GAS,
            TYPE1FIBER_HAM,
            DIRECTION_GAS,
            DIRECTION_HAM,
            FIRST = LENGTH_TRUNK,
            LAST = DIRECTION_HAM,
        } Constants;
        const int NUMBER = CONSTANT::LAST - CONSTANT::FIRST + 1;
        
        const unsigned FIRST_REF_POS_JOINT = REF_POS_HIP_LEFT;
        const unsigned LAST_REF_POS_JOINT = REF_POS_ANKLE_RIGHT;
        const unsigned FIRST_MIN_POS_JOINT = MIN_POS_HIP_LEFT;
        const unsigned LAST_MIN_POS_JOINT = MIN_POS_ANKLE_RIGHT;
        const unsigned FIRST_MAX_POS_JOINT = MAX_POS_HIP_LEFT;
        const unsigned LAST_MAX_POS_JOINT = MAX_POS_ANKLE_RIGHT;
        
        const unsigned FIRST_R_MUSCLE_T1 = R_SOL;
        const unsigned LAST_R_MUSCLE_T1 = R_HF;
        const unsigned FIRST_MAX_PHI_MUSCLE_T1 = MAX_PHI_SOL;
        const unsigned LAST_MAX_PHI_MUSCLE_T1 = MAX_PHI_HF;
        const unsigned FIRST_REF_PHI_MUSCLE_T1 = REF_PHI_SOL;
        const unsigned LAST_REF_PHI_MUSCLE_T1 = REF_PHI_HF;
        const unsigned FIRST_OPT_L_MUSCLE_T1 = OPT_L_SOL;
        const unsigned LAST_OPT_L_MUSCLE_T1 = OPT_L_HF;
        const unsigned FIRST_SLACK_L_MUSCLE_T1 = SLACK_L_SOL;
        const unsigned LAST_SLACK_L_MUSCLE_T1 = SLACK_L_HF;
        const unsigned FIRST_MAX_V_MUSCLE_T1 = MAX_V_SOL;
        const unsigned LAST_MAX_V_MUSCLE_T1 = MAX_V_HF;
        const unsigned FIRST_MASS_MUSCLE_T1 = MASS_SOL;
        const unsigned LAST_MASS_MUSCLE_T1 = MASS_HF;
        const unsigned FIRST_PENNATION_MUSCLE_T1 = PENNATION_SOL;
        const unsigned LAST_PENNATION_MUSCLE_T1 = PENNATION_HF;
        const unsigned FIRST_TYPE1FIBER_MUSCLE_T1 = TYPE1FIBER_SOL;
        const unsigned LAST_TYPE1FIBER_MUSCLE_T1 = TYPE1FIBER_HF;
        const unsigned FIRST_DIRECTION_MUSCLE_T1 = DIRECTION_SOL;
        const unsigned LAST_DIRECTION_MUSCLE_T1 = DIRECTION_HF;
        
        const unsigned FIRST_R_MUSCLE_T2 = R_GAS_ANKLE;
        const unsigned LAST_R_MUSCLE_T2 = R_HAM_HIP;
        const unsigned FIRST_MAX_PHI_MUSCLE_T2 = MAX_PHI_GAS_ANKLE;
        const unsigned LAST_MAX_PHI_MUSCLE_T2 = MAX_PHI_HAM_HIP;
        const unsigned FIRST_REF_PHI_MUSCLE_T2 = REF_PHI_GAS_ANKLE;
        const unsigned LAST_REF_PHI_MUSCLE_T2 = REF_PHI_HAM_HIP;
        const unsigned FIRST_OPT_L_MUSCLE_T2 = OPT_L_GAS;
        const unsigned LAST_OPT_L_MUSCLE_T2 = OPT_L_HAM;
        const unsigned FIRST_SLACK_L_MUSCLE_T2 = SLACK_L_GAS;
        const unsigned LAST_SLACK_L_MUSCLE_T2 = SLACK_L_HAM;
        const unsigned FIRST_MAX_V_MUSCLE_T2 = MAX_V_GAS;
        const unsigned LAST_MAX_V_MUSCLE_T2 = MAX_V_HAM;
        const unsigned FIRST_MASS_MUSCLE_T2 = MASS_GAS;
        const unsigned LAST_MASS_MUSCLE_T2 = MASS_HAM;
        const unsigned FIRST_PENNATION_MUSCLE_T2 = PENNATION_GAS;
        const unsigned LAST_PENNATION_MUSCLE_T2 = PENNATION_HAM;
        const unsigned FIRST_TYPE1FIBER_MUSCLE_T2 = TYPE1FIBER_GAS;
        const unsigned LAST_TYPE1FIBER_MUSCLE_T2 = TYPE1FIBER_HAM;
        const unsigned FIRST_DIRECTION_MUSCLE_T2 = DIRECTION_GAS;
        const unsigned LAST_DIRECTION_MUSCLE_T2 = DIRECTION_HAM;

    }



}

namespace MOUSE{
namespace CONSTANT{
    typedef enum
    {
        /*****************
           Segment constants
         *****************/
        LENGTH_THIGH,
        LENGTH_SHIN,
        LENGTH_FOOT,
        WEIGHT_TRUNK,
        WEIGHT_THIGH,
        WEIGHT_SHIN,
        WEIGHT_FOOT,
        MODEL_WEIGHT,
        MODEL_HEIGHT,
        /*****************
           Joint constants 
         *****************/
        REF_POS_HIP_LEFT,
        REF_POS_KNEE_LEFT,
        REF_POS_ANKLE_LEFT,
        REF_POS_HIP_RIGHT,
        REF_POS_KNEE_RIGHT,
        REF_POS_ANKLE_RIGHT,
        MAX_POS_HIP_LEFT,
        MAX_POS_KNEE_LEFT,
        MAX_POS_ANKLE_LEFT,
        MAX_POS_HIP_RIGHT,
        MAX_POS_KNEE_RIGHT,
        MAX_POS_ANKLE_RIGHT,
        MIN_POS_HIP_LEFT,
        MIN_POS_KNEE_LEFT,
        MIN_POS_ANKLE_LEFT,
        MIN_POS_HIP_RIGHT,
        MIN_POS_KNEE_RIGHT,
        MIN_POS_ANKLE_RIGHT,
        REF_POS_HIPCOR_LEFT,
        REF_POS_HIPCOR_RIGHT,
        MAX_POS_HIPCOR_LEFT,
        MAX_POS_HIPCOR_RIGHT,
        MIN_POS_HIPCOR_LEFT,
        MIN_POS_HIPCOR_RIGHT,
        /*****************
           Muscle constants 
         *****************/
        // Mono-articular muscle, variables order R,PHI_MAX,PHI_REF,L_OPT,L_SLACK,V_MAX,MASS,PENNATION PERCENTAGE, TYPE1FIBER PERCENTAGE, DIRECTION

        R_PMA,
        R_CF,
        R_POP,
        R_RF,
        R_TA,
        R_SOL,
        MAX_PHI_PMA,
        MAX_PHI_CF,
        MAX_PHI_POP,
        MAX_PHI_RF,
        MAX_PHI_TA,
        MAX_PHI_SOL,
        REF_PHI_PMA,
        REF_PHI_CF,
        REF_PHI_POP,
        REF_PHI_RF,
        REF_PHI_TA,
        REF_PHI_SOL,    
        OPT_L_PMA,
        OPT_L_CF,
        OPT_L_POP,
        OPT_L_RF,
        OPT_L__TA,
        OPT_L__SOL,
        SLACK_L_PMA,
        SLACK_L_CF,
        SLACK_L_POP,
        SLACK_L_RF,
        SLACK_L_TA,
        SLACK_L_SOL,
        MAX_V_PMA,
        MAX_V_CF,
        MAX_V_POP,
        MAX_V_RF,
        MAX_V__TA,
        MAX_V__SOL,
        MASS_PMA,
        MASS_CF,
        MASS_POP,
        MASS_RF,
        MASS_TA,
        MASS_SOL,
        PENNATION_PMA,
        PENNATION_CF,
        PENNATION_POP,
        PENNATION_RF,
        PENNATION_TA,
        PENNATION_SOL,
        TYPE1FIBER_PMA,
        TYPE1FIBER_CF,
        TYPE1FIBER_POP,
        TYPE1FIBER_RF,
        TYPE1FIBER_TA,
        TYPE1FIBER_SOL,
        DIRECTION_PMA,
        DIRECTION_CF,
        DIRECTION_POP,
        DIRECTION_RF,
        DIRECTION_TA,
        DIRECTION_SOL,        
        // bi-articular muscle, variables order R,PHI_MAX,PHI_REF,L_OPT,L_SLACK,V_MAX,MASS,PENNATION PERCENTAGE, TYPE1FIBER PERCENTAGE, DIRECTION
        // R,PHI_MAX, PHI_REF have two variable one for each joint on which it acts (order : distal-proximal)        


        R_SM_KNEE,
        R_SM_HIP,
        R_LG_ANKLE,
        R_LG_KNEE,
        MAX_PHI_SM_KNEE,
        MAX_PHI_SM_HIP,
        MAX_PHI_LG_ANKLE,
        MAX_PHI_LG_KNEE,
        REF_PHI_SM_KNEE,
        REF_PHI_SM_HIP,
        REF_PHI_LG_ANKLE,
        REF_PHI_LG_KNEE,        
        OPT_L_SM,
        OPT_L_LG,
        SLACK_L_SM,
        SLACK_L_LG,
        MAX_V_SM,
        MAX_V_LG,
        MASS_SM,
        MASS_LG,
        PENNATION_SM,
        PENNATION_LG,
        DIRECTION_SM,
        DIRECTION_LG,
        FIRST = LENGTH_THIGH,
        LAST = DIRECTION_LG,
    } ConstantsMouse;

    const int NUMBER = CONSTANT::LAST - CONSTANT::FIRST + 1;
    
    const unsigned FIRST_REF_POS_JOINT = REF_POS_HIP_LEFT;
    const unsigned LAST_REF_POS_JOINT = REF_POS_ANKLE_RIGHT;
    const unsigned FIRST_MIN_POS_JOINT = MIN_POS_HIP_LEFT;
    const unsigned LAST_MIN_POS_JOINT = MIN_POS_ANKLE_RIGHT;
    const unsigned FIRST_MAX_POS_JOINT = MAX_POS_HIP_LEFT;
    const unsigned LAST_MAX_POS_JOINT = MAX_POS_ANKLE_RIGHT;
    
    // const unsigned FIRST_R_MUSCLE_T1 = R_SOL;
    // const unsigned LAST_R_MUSCLE_T1 = R_HF;
    // const unsigned FIRST_MAX_PHI_MUSCLE_T1 = MAX_PHI_SOL;
    // const unsigned LAST_MAX_PHI_MUSCLE_T1 = MAX_PHI_HF;
    // const unsigned FIRST_REF_PHI_MUSCLE_T1 = REF_PHI_SOL;
    // const unsigned LAST_REF_PHI_MUSCLE_T1 = REF_PHI_HF;
    // const unsigned FIRST_OPT_L_MUSCLE_T1 = OPT_L_SOL;
    // const unsigned LAST_OPT_L_MUSCLE_T1 = OPT_L_HF;
    // const unsigned FIRST_SLACK_L_MUSCLE_T1 = SLACK_L_SOL;
    // const unsigned LAST_SLACK_L_MUSCLE_T1 = SLACK_L_HF;
    // const unsigned FIRST_MAX_V_MUSCLE_T1 = MAX_V_SOL;
    // const unsigned LAST_MAX_V_MUSCLE_T1 = MAX_V_HF;
    // const unsigned FIRST_MASS_MUSCLE_T1 = MASS_SOL;
    // const unsigned LAST_MASS_MUSCLE_T1 = MASS_HF;
    // const unsigned FIRST_PENNATION_MUSCLE_T1 = PENNATION_SOL;
    // const unsigned LAST_PENNATION_MUSCLE_T1 = PENNATION_HF;
    // const unsigned FIRST_TYPE1FIBER_MUSCLE_T1 = TYPE1FIBER_SOL;
    // const unsigned LAST_TYPE1FIBER_MUSCLE_T1 = TYPE1FIBER_HF;
    // const unsigned FIRST_DIRECTION_MUSCLE_T1 = DIRECTION_SOL;
    // const unsigned LAST_DIRECTION_MUSCLE_T1 = DIRECTION_HF;
    
    // const unsigned FIRST_R_MUSCLE_T2 = R_GAS_ANKLE;
    // const unsigned LAST_R_MUSCLE_T2 = R_HAM_HIP;
    // const unsigned FIRST_MAX_PHI_MUSCLE_T2 = MAX_PHI_GAS_ANKLE;
    // const unsigned LAST_MAX_PHI_MUSCLE_T2 = MAX_PHI_HAM_HIP;
    // const unsigned FIRST_REF_PHI_MUSCLE_T2 = REF_PHI_GAS_ANKLE;
    // const unsigned LAST_REF_PHI_MUSCLE_T2 = REF_PHI_HAM_HIP;
    // const unsigned FIRST_OPT_L_MUSCLE_T2 = OPT_L_GAS;
    // const unsigned LAST_OPT_L_MUSCLE_T2 = OPT_L_HAM;
    // const unsigned FIRST_SLACK_L_MUSCLE_T2 = SLACK_L_GAS;
    // const unsigned LAST_SLACK_L_MUSCLE_T2 = SLACK_L_HAM;
    // const unsigned FIRST_MAX_V_MUSCLE_T2 = MAX_V_GAS;
    // const unsigned LAST_MAX_V_MUSCLE_T2 = MAX_V_HAM;
    // const unsigned FIRST_MASS_MUSCLE_T2 = MASS_GAS;
    // const unsigned LAST_MASS_MUSCLE_T2 = MASS_HAM;
    // const unsigned FIRST_PENNATION_MUSCLE_T2 = PENNATION_GAS;
    // const unsigned LAST_PENNATION_MUSCLE_T2 = PENNATION_HAM;
    // const unsigned FIRST_TYPE1FIBER_MUSCLE_T2 = TYPE1FIBER_GAS;
    // const unsigned LAST_TYPE1FIBER_MUSCLE_T2 = TYPE1FIBER_HAM;
    // const unsigned FIRST_DIRECTION_MUSCLE_T2 = DIRECTION_GAS;
    // const unsigned LAST_DIRECTION_MUSCLE_T2 = DIRECTION_HAM;

}
}

#endif