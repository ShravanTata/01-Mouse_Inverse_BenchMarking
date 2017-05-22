#pragma once
#ifndef __GeyerSml_HH__
#define __GeyerSml_HH__

#include <sml/musculoSkeletalSystem/Sml.hh>

class GeyerSml: public Sml
{
public:
	ReflexController geyerController;
	void loadFeedback(SIDE::Side side);
	void loadCpg(SIDE::Side side);
	void loadPartialCpg(SIDE::Side side);
	virtual void initialiseSpinalControl();
	virtual void step_SPINE_TO_MTU(double dt);
};

#endif