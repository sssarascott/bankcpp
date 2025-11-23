#include "DataManager.hpp"
#include <iostream>

DataManager::DataManager() {
    // Constructor logic, if any
}

DataManager& DataManager::getInstance() {
    static DataManager instance; // Guaranteed to be destroyed, instantiated on first use.
    return instance;
}

void DataManager::logEvent(DataLogEntry::LogLevel level, const std::string& description) {
    std::lock_guard<std::mutex> lock(mtx_); // Ensure thread-safe access
    logs_.emplace_back(level, description);
    // Optionally print to console for immediate feedback
    std::cout << "LOG: " << logs_.back() << std::endl;
}

std::vector<DataLogEntry> DataManager::getAllLogs() const {
    std::lock_guard<std::mutex> lock(mtx_); // Ensure thread-safe access
    return logs_; // Returns a copy of the logs
}

void DataManager::clearLogs() {
    std::lock_guard<std::mutex> lock(mtx_); // Ensure thread-safe access
    logs_.clear();
}