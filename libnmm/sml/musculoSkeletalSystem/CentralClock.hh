#ifndef __CENTRALCLOCK_HH__
#define __CENTRALCLOCK_HH__

/*!
 * \file CentralClock.hh
 * \author efx
 * \version 0.1
 */

/*! \class CentralClock
 *  \brief Global frequency of the CPG
 * 
 * 
 *  This class controls the global frequency of the CPG
 */
class CentralClock{
private:
	double & frequency;
public:
	CentralClock(double& f):frequency(f){};
	double getFrequency(){return frequency;}
	void setFrequency(double& f){frequency = f;}
};

#endif