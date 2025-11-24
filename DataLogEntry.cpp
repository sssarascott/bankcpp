#include <map> // For mapping enum to string
#include <chrono> // For timestamp
#include <ctime>   // For time_t
#include <string>
#include <ostream> // For ostream
#include <atomic>  // For unique ID generation
using namespace std;

class DataLogEntry {
    public:
        enum class LogLevel { INFO, WARNING, ERROR };

        DataLogEntry(LogLevel level, const string& description) 
        : entryId_(nextId++), timestamp_(), level_(level), description_(description) {}

        long getEntryId() const {
            return entryId_;
        }
        time_t  getTimestamp() const {
            return timestamp_; 
        }

        DataLogEntry::LogLevel getLevel() const {
            return level_;
        }

        const string& getDescription() const {
            return description_;
        }

        // Overload stream insertion operator for easy printing
        friend ostream& operator<<(ostream& os, const DataLogEntry& entry) {
            map<DataLogEntry::LogLevel, string> logLevelMap = {
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

    private:
        // Initialize the static atomic counter
        static int nextId; // For unique ID generation
        long long entryId_;
        time_t timestamp_;
        LogLevel level_;
        string description_;
};

int DataLogEntry::nextId = 10001; // Initialize static member variable