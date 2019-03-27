#include "configurator.h"

std::string Configurator::EngineSrc     = "EngineSrc";
std::string Configurator::ProjectConfig = "ProjectConfig";
std::string Configurator::ProjectName   = "ProjectName";
std::string Configurator::ProjectLogDir = "ProjectLogDir";
std::string Configurator::ProjectRoot   = "ProjectRoot";

std::map<std::string, std::string> Configurator::configuration;

void Configurator::load() {
    std::fstream configFile;
    configFile.open(CONFIG_PATH);

    //create new config if file does not exist
    if(configFile.fail()) {
        configFile.close();
        configFile.open(CONFIG_PATH, std::fstream::out);
        configFile << "#Engine"                           << std::endl;
        configFile << EngineSrc     << "="                << std::endl;
        configFile << "#Project"                          << std::endl;
        configFile << ProjectConfig << "=" << CONFIG_PATH << std::endl;
        configFile << ProjectName   << "="                << std::endl;
        configFile << ProjectLogDir << "="                << std::endl;
        configuration[EngineSrc]     = "";
        configuration[ProjectConfig] = std::string(CONFIG_PATH);
        configuration[ProjectName]   = "";
        configuration[ProjectLogDir] = "";
        configuration[ProjectRoot]   = "";
    }
    else if(configFile.good()) {
       std::string configLine;
       while(std::getline(configFile, configLine)) {
           size_t delimeterPos = configLine.find("=");
           if(delimeterPos != std::string::npos) {
                std::string property = configLine.substr(0, delimeterPos);
                trim(property);

                std::string value = configLine.substr(delimeterPos+1, configLine.size()-(delimeterPos+1));
                trim(value);

                configuration[property] = value;
           }
       } 
    }
    configFile.close();
}

std::string Configurator::get(std::string property) {
    auto propertyLoc = configuration.find(property);
    if(propertyLoc != configuration.end()) {
        return configuration[property];
    }
    Log::write(LogType::Error, "Property " + property + " was not set. Please update the config file", "Config");
    return "Error";
}

void Configurator::trim(std::string & entry) {
    //left trim
    for(auto i = entry.begin(); i != entry.end(); i++) {
        if(!std::isspace(*i)) {
            break;
        }
        entry.erase(i);
    }
    //right trim
    for(int i = entry.size()-1; i >= 0; i--) {
        if(!std::isspace(entry[i])) {
            break;
        }
        entry.erase(i);
    }
}