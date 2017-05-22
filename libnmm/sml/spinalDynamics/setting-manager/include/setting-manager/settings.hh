#ifndef __SETTINGS__HH__
#define __SETTINGS__HH__

#include <iostream>
#include <map>
#include <string>
#include <yaml-cpp/yaml.h>

#include <setting-manager/parameters.hh> 


/**
 * @brief Settings class 
 * @details This class is used to access and store settings. Settings can be either loaded one by one with the get method
 *          or with a string in a yaml format as follow : 
 *              \verbatim
             - settings: int
               a: 1
               b: 2
             - settings: double
               a: 1.0
               b: 2.0
             - settings: string
               a: 1
               b: 2
               \endverbatim
 */
class Settings {
public:
		    template<typename T>
        static void set(std::string key, T value){
            data.get<T>(key)=value;
        }

        template<typename T>
        static T& get(std::string key){
            return data.get<T>(key);
        }
        template<typename T, typename... Args>
        static void addList(Args... args){
          data.addList<T>(args...);
        }
        template<typename T>
        static void addListFromStream(T& t){
        	YAML::Node doc = YAML::Load(t);
          data.parseYAMLNode(doc,"settings");
        }
private:
		    static Parameters data;
};


#endif