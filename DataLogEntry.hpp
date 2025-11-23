#ifndef DATALOGENTRY_HPP
#define DATALOGENTRY_HPP

#include <string>
#include <ostream> // For std::ostream
#include <atomic>  // For unique ID generation
#include <chrono> // For timestamp
#include <ctime>   // For std::time_t
#include <map>

class DataLogEntry {
public:
    enum class LogLevel { INFO, WARNING, ERROR };

    DataLogEntry(LogLevel level, const std::string& description);

    long long getEntryId() const;
    std::time_t getTimestamp() const;
    LogLevel getLevel() const;
    const std::string& getDescription() const;

    // Overload stream insertion operator for easy printing
    friend std::ostream& operator<<(std::ostream& os, const DataLogEntry& entry);

private:
    static std::atomic<long long> nextId; // For unique ID generation
    long long entryId_;
    std::time_t timestamp_;
    LogLevel level_;
    std::string description_;
};

#endif // DATALOGENTRY_HPP