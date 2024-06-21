//
// Created by Michael Szotkowski on 6/17/2024.
//

#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <mutex>
#include <memory>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <thread>
#include <filesystem>
#include <windows.h>

enum LogLevel {
    Info,
    Warning,
    Error
};

class Logger {
private:
    Logger();

    std::string formatLogMessage(LogLevel type, const std::string &message);

    std::string logLevelToString(LogLevel type);

    std::string getCurrentTime();

    std::ofstream logFile_;

public:
    ~Logger();

    static Logger &getInstance();

    void log(LogLevel type, const std::string &message);

    Logger(const Logger &) = delete;

    Logger &operator=(const Logger &) = delete;

    Logger(Logger &&) = delete;

    Logger &operator=(Logger &&) = delete;

    static void showLog();

    void archiveLogFile();
};

// Usage macro to simplify logging
#define LOG_INFO(message) Logger::getInstance().log(LogLevel::Info, message)
#define LOG_ERROR(message) Logger::getInstance().log(LogLevel::Error, message)
#define LOG_WARNING(message) Logger::getInstance().log(LogLevel::Warning, message)

#define RED_TEXT "\033[1;31m"
#define GREEN_TEXT "\033[1;32m"
#define YELLOW_TEXT "\033[1;33m"
#define RESET_TEXT "\033[0m"

#endif //LOGGER_H
