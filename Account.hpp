#ifndef ACCOUNT_HPP
#define ACCOUNT_HPP

#include "Transaction.hpp"
#include <string>
#include <vector>
#include <memory>  // For std::unique_ptr
#include <atomic>  // For unique ID generation
#include <ostream> // For std::ostream

class Account {
public:
    // Pure virtual destructor to make Account an abstract base class
    virtual ~Account() = default;

    std::string getAccountNumber() const;
    double getBalance() const;
    const std::string& getOwnerId() const;
    const std::vector<Transaction>& getTransactionHistory() const;

    virtual bool deposit(double amount, const std::string& description = "");
    virtual bool withdraw(double amount, const std::string& description = "");

    // Pure virtual function for account-specific operations (e.g., interest, overdraft)
    virtual void performMonthlyMaintenance() = 0;

    // Overload stream insertion operator for easy printing
    friend std::ostream& operator<<(std::ostream& os, const Account& account);

protected: // Accessible by derived classes
    Account(const std::string& ownerId, double initialBalance = 0.0);

    // Protected members
    static std::atomic<long long> nextAccountNumber;
    std::string accountNumber_;
    std::string ownerId_; // ID of the customer who owns this account
    double balance_;
    std::vector<Transaction> transactionHistory_;

    // Helper to add transaction and log it
    void addTransaction(Transaction::Type type, double amount, const std::string& description);
};

#endif // ACCOUNT_HPP