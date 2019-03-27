#pragma once

#include <fstream>
#include <iostream>
#include <ctime>

enum LogType {
    Info,
    Warning,
    Error
};

/**
 * @brief Wrapper for loggin functionality
 * 
 */
class Log {
public:
    /**
     * @brief Open output filestreams
     * 
     * @param outputDirectory 
     */
    static void load(std::string outputDirectory);

    /**
     * @brief Writes message to log file
     * 
     * @param type Log type
     * @param message Log message
     * @param module Module name
     */
    static void write(LogType type, std::string message, std::string module = "");

    /**
     * @brief Writes message of custom LogType to log file
     * 
     * @param type Custom LogType
     * @param message Log message
     * @param module Module name
     */
    static void write(std::string type, std::string message, std::string module = "");

    /**
     * @brief Set Log Filter
     * 
     * Changes LogType for filter if enabled is true
     * 
     * @param enable true if filter is activated
     * @param type Will output type logs only
     */
    static void setFilter(bool enable, LogType type = LogType::Error);

    /**
     * @brief Set Log Filter
     * 
     * Changes LogType for filter if enabled is true
     * 
     * @param enable True if filer is activated
     * @param type Will output type logs only
     */
    static void setFilter(bool enable, std::string type = "");

    /**
     * @brief Closes filestreams
     * 
     */
    static void dispose();
private:
    static std::fstream logFile;
    static bool useFilter;
    static std::string filterType;

    static std::string getLogTypeString(LogType type);
};