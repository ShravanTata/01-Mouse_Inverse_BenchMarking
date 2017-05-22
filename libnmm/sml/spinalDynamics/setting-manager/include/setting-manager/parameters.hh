#ifndef __PARAMETERS__HH__
#define __PARAMETERS__HH__

#include <typeinfo>
#include <typeindex>
#include <unordered_map>
#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>
#include <sstream>
#include <fstream>
#include <exception>
#include <iostream>

#define Type2String(x) _Type2String<x>(#x)

template<typename T>
std::string _Type2String (std::string ltype){
	return ltype;
}

/**
 * @brief Data_ empty class used to create a map of different submaps type
 * @details [long description]
 * 
 */
class Data_{

};

/**
 * @brief Data template class that stores an ordered map of type T
 * 
 * @tparam T type of the elements stored in the map
 */
template <class T>
class Data : public Data_
{
 public:
	std::unordered_map<std::string,T> data;
};


using set_data_map = std::unordered_map<std::type_index, Data_*>;

/**
 * @brief Parameters class.
 * @details This class is used to store maps of parameters of different type. It supports loading parameters described in a yaml file.
 * 
 * 
 * Example of yaml format : 
 * 
\code{.yaml}
- parameters : double
  p1 : 0.1
  p2 : 0.2
  p3 : 0.3
- parameters : bool
  b1 : true
  b2 : true
  b3 : false
- parameters : string
  list_name  :  muscles
  list_content : A B C D E F
- parameters : double
  list_name  :  muscles_length
  list_content : 0.1 0.1 0.1
- parameters : table
  headers_type : string double double bool
  headers_name : muscles_names muscles_pennation muscles_forces muscles_locked
  content : A 0.1 100 true
            B 0.2 200 true
            C 0.3 300 true
 \endcode
 */
class Parameters
{
private:
	/**
	 * @brief calls the add(const std::string& key, const T t)
	 * @details used in the context of recursive variadic call to add a list of parameters
	 * 
	 * @param key the name of the parameter
	 * @param value the value of the parameter
	 */
	template <typename T>
	void _addList(std::string key, T value);

	/**
	 * @brief calls the add(const std::string& key, const T t) and _addList with remaining parameter list
	 * @details used in the context of recursive variadic call to add a list of parameters
	 * 
	 * @param key the name of the current parameter to be added
	 * @param value the parameter value
	 * @param args the remaining parameter to be added
	 */
	template<typename T, typename... Args>
	void _addList(std::string key, T value, Args... args);

public:
	/**
	 * @brief Adds a list of parameters
	 * @details example usage: parameter.addList<double>("param1",1.0,"param2", 2.0, "param3", 3.0);
	 * 
	 * @param args the list of parameters
	 * @tparam T the type of the parameters in the parameter list
	 */
	template<typename T, typename... Args>
	void addList(Args... args);

	/**
	 * @brief loads parameters given in the stream in parameter (using yaml format)
	 * @details [long description]
	 * 
	 * @param ss [description]
	 */
	template<typename T>
	void addListFromStream(T& ss);

	/**
	 * @brief Parse the yaml node given in parameter looking for the section_name
	 * 
	 * @param doc the yaml node
	 * @param section_name the name of the section to be parsed
	 */
	void parseYAMLNode(YAML::Node doc, std::string section_name);

	/**
	 * @brief Loads a maps of parameter from a yaml node
	 * @details Example of code
\code{.yaml}
- parameters : double
  p1 : 0.1
  p2 : 0.2
  p3 : 0.3
- parameters : bool
  b1 : true
  b2 : true
  b3 : false
\endcode
	 * 
	 * @param doc the yaml node
	 * @tparam T the type of the parameters to be loaded
	 */
	template<typename T>
	void addYAMLNodeMap(YAML::Node doc);

	/**
	 * @brief Loads a list of parameter from a yaml node
	 * @details Example of code
\code{.yaml}
- parameters : double
  list_name  :  muscles_length
  list_content : 0.1 0.1 0.1
\endcode
	 * 
	 * @param doc the yaml node
	 * @tparam T the type of the parameters to be loaded
	 */
	template<typename T>
	void addYAMLNodeList(YAML::Node doc);


	/**
	 * @brief Transform a string into a vector of type T (using ' ' as separator)
	 * 
	 * @param s the string
	 * @tparam T the type of the vector to create
	 * @return the vector of type T containing the element in the string s split by ' '
	 */
	template<typename T>
	std::vector<T> string2Vector(std::string s);

	/**
	 * @brief Creates a new Parameter from the stream passed into parameters
	 * 
	 * @param ss  the stream
	 * @return a Parameter object containing the parameter as given in the stream ss
	 */
	template<typename T>
	static Parameters& CreateFromStream(T& ss);

