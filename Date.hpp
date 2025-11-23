#ifndef DATE_HPP
#define DATE_HPP

#include <chrono>
#include <string>
#include <ostream> // For std::ostream

// A simple wrapper for std::chrono::system_clock::time_point
// to represent a "LocalDateTime" and provide easy formatting.
class Date {
public:
    Date(); // Defaults to now
    explicit Date(std::chrono::system_clock::time_point tp);

    std::chrono::system_clock::time_point getTimePoint() const;
    std::string toString() const; // Format as YYYY-MM-DD HH:MM:SS

    // Overload the stream insertion operator for easy printing
    friend std::ostream& operator<<(std::ostream& os, const Date& date);

private:
    std::chrono::system_clock::time_point timePoint_;
};

#endif // DATE_HPP