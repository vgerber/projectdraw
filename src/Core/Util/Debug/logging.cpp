#include "logging.h"

std::fstream Log::logFile;
std::string Log::filterType;
bool Log::useFilter = false;

void Log::load(std::string outputDirectory) {
    logFile.open(outputDirectory + "output.log", std::fstream::out | std::fstream::app);
    if(logFile.fail()) {
        std::cout << "Failed to create " << outputDirectory << "output.log" << std::endl;
    }
    if(logFile.good()) {
        logFile << "----LogStart---" << std::endl;
    }
}

void Log::write(LogType type, std::string message, std::string module) {
    Log::write(getLogTypeString(type), message, module);
}

void Log::write(std::string type, std::string message, std::string module) {
    //filter wrong types out
    if(useFilter && type != filterType) {
        return;
    }

    type = "[" + type + "]";

    if(module.size() > 0) {
        module = "[" + module + "] ";
    }

    //get time stamp as tring
    time_t rawTime;
    struct tm * timeInfo;
    char timeBuffer[80];

    time(&rawTime);
    timeInfo = localtime(&rawTime);

    strftime(timeBuffer, sizeof(timeBuffer), "%d-%m-%Y %H:%M:%S", timeInfo);
    std::string timeString = "[" + std::string(timeBuffer) + "]";

    //combine all log parameters
    std::string outString = timeString + " " + type + " " + module + message;

    if(logFile.is_open() && logFile.good()) {
        logFile << outString << std::endl;
    }
    else {
        std::cout << outString << std::endl;
    }
}

void Log::setFilter(bool enable, LogType type) {
    setFilter(enable, getLogTypeString(type));
}

void Log::setFilter(bool enable, std::string type) {
    useFilter = enable;
    if(useFilter) {
        filterType = type;
    }
}

void Log::dispose() {
    logFile.close();
}

std::string Log::getLogTypeString(LogType type) {
    switch (type)
    {
        case LogType::Error   : return "Error";
        case LogType::Info    : return "Info";
        case LogType::Warning : return "Warning"; 
        default               : return "Undefined";
    }
}