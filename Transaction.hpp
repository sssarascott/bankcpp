#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP

// #include "Date.hpp"
#include <string>
#include <ostream> // For std::ostream
#include <atomic>  // For unique ID generation
#include <chrono> // For timestamp
#include <ctime>   // For std::time_t
#include <map>

class Transaction {
public:
    enum class Type { DEPOSIT, WITHDRAWAL, TRANSFER };

    Transaction(Type type, double amount, const std::string& accountNum, const std::string& description = "");

    long long getTransactionId() const;
    std::time_t getTimestamp() const;
    Type getType() const;
    double getAmount() const;
    const std::string& getAccountNumber() const;
    const std::string& getDescription() const;

    // Overload stream insertion operator for easy printing
    friend std::ostream& operator<<(std::ostream& os, const Transaction& transaction);

private:
    static std::atomic<long long> nextId;
    long long transactionId_;
    std::time_t timestamp_;
    Type type_;
    double amount_;
    std::string accountNumber_; // Account associated with this transaction
    std::string description_;
};

#endif // TRANSACTION_HPP