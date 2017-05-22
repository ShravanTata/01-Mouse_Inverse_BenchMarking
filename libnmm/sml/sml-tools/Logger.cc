#include "Logger.hh"
#include <sml/sml-tools/Settings.hh>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/any.hpp>

map<string, Logger*> LoggerManager::loggers;

void LoggerManager::init(){
	const int LogOn =Settings::get<int>("save_for_matlab");
	if(LogOn != 1) return;
	for(auto&kv: loggers){
		kv.second->createFiles();
		kv.second->writeHeader();
	}
}

void LoggerManager::step(){
	const int LogOn =Settings::get<int>("save_for_matlab");
	if(LogOn != 1) return;
	for(auto&kv: loggers ){
		kv.second->writeContent();
	}
}

void LoggerManager::end(){
	const int LogOn =Settings::get<int>("save_for_matlab");
	if(LogOn != 1) return;
	for(auto&kv: loggers ){
		kv.second->closeFiles();
	}
}




void Logger::closeFiles(){
	if(LogOn != 1) return;
		for(auto& kv : raw_files) kv.second->close();
}
void Logger::createFiles(){
	int i=1;
	string out = boost::lexical_cast<std::string>(i);
    if(LogOverwrite==0){
        while(fexists(Path+Folder+raw_filesname[0]+out) || fexists(Path+Folder+raw_filesname[1]+out)){
            i++;
            out = boost::lexical_cast<std::string>(i);
        }
    }
    else{
        i = LogOverwrite;
		out = boost::lexical_cast<std::string>(i);
	}
	std::ofstream *file;
	for (auto & it : raw_filesname){
		file = new ofstream();
		raw_files[it] = file;
		raw_files[it]->open(Path+Folder+it+out);
	}
}

void MusculoSkeletalLogger::writeHeader(){
	if(LogOn != 1) return;
	
	int id=0;
	for(auto& mu : body.muscles)
	{
		*raw_files["muscles_force"] << mu->getName() << " ";
		*raw_files["muscles_v_CE"] << mu->getName() << " ";
		*raw_files["muscles_f_v"] << mu->getName() << " ";
		*raw_files["muscles_length"] << mu->getName() << " ";
		*raw_files["muscles_activity"] << mu->getName() << " " ;
		*raw_files["muscles_noise"] << mu->getName() << " " ;
        id++;
	}
	for(auto& joint : body.joints)
	{
		*raw_files["joints_force"] << joint->getName() << " ";
		*raw_files["joints_angle"] << joint->getName() << " ";
		*raw_files["joints_position_y"] << joint->getName() << " ";
		*raw_files["joints_position_z"] << joint->getName() << " ";
	}
    
	for(auto& kv :this->body.sensors)
	{
		*raw_files["sensors_activity"] << kv.first << " ";
	}

	for(auto &kv : raw_files)
		*kv.second << endl;
    
}

void MusculoSkeletalLogger::writeContent(){
	if(LogOn != 1) return;
	for(auto& mu :body.muscles)
	{
		*raw_files["muscles_force"] << mu->getForce() << " ";
		*raw_files["muscles_v_CE"] << mu->getV_CE() << " ";
		*raw_files["muscles_length"] << mu->getL_CE() << " ";
		*raw_files["muscles_f_v"] << mu->getF_v() << " ";
		*raw_files["muscles_activity"] << mu->getA() << " " ;
		*raw_files["muscles_noise"] << mu->getNoise() << " " ;
	}
	print_debug("--writing muscles");
	for(auto& joint : body.joints)
	{
		*raw_files["joints_force"] << joint->torque + joint->torque_soft_limit << " ";
		*raw_files["joints_angle"] << joint->getAngle() << " ";
		//*raw_files["joints_position_z"] << this->body.getFromDef(joint->getName())->getPosition()[2]-this->body.getFromDef("REGIS")->getPosition()[2] << " ";
		//*raw_files["joints_position_y"] << this->body.getFromDef(joint->getName())->getPosition()[1]-this->body.getFromDef("REGIS")->getPosition()[1] << " ";
	}
	print_debug("--writing joints");
	//for(auto& kv :this->body.sensors)
	//{
	//	//*raw_files["sensors_activity"] << kv.second->get() << " ";
	//}
	print_debug("--writing sensors");        
	for(auto &kv : raw_files)
		*kv.second << endl;
}

void ReflexControllerLogger::writeHeader()
{
	if(LogOn != 1) return;
	for(auto& mu : controller.motoneurons)
	{
		*raw_files["motoneurons_activity"] << mu->getName() << " " ;
	}
	for(auto& kv :controller.INsens)
	{
        auto i = &kv - &controller.INsens[0];
		*raw_files["feedbacks"] << SENSORY_INTERNEURON::toString(i) << " ";
		*raw_files["cpgs"] << SENSORY_INTERNEURON::toString(i) << " ";
	}
    for (int INsen=SENSORY_INTERNEURON::FIRST; INsen<=SENSORY_INTERNEURON::LAST; INsen++){
        for(int MN=MUSCLES::FIRST; MN<=MUSCLES::LAST; MN++){
            if(SENSORY_INTERNEURON::CONNECTION_TO_MOTONEURONS[INsen][MN]){
                *raw_files["interneurons"] << SENSORY_INTERNEURON::toString(INsen) << " ";
            }   
        }
    }
	for(auto &kv : raw_files)
		*kv.second << endl;
}

void ReflexControllerLogger::writeContent()
{
	if(LogOn != 1) return;
	for(auto& mn : controller.motoneurons)
	{
		*raw_files["motoneurons_activity"] << ((Entity *)mn)->get() << " " ;
	}
	for(auto& kv :controller.INcpgs)
	{
		*raw_files["cpgs"] << kv->get() << " ";
	}
	for(auto& kv :controller.INsens)
	{
		*raw_files["feedbacks"] << kv->get() << " ";
	}
    for (int INsen=SENSORY_INTERNEURON::FIRST; INsen<=SENSORY_INTERNEURON::LAST; INsen++){
        for(int MN=MUSCLES::FIRST; MN<=MUSCLES::LAST; MN++){
            if(SENSORY_INTERNEURON::CONNECTION_TO_MOTONEURONS[INsen][MN]){
                *raw_files["interneurons"] << controller.__INsen_MN[INsen][MN]->input->get() << " ";
            }   
        }
    }
    
	for(auto &kv : raw_files)
		*kv.second << endl;
    
}

