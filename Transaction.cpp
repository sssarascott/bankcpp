#include "Transaction.hpp"
#include <map> // For mapping enum to string

// Initialize the static atomic counter
std::atomic<long long> Transaction::nextId(1001); // Start transaction IDs from 1001

Transaction::Transaction(Type type, double amount, const std::string& accountNum, const std::string& description)
    : transactionId_(nextId++), timestamp_(), type_(type), amount_(amount),
      accountNumber_(accountNum), description_(description) {}

long long Transaction::getTransactionId() const {
    return transactionId_;
}

Date Transaction::getTimestamp() const {
    return timestamp_;
}

Transaction::Type Transaction::getType() const {
    return type_;
}

double Transaction::getAmount() const {
    return amount_;
}

const std::string& Transaction::getAccountNumber() const {
    return accountNumber_;
}

const std::string& Transaction::getDescription() const {
    return description_;
}

std::ostream& operator<<(std::ostream& os, const Transaction& transaction) {
    std::map<Transaction::Type, std::string> typeMap = {
        {Transaction::Type::DEPOSIT, "DEPOSIT"},
        {Transaction::Type::WITHDRAWAL, "WITHDRAWAL"},
        {Transaction::Type::TRANSFER, "TRANSFER"}
    };

    os << "[TRXID:" << transaction.getTransactionId() << "] "
       << "[" << transaction.getTimestamp() << "] "
       << "[" << typeMap[transaction.getType()] << "] "
       << "Account: " << transaction.getAccountNumber() << ", "
       << "Amount: $" << std::fixed << std::setprecision(2) << transaction.getAmount() << " "
       << (transaction.getDescription().empty() ? "" : "(" + transaction.getDescription() + ")");
    return os;
}