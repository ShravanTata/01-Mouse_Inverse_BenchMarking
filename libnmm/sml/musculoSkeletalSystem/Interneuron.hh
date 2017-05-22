#pragma once
#ifndef __SIGNAL_HH__
#define __SIGNAL_HH__

/*!
 * \file Interneuron.hh
 * \brief Interneurons definition (all are children of the Entity mother class) (mono-sensory / bi-sensory / cpg)
 * \author efx
 * \version 0.1
 */

#include <string>
#include <math.h>
#include <fstream>
#include <map>
#include <sml/musculoSkeletalSystem/Entity.hh>
#include <sml/sml-tools/RungeKutta.hh>

class Sensor;
class Foot;
#include <boost/any.hpp>
/**
 * 
 *  Mother SensoryInterneuron class
 * 
 */
extern double weight_const;
class Interneuron : public Entity{
public:
    std::string file;
	Entity * output_neuron; // output_neuron in output
	double& weight; // connexion weight
	double factor;
	Interneuron() : Entity(), file(""),weight(weight_const),factor(1.0){};
	Interneuron(Entity * output_neuron, std::string file) : Entity(), file(file), output_neuron(output_neuron), weight(weight_const),factor(1.0){};
	Interneuron(Entity * output_neuron, double& weight, std::string file) : Entity(), file(file), output_neuron(output_neuron), weight(weight),factor(1.0){};
	Interneuron(Entity * output_neuron, double& weight, double factor, std::string file) : Entity(), file(file), output_neuron(output_neuron), weight(weight),factor(factor){};
	Interneuron(Entity * output_neuron, ACTIVE_DURING::ActiveDuring active_during, std::string file) : Entity(SIDE::ALL,active_during), file(file), output_neuron(output_neuron), weight(weight_const),factor(1.0){};
	Interneuron(Entity * output_neuron, double& weight, ACTIVE_DURING::ActiveDuring active_during, std::string file) : Entity(SIDE::ALL,active_during), file(file), output_neuron(output_neuron), weight(weight),factor(1.0){};
	Interneuron(Entity * output_neuron, double& weight, double factor, ACTIVE_DURING::ActiveDuring active_during, std::string file) : Entity(SIDE::ALL,active_during), file(file), output_neuron(output_neuron), weight(weight),factor(factor){};
	virtual void apply() = 0;
    virtual double tf(double x);
	virtual void apply(double o, double a) = 0;
	virtual void add(double state ){this->state += state;}
	virtual Entity * getOutputEntity(){return output_neuron;}
};
class SensoryInterneuron : public Interneuron{
public:
	const double& sensor; // sensor in input
	SensoryInterneuron(
        std::string file,
		ACTIVE_DURING::ActiveDuring active_during,
		const double& sensor,
		Entity * output_neuron,
		double& weight): Interneuron(output_neuron,weight,active_during,file),sensor(sensor){}
	SensoryInterneuron(
        std::string file,
		ACTIVE_DURING::ActiveDuring active_during,
		const double& sensor,
		Entity * output_neuron,
		double& weight,
		double factor): Interneuron(output_neuron,weight,factor,active_during,file),sensor(sensor){}
	void apply();
	void apply(double o, double a);
	};
class SensorySensoryInterneuron : public Interneuron{
public:
	const double& sensor1; // sensor in input
	const double& sensor2;
	
	SensorySensoryInterneuron(
        std::string file,
		ACTIVE_DURING::ActiveDuring active_during,
		const double& sensor1,
		const double& sensor2,
		Entity * output_neuron,
		double& weight): Interneuron(output_neuron,weight,active_during,file), sensor1(sensor1), sensor2(sensor2){};
	SensorySensoryInterneuron(
        std::string file,
		ACTIVE_DURING::ActiveDuring active_during,
		const double& sensor1,
		const double& sensor2,
		Entity * output_neuron,
		double& weight,
		double factor): Interneuron(output_neuron,weight,factor,active_during,file), sensor1(sensor1), sensor2(sensor2){};
	void apply();
	void apply(double o, double a);
    double tf(double x);
	};
		
class CPGInterneuron : public Interneuron, public Derivatives{
public:
	double dt;
    double * parameter_level2;
	double * parameter_level3;
	double getParameter_level2();
	double getParameter_level3();
	bool isCst();
	double t;
	double tau;
	double offset;
	double amplitude;
	double mean;
	double freq;
	double period;
	std::string file;
	Foot * foot;
	std::vector<double> yi;
	std::vector<double> teacher;
	std::vector<double> teacher_d;
	double f;
	CPGInterneuron(
        ACTIVE_DURING::ActiveDuring active_during, 
        int time_step,Entity * output_neuron, 
        Foot * foot, 
        std::string file);
	void update_state();
	void apply();
	void apply(double o, double a);
	std::vector<double> dydt(double t, std::vector<double> y, std::vector<double> param, std::vector<double> input);
	double getDG(double val);
	double getG(double val);
private:
	void initialize();
	void initialize(double mean, double std);
};
#endif /* __SIGNAL_HH__ */