#pragma once

#include <iostream>
#include <fstream>
#include <map>

#include "Core/Util/Debug/logging.h"


#ifndef CONFIG_PATH
    #define CONFIG_PATH "config.cfg"
#endif

/**
 * @brief Wrapper for configuration file loading
 * 
 */
class Configurator {
public:
    static std::string EngineSrc;

    static std::string ProjectConfig;
    static std::string ProjectName;
    static std::string ProjectLogDir;
    static std::string ProjectRoot;

    /**
     * @brief Load config from CONFIG_PATH
     * 
     * define CONFIG_PATH via build tool or in project
     */
    static void load();

    /**
     * @brief Get configuration value by proeprty name
     * 
     * @param property Property name
     * @return std::string 
     */
    static std::string get(std::string property);
private:
    static std::map<std::string, std::string> configuration;

    /**
     * @brief Helper for simple string trimming
     * 
     * @param entry String with whitespaces
     */
    static void trim(std::string & entry);
};