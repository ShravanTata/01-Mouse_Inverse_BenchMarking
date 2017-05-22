#include <chrono>
#include <iostream>
#include <vector>
#include <iomanip>


namespace time_unit{
typedef std::chrono::milliseconds ms;
typedef std::chrono::seconds s;
typedef std::chrono::nanoseconds ns;
typedef std::chrono::microseconds us;
typedef std::chrono::duration<double> fsec;
}



template <class T>
class Clock {
public:
	std::vector<time_unit::fsec> durations;
	std::chrono::system_clock::time_point duration;


	void start(){
		duration = std::chrono::high_resolution_clock::now();
	}
	void end(){
		durations.push_back(std::chrono::high_resolution_clock::now()-duration);
	}
	void printCurrent(){
		if ( durations.size() > 0){
			T d = std::chrono::duration_cast<T>(durations.back());
			std::cout << d.count() << std::setprecision(5) << std::endl;
		}
	}
	double getMean(){
		double total = 0;
		for(auto &kv: durations){
			T d = std::chrono::duration_cast<T>(durations.back());
			total+=d.count();
		}
		return total/durations.size();
	}
	void printMean(){
		
		std::cout << getMean() << std::setprecision(5) << std::endl;
	}
};