	/**
	 * @brief Creates a new Parameter from list passed in parameters
	 * 
	 */
	template<typename T, typename... Args>
	static Parameters& Create(Args... args);

	/**
	 * @brief Creates a new Parameter from a file called filename
	 */
	static Parameters& CreateFromFile(std::string filename);

	/**
	 * @brief loads new Parameter from the stream passed into parameter
	 */
	template<typename T>
	void loadFromStream(T& ss);

	/**
	 * @brief loads new Parameter from the file filename
	 */
	void loadFromFile(std::string filename);

	/**
	 * @brief Adds a single parameter into the object	
	 * 
	 * @param key the parameter name
	 * @param t the parameter value
	 */
	template<typename T>
	void add(const std::string& key, const T t);

	/**
	 * @brief Returns the parameters of type T associated to string key
	 * 
	 * @param key the parameter name
	 * @param T the type of the parameter
	 * @return the parameter value
	 *
	 */
	template<typename T>
	T& get(std::string key);

	/**
	 * @brief Returns the vector of type T associated to the string key
	 * 
	 * @param key the name of the vector 
	 * @return the vector 
	 */
	template<typename T>
	std::vector<T>& getVector(std::string key);

public:


private:
	set_data_map sets;

};


template <typename T>
void Parameters::_addList(std::string key, T value)
{
    add<T>(key, value);
}

template<typename T, typename... Args>
void Parameters::_addList(std::string key, T value, Args... args) // recursive variadic function
{
    add<T>(key, value);
    _addList(args...) ;
}


template<typename T, typename... Args>
void Parameters::addList(Args... args){
	_addList<T>(args...);
}
template<typename T>
void Parameters::addListFromStream(T& ss){
	YAML::Node doc = YAML::Load(ss);
	parseYAMLNode(doc,"parameters");
}


template<typename T>
void Parameters::addYAMLNodeMap(YAML::Node doc){
	for(auto kv: doc){
		const std::string key_str = kv.first.as<std::string>();
		const std::string value_str = kv.second.as<std::string>();
		try{
			if ( key_str != "parameters"){
				const T value = kv.second.as<T>();
				add<T>(key_str, value);
			}
		}
		catch(std::exception e){

		}
	}
}

template<typename T>
void Parameters::addYAMLNodeList(YAML::Node doc){
	std::vector<std::string> vstr = string2Vector<std::string>(doc["list_content"].as<std::string>());
	std::vector<T> myvec;

	for (auto&kv : vstr){
		YAML::Node tmp = YAML::Node(kv);
		myvec.push_back(tmp.as<T>());
	}
	add<std::vector<T>>(doc["list_name"].as<std::string>(), myvec);
}

template<typename T>
std::vector<T> Parameters::string2Vector(std::string s){
	std::stringstream ss(s);
	std::istream_iterator<T> begin(ss);
	std::istream_iterator<T> end;
	std::vector<T> vec(begin, end);
	return vec;
}



template<typename T>
Parameters& Parameters::CreateFromStream(T& ss){
	Parameters * P = new Parameters();
	P->addListFromStream<T>(ss);
	return *P;
}


template<typename T, typename... Args>
Parameters& Parameters::Create(Args... args){
	Parameters * P = new Parameters();
	P->addList<T>(args...);
	return *P;
}

template<typename T>
void Parameters::loadFromStream(T& ss){
	addListFromStream<T>(ss);
}


template<typename T>
void Parameters::add(const std::string& key, const T t)
{
	if (sets.find(typeid(T)) == sets.end())
		sets[typeid(T)] = new Data<T>;
	static_cast<Data<T>*>(sets[typeid(T)])->data[key] = t;
}
template<typename T>
T& Parameters::get(std::string key)
{
	if (sets.find(typeid(T)) == sets.end())
		sets[typeid(T)] = new Data<T>;
	return static_cast<Data<T>*>(sets[typeid(T)])->data[key];
}
template<typename T>
std::vector<T>& Parameters::getVector(std::string key)
{
	return get<std::vector<T>>(key);
}




/**
 * @brief Basic parametrized class
 * @details Any class inheriting from this class will have a parameters field accessible.
 * Moreover a set of 4 default parameters (from p0 to p3) are created and set to 1.0
 */
struct Parametrized{
  Parametrized(){
    parameters.get<double>("p0") = 1.0;
    parameters.get<double>("p1") = 1.0;
    parameters.get<double>("p2") = 1.0;
    parameters.get<double>("p3") = 1.0;
  }
  Parameters parameters;
};


#endif
