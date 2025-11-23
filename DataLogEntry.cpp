#include "DataLogEntry.hpp"
#include <map> // For mapping enum to string

// Initialize the static atomic counter
std::atomic<long long> DataLogEntry::nextId(1);

DataLogEntry::DataLogEntry(LogLevel level, const std::string& description)
    : entryId_(nextId++), timestamp_(), level_(level), description_(description) {}

long long DataLogEntry::getEntryId() const {
    return entryId_;
}

Date DataLogEntry::getTimestamp() const {
    return timestamp_;
}

DataLogEntry::LogLevel DataLogEntry::getLevel() const {
    return level_;
}

const std::string& DataLogEntry::getDescription() const {
    return description_;
}

std::ostream& operator<<(std::ostream& os, const DataLogEntry& entry) {
    std::map<DataLogEntry::LogLevel, std::string> logLevelMap = {
        {DataLogEntry::LogLevel::INFO, "INFO"},
        {DataLogEntry::LogLevel::WARNING, "WARNING"},
        {DataLogEntry::LogLevel::ERROR, "ERROR"}
    };

    os << "[" << entry.getTimestamp() << "] "
       << "[" << logLevelMap[entry.getLevel()] << "] "
       << "(ID:" << entry.getEntryId() << ") "
       << entry.getDescription();
    return os;
}