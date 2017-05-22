#include "state.hh"
#include "link.hh"
#include "network_manager.hh"
#include <iostream>

using namespace std;


Real State::nan = std::nan("1");


 State::State(const State &vec): X(vec.X){
 	init();
}
 State::State(const std::vector<Real>& X) : X(X){
 	init();
}

/* --------------------------------------------------------------------------- */

 State::~State(){
	NetworkManager::RemoveState(this);
}


/* --------------------------------------------------------------------------- */

void State::init(){
	transferFunction = TransferFunction();
	transferFunction.super = this;
	transferFunction.tf = new TransferFunctions::Identity();

	for(int i=0; i<=MEMORY; i++){
		Vector * x = new Vector(X);
		Xqueue.push_front( x );
	}
}

void State::save(){
	Vector * x = new Vector(X);
	Xqueue.push_front ( x );
	Xqueue.pop_back ();
}
/* --------------------------------------------------------------------------- */

UInt State::dimension(){
	/*
		Returns the dimension of the State
	*/
	return X.size();
}
UInt State::dimension() const{
	return X.size();
}



State::TransferFunction::TransferFunction() {

}
void State::TransferFunction::set(TransferFunctions::Base* f){
	tf = f;
}
State State::TransferFunction::operator ()() const{
	auto& f = *super->transferFunction.tf;
	std::vector<double> X_;
	for (auto& kv: super->X)
		X_.push_back((f)(kv));
	
	return State(X_);
}
Real State::TransferFunction::operator ()(UInt i) const{
	auto& f = *super->transferFunction.tf;
	if (i < super->X.size()){
		return (f)(super->X[i]);
	}

	cout << "Error State size is " << super->X.size() << " but requested element " << i+1 << endl;
	return super->State::nan;
}
Real State::TransferFunction::operator ()(UInt i, UInt Dt) const{
	/*
		read () overloading operator
	*/
	auto& f = *super->transferFunction.tf;
	if (Dt > MEMORY){
		Dt = MEMORY;
		cout << "Error requested state " << Dt << " time step before" << endl;
		cout << "Limit was : " << MEMORY << " time step " << endl;
	} 
	if (i < super->X.size()){
		Real x = (*super->Xqueue[Dt])[i];
		return f(x);
	}

	cout << "Error State size is " << super->X.size() << " but requested element " << i+1 << endl;
	return super->State::nan;
}





/* --------------------------------------------------------------------------- */

