#include <map>    // For mapping enum to string
#include <chrono> // For timestamp
#include <ctime>  // For time_t
#include <map>
#include <iostream>
#include <iomanip>
#include <string>
#include <ostream> // For ostream
#include <atomic>  // For unique ID generation
using namespace std;

#ifndef TRANSACTION
#define TRANSACTION
class Transaction
{
public:
    enum class Type
    {
        DEPOSIT,
        WITHDRAWAL,
        TRANSFER
    };

    Transaction(Type type, double amount, const string &accountNum, const string &description = "")
        : transactionId_(nextId++), timestamp_(), type_(type), amount_(amount),
          accountNumber_(accountNum), description_(description) {}

    long getTransactionId() const
    {
        return transactionId_;
    }

    time_t getTimestamp() const
    {
        return timestamp_;
    }

    Type getType() const
    {
        return type_;
    }

    double getAmount() const
    {
        return amount_;
    }

    const string &getAccountNumber() const
    {
        return accountNumber_;
    }

    const string &getDescription() const
    {
        return description_;
    }

    // Overload stream insertion operator for easy printing
    friend ostream &operator<<(ostream &os, const Transaction &transaction)
    {
        map<Type, string> typeMap = {
            {Type::DEPOSIT, "DEPOSIT"},
            {Type::WITHDRAWAL, "WITHDRAWAL"},
            {Type::TRANSFER, "TRANSFER"}};

        os << "[TRXID:" << transaction.getTransactionId() << "] "
           << "[" << transaction.getTimestamp() << "] "
           << "[" << typeMap[transaction.getType()] << "] "
           << "Account: " << transaction.getAccountNumber() << ", "
           << "Amount: $" << fixed << setprecision(2) << transaction.getAmount() << " "
           << (transaction.getDescription().empty() ? "" : "(" + transaction.getDescription() + ")");
        return os;
    }

private:
    static int nextId;
    long transactionId_;
    time_t timestamp_;
    Type type_;
    double amount_;
    string accountNumber_; // Account associated with this transaction
    string description_;
};

int Transaction::nextId = 1001;

#endif // TRANSACTION