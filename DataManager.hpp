#ifndef DATAMANAGER_HPP
#define DATAMANAGER_HPP

#include "DataLogEntry.hpp"
#include <vector>
#include <string>
// #include <mutex> // For thread-safety (Singleton pattern)

// DataManager will be a Singleton to ensure consistent logging
class DataManager {
public:
    // Deleted copy constructor and assignment operator to enforce Singleton
    DataManager(const DataManager&) = delete;
    DataManager& operator=(const DataManager&) = delete;

    static DataManager& getInstance();

    void logEvent(DataLogEntry::LogLevel level, const std::string& description);
    std::vector<DataLogEntry> getAllLogs() const;
    void clearLogs();

private:
    DataManager(); // Private constructor for Singleton pattern
    
    std::vector<DataLogEntry> logs_;
    // mutable std::mutex mtx_; // Protects access to logs_
};

#endif // DATAMANAGER_HPP