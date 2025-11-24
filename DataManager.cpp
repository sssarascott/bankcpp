#include <iostream>
#include <vector>
#include <string>
#include "DataLogEntry.cpp"
using namespace std;

#ifndef DATAMANAGER
#define DATAMANAGER

// DataManager will be a Singleton to ensure consistent logging
class DataManager
{
private:
    vector<DataLogEntry> logs_;
    DataManager() {}

public:
    static DataManager &getInstance()
    {
        static DataManager instance; // Guaranteed to be destroyed, instantiated on first use.
        return instance;
    }

    void logEvent(DataLogEntry::LogLevel level, const string &description)
    {
        logs_.emplace_back(level, description);
        // Optionally print to console for immediate feedback
        cout << "LOG: " << logs_.back() << endl;
    }

    vector<DataLogEntry> getAllLogs() const
    {
        return logs_; // Returns a copy of the logs
    }

    void clearLogs()
    {
        logs_.clear();
    }
};

#endif // DATAMANAGER