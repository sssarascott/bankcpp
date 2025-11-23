#include "Date.hpp"
#include <iomanip> // For std::put_time
#include <sstream> // For std::stringstream
#include <ctime>   // For std::localtime, std::mktime

Date::Date() : timePoint_(std::chrono::system_clock::now()) {}

Date::Date(std::chrono::system_clock::time_point tp) : timePoint_(tp) {}

std::chrono::system_clock::time_point Date::getTimePoint() const {
    return timePoint_;
}

std::string Date::toString() const {
    std::time_t tt = std::chrono::system_clock::to_time_t(timePoint_);
    std::tm tm_buf;
    
    // Use thread-safe localtime_r on POSIX or localtime_s on Windows,
    // otherwise fallback to localtime (not thread-safe)
#ifdef _MSC_VER
    localtime_s(&tm_buf, &tt);
#else
    localtime_r(&tt, &tm_buf);
#endif

    std::stringstream ss;
    ss << std::put_time(&tm_buf, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

std::ostream& operator<<(std::ostream& os, const Date& date) {
    os << date.toString();
    return os;
}