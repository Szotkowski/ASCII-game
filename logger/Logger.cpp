//
// Created by Michael Szotkowski on 6/17/2024.
//

#include "Logger.h"

Logger::Logger() {
    std::filesystem::create_directories("logs");

    archiveLogFile();

    // Open the log file in the logs directory
    logFile_.open("logs/latest.log", std::ios_base::app);
    if (!logFile_.is_open()) {
        std::cerr << RED_TEXT << "Unable to open log file!" << RESET_TEXT << std::endl;
    }
}

Logger::~Logger() {
    if (logFile_.is_open()) {
        logFile_.close();
    }
}

Logger &Logger::getInstance() {
    static Logger instance;
    return instance;
}

void Logger::log(LogLevel level, const std::string& message) {
    if (logFile_.is_open()) {
        logFile_ << formatLogMessage(level, message) << std::endl;
    } else {
        std::cerr << RED_TEXT << "Unable to open log file!" << RESET_TEXT << std::endl;
    }
}

std::string Logger::formatLogMessage(LogLevel level, const std::string& message) {
    std::ostringstream oss;
    oss << "[" << logLevelToString(level) << ": " << getCurrentTime() << "] " << message;
    return oss.str();
}

std::string Logger::logLevelToString(LogLevel level) {
    switch (level) {
    case Info:
        return "Info";
    case Error:
        return "Error";
    case Warning:
        return "Warning";
    default:
        return "Unknown";
    }
}

std::string Logger::getCurrentTime() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()) % 1000000;

    std::ostringstream oss;
    oss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
    oss << '.' << std::setw(6) << std::setfill('0') << ms.count();
    return oss.str();
}


void Logger::showLog() {
    // Get the current working directory
    char currentDir[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, currentDir);

    // Set the log file path
    std::string logFilePath = std::string(currentDir) + "\\logs\\latest.log";

    // Check if the log file exists
    if (GetFileAttributes(logFilePath.c_str()) == INVALID_FILE_ATTRIBUTES) {
        std::cerr << RED_TEXT << "Log file not found: " << logFilePath << RESET_TEXT << std::endl;
        return;
    }

    // Command to open the log file with Notepad
    std::string command = "notepad \"" + logFilePath + "\"";

    // Set up the process information
    STARTUPINFO si = { sizeof(STARTUPINFO) };
    PROCESS_INFORMATION pi;

    // Create the process
    if (!CreateProcess(NULL, const_cast<char*>(command.c_str()), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        std::cerr << RED_TEXT << "CreateProcess failed (" << GetLastError() << ")." << RESET_TEXT << std::endl;
        return;
    }

    // Close process and thread handles
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}


void Logger::archiveLogFile() {
    const std::string logFilePath = "logs/latest.log";
    if (std::filesystem::exists(logFilePath)) {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        std::tm local_tm = *std::localtime(&in_time_t);

        std::ostringstream date_oss;
        date_oss << std::put_time(&local_tm, "%Y-%m-%d");
        std::string date_str = date_oss.str();

        int sequence_number = 1;
        std::string newLogFileName;

        do {
            std::ostringstream new_oss;
            new_oss << "logs/" << "log-" << date_str << "-" << sequence_number << ".log";
            newLogFileName = new_oss.str();
            sequence_number++;
        } while (std::filesystem::exists(newLogFileName));

        std::filesystem::rename(logFilePath, newLogFileName);
    }
}