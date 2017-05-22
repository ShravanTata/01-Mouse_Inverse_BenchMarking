#include <iostream>
#include "parameters.hh"

using namespace std;

typedef std::vector<std::string> VString;
typedef std::vector<double> VDouble;
typedef std::vector<double> VInt;


void Parameters::loadFromFile(std::string filename){
    std::ifstream f(filename);
    std::stringstream s;
    if (f) {
        s << f.rdbuf();    
        f.close();
        if (s.str() == ""){
        	std::cout << "parameter file ( " << filename << " ) found but empty" << std::endl;
        }
        else{

        	return loadFromStream<std::stringstream>(s);
        }
    }
    else{
        std::cout << "parameter file ( " << filename << " ) not found" << std::endl;
    }
    
}



Parameters& Parameters::CreateFromFile(std::string filename){
    std::ifstream f(filename);
    std::stringstream s;
    if (f) {
        s << f.rdbuf();    
        f.close();
        if (s.str() == ""){
        	std::cout << "parameter file ( " << filename << " ) found but empty" << std::endl;
        	return Parameters::Create<double>("empty",0.0);
        }
        else{
        	return Parameters::CreateFromStream<std::stringstream>(s);
        }
    }
    else{
        std::cout << "parameter file ( " << filename << " ) not found" << std::endl;
        return Parameters::Create<double>("empty",0.0);
    }
    
}

void Parameters::parseYAMLNode(YAML::Node doc, std::string section_name){

		auto isList = [] (YAML::Node doc) -> bool {
			if (
					doc["list_name"].IsDefined()
					&&
					doc["list_content"].IsDefined()
				)
			{
				return true;
			}
			else
				return false;

		 };

		 auto isTable = [this] (YAML::Node doc) -> bool {

		 	if( 
		 		!doc["headers_type"].IsDefined()
		 		||
		 		!doc["headers_name"].IsDefined()
		 		|| 
		 		!doc["content"].IsDefined()
		 		){
		 		cout << "header_type / headers_name or content missing" << endl;
		 		return false;
		 }

			VString headers_type = string2Vector<string>(doc["headers_type"].as<string>());
			VString headers_name = string2Vector<string>(doc["headers_name"].as<string>());
			VString content = string2Vector<string>(doc["content"].as<string>());

		 	if(headers_type.size() != headers_name.size()){
		 		cout << "header_type size not the same than headers_name" << endl;
		 		return false;
		 	}

		 	if(content.size() % headers_name.size() != 0){
		 		cout << "content size = " << content.size() << ", header number = " << headers_name.size() << endl;
		 		return false;
		 	}

		 	return true;

		 };
		const std::string settings_type = doc[0][section_name].as<std::string>();
		for(unsigned i=0;i<doc.size();i++) {
			const std::string settings_type = doc[i][section_name].as<std::string>();
			if ( 
				settings_type == Type2String(int)
				){
				if ( isList(doc[i]))
					addYAMLNodeList<int>(doc[i]);
				else
					addYAMLNodeMap<int>(doc[i]);
				}
			else if ( 
				settings_type == Type2String(bool)
				){
				if ( isList(doc[i])){
					addYAMLNodeList<bool>(doc[i]);
				}
				else
					addYAMLNodeMap<bool>(doc[i]);
				}
			else if(
				settings_type == Type2String(double)
				){
				if ( isList(doc[i]))
					addYAMLNodeList<double>(doc[i]);
				else
					addYAMLNodeMap<double>(doc[i]);
			}
			else if(
				settings_type == Type2String(string)
				){
				if ( isList(doc[i])){
					addYAMLNodeList<std::string>(doc[i]);
				}
				else{
					addYAMLNodeMap<std::string>(doc[i]);
				}
			}
			else if(
				settings_type == "table"
				){
					if ( !isTable(doc[i])){
						cout << "bad table format, skipping" << endl;
						continue;
					}
					/*
					- parameters : double
					  list_name  :  muscles_length
					  list_content : 0.1 0.1 0.1
					- parameters : table
					  headers_type : string double double
					  headers_name : muscles_names muscles_length muscles_forces
					  content : A 0.1 100
					            B 0.2 200
					            C 0.3 300
					*/


					VString headers_type = string2Vector<string>(doc[i]["headers_type"].as<string>());
					VString headers_name = string2Vector<string>(doc[i]["headers_name"].as<string>());
					VString content = string2Vector<string>(doc[i]["content"].as<string>());

					

					for(unsigned int i=0; i<headers_name.size(); i++){
						stringstream ss;

						ss << "- parameters : " << headers_type[i] << endl;
						ss << "  list_name : " << headers_name[i] << endl;
						ss << "  list_content :";

						
						
						for(unsigned int j=0; j<content.size() / headers_name.size(); j++){
							ss << " " << content[i+j*headers_name.size()];
							
						}
						YAML::Node node = YAML::Load(ss.str());
						Parameters::parseYAMLNode(node,"parameters");
					}
			}
			else{
				std::cout << "Unknown type (" << settings_type << " )" << std::endl;
			}
		}	
}
