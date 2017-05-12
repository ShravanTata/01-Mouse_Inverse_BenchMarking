/*

This file is created as interface with Webots and libnmm package.
Author : STata
Data : 21-11-2016

*/
#include <iostream>
#include <limits>
#include "conveyer.hh"
#include <fstream>
#include <math.h>

using namespace std;
using namespace webots;


Conveyer::Conveyer() {


// Get the conveyer belt joint
    
    conveyer[0] = getMotor("BELT_L");
    conveyer[1] = getMotor("BELT_R");

}

Conveyer::~Conveyer() {

}

void Conveyer::runSync()
{  

  while (step(TIME_STEP) != -1)
  {
    
    conveyer[0]->setForce(0);
    conveyer[1]->setForce(0);

  }

}


int main(int argc, char **argv) {

    Conveyer *conveyer = new Conveyer();

    conveyer->runSync();

    return 0;

    delete conveyer;
}