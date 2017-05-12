/*

This file is created as interface with Webots and libnmm package.
Author : STata
Data : 21-11-2016

*/
#include <iostream>
#include <limits>
#include "hindLimb.hh"
#include <fstream>
#include <math.h>

using namespace std;
using namespace webots;


Muscle::Muscle() {


// Get the joints and position sensors
    joints[0] = getMotor("HIP_RIGHT");
    joints[1] = getMotor("KNEE_RIGHT");
    joints[2] = getMotor("ANKLE_RIGHT");

    joints[3] = getMotor("HIP_LEFT");
    joints[4] = getMotor("KNEE_LEFT");
    joints[5] = getMotor("ANKLE_LEFT");

    joints[6] = getMotor("FORE_U_RIGHT");
    joints[7] = getMotor("FORE_L_RIGHT");
    joints[8] = getMotor("FOREPAW_RIGHT");

    joints[9] = getMotor("FORE_U_LEFT");
    joints[10] = getMotor("FORE_L_LEFT");
    joints[11] = getMotor("FOREPAW_LEFT");
    
    joints_pos[0] = getPositionSensor("HIP_RIGHT_POS");
    joints_pos[1] = getPositionSensor("KNEE_RIGHT_POS");
    joints_pos[2] = getPositionSensor("ANKLE_RIGHT_POS");

    joints_pos[3] = getPositionSensor("HIP_LEFT_POS");
    joints_pos[4] = getPositionSensor("KNEE_LEFT_POS");
    joints_pos[5] = getPositionSensor("ANKLE_LEFT_POS");

    joints_pos[6] = getPositionSensor("FORE_U_RIGHT_POS");
    joints_pos[7] = getPositionSensor("FORE_L_RIGHT_POS");
    joints_pos[8] = getPositionSensor("FOREPAW_RIGHT_POS");

    joints_pos[9] = getPositionSensor("FORE_U_LEFT_POS");
    joints_pos[10] = getPositionSensor("FORE_L_LEFT_POS");
    joints_pos[11] = getPositionSensor("FOREPAW_LEFT_POS");


    // ENABLE POSITION SENSOR
    joints_pos[0]->enable(TIME_STEP);
    
    joints_pos[1]->enable(TIME_STEP);

    joints_pos[2]->enable(TIME_STEP);

    joints_pos[3]->enable(TIME_STEP);

    joints_pos[4]->enable(TIME_STEP);

    joints_pos[5]->enable(TIME_STEP);

    joints_pos[6]->enable(TIME_STEP);

    joints_pos[7]->enable(TIME_STEP);

    joints_pos[8]->enable(TIME_STEP);

    joints_pos[9]->enable(TIME_STEP);

    joints_pos[10]->enable(TIME_STEP);

    joints_pos[11]->enable(TIME_STEP);



// TO Save Data
    save_data.open("../../Results/hindLimbComplete_test.txt");

    if (save_data.is_open())
    {
        cout << "File Successfully opened for writing" << endl;
     }
     else
     {
        cerr << "Unable to open file "  << endl;
     }


// Header
    save_data << "Time" << "\t" << "HIP_RIGHT"
    << "\t" << "KNEE_RIGHT"
    << "\t" << "ANKLE_RIGHT"
    << "\t" << "HIP_LEFT"
    << "\t" << "KNEE_LEFT"
    << "\t" << "ANKLE_LEFT" << "\n" ;
  
    if (inverse == 1)
    {
        cout << "Running inverse simulation" << endl;
    }
    else
    {
        cout << "Running forward simulation" << endl;
    }


    if (inverse == 1)
    {
        joint_data_file.open("../../Data/KinematicData/RAT_TREADMILL.txt");

        if (joint_data_file.is_open())
        {
            cout << "File Successfully read" << endl;
        }
        else
        {
            cerr << "Enable to read file : Terminating program" << endl;
            cerr << "Check if the file exists in the path. " <<  endl;
        }
        

    }


}

Muscle::~Muscle() {
    if(inverse == 1)
    {
      joint_data_file.close();
    }
    else
    {
      save_data.close();
    }
}

void Muscle::MuscleSimulate() {

    double time = 0;

    while (step(TIME_STEP) != -1) {

        time = getTime();
  
        if (inverse == 0)
        {
            double hipLeft = -0.5;
            double kneeLeft = 0;
            double ankleLeft = 0;
            double hipRight = -0.5;
            double kneeRight = 0;
            double ankleRight = 0;

            joints[0]->setPosition(hipLeft);
            joints[1]->setPosition(kneeLeft);
            joints[2]->setPosition(ankleLeft);
            joints[3]->setPosition(hipRight);
            joints[4]->setPosition(kneeRight);
            joints[5]->setPosition(ankleRight);

            // Write Data
            save_data << time << "\t";
            
            for (unsigned int j = 0; j < 6; j++)
            {
                save_data << joints_pos[j]->getValue() << "\t";
            }
            
            save_data << "\n";

            //std::cout << "Joint Pos : " << joints_pos[0]->getValue() << std::endl;

        }

        else
        {
        
            getJointPosition();

            for (uint j = 0; j < 6; j++) 
            {
                joints[j]->setPosition(joint_angles[j]);
            }
            
            for (uint k = 6; k <12;k++)
            {
                //cout << "CHECK" << endl;
                
                if((k==8) || (k==11))
                {
                    joints[k]->setTorque(0);
                }
                else
                {
                    joints[k]->setPosition(0);
                }
                
            }
            
            
            // Write Data
            save_data << time << "\t";
            
            for (unsigned int j = 0; j < 6; j++)
            {
                save_data << joints_pos[j]->getValue() << "\t";
            }
            
            save_data << "\n";

        }

    }

}



int Muscle::getJointPosition() {
    //static int count = 0;

    // Function to return the joint position angles from the kinematic data

    for (unsigned int j = 0; j < 6; j++) {
        joint_data_file >> joint_angles[j];
        //std::cout << "Angle " << j << " : " << joint_angles[j] << " : " << joints_pos[j]->getValue() << std::endl;
        //std::cout << "COUNT : " << count++ << std::endl;
    }
    if (joint_data_file.eof())
    {   std::cout << "END OF FILE :" << std::endl;
        //std::exit(EXIT_SUCCESS);
        return 1;
    }
    else
        return 0;
}



int main(int argc, char **argv) {

    Muscle *Muscle1 = new Muscle();

    Muscle1->MuscleSimulate();

    return 0;

    delete Muscle1;
}
