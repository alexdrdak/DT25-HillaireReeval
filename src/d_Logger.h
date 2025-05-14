#ifndef DT25_D_LOGGER_H
#define DT25_D_LOGGER_H

#include <string>
#include <iostream>
#include <iomanip>

class d_Logger {
public:
    static void err(std::string const& message) {
        std::time_t time_now = std::time(nullptr);
        std::cout << "[" << std::put_time(std::localtime(&time_now), "%F %T") << "] ERR: " << message << std::endl;
    }

    static void info(std::string const& message) {
        std::time_t time_now = std::time(nullptr);
        std::cout << "[" << std::put_time(std::localtime(&time_now), "%F %T") << "] INFO: " << message << std::endl;
    }

    static void warn(std::string const& message) {
        std::time_t time_now = std::time(nullptr);
        std::cout << "[" << std::put_time(std::localtime(&time_now), "%F %T") << "] WARN: " << message << std::endl;
    }

    static void debug(std::string const& message) {
        std::time_t time_now = std::time(nullptr);
        std::cout << "[" << std::put_time(std::localtime(&time_now), "%F %T") << "] DEBUG: " << message << std::endl;
    }
};


#endif