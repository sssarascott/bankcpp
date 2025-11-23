#include "Account.hpp"
#include "DataManager.hpp" // For logging
#include <stdexcept>   // For std::runtime_error
#include <iomanip>     // For std::fixed, std::setprecision

// Initialize the static atomic counter for account numbers
std::atomic<long long> Account::nextAccountNumber(200000); // Start account numbers from 200000

Account::Account(const std::string& ownerId, double initialBalance)
    : accountNumber_("ACC" + std::to_string(nextAccountNumber++)),
      ownerId_(ownerId),
      balance_(initialBalance) {
    
    DataManager::getInstance().logEvent(
        DataLogEntry::LogLevel::INFO,
        "Account created: " + accountNumber_ + " for owner " + ownerId_ + " with initial balance $" + std::to_string(initialBalance)
    );
}

std::string Account::getAccountNumber() const {
    return accountNumber_;
}

double Account::getBalance() const {
    return balance_;
}

const std::string& Account::getOwnerId() const {
    return ownerId_;
}

const std::vector<Transaction>& Account::getTransactionHistory() const {
    return transactionHistory_;
}

bool Account::deposit(double amount, const std::string& description) {
    if (amount <= 0) {
        DataManager::getInstance().logEvent(
            DataLogEntry::LogLevel::WARNING,
            "Deposit failed for " + accountNumber_ + ": Invalid amount $" + std::to_string(amount)
        );
        return false;
    }
    balance_ += amount;
    addTransaction(Transaction::Type::DEPOSIT, amount, description);
    DataManager::getInstance().logEvent(
        DataLogEntry::LogLevel::INFO,
        "Deposited $" + std::to_string(amount) + " into " + accountNumber_ + ". New balance: $" + std::to_string(balance_)
    );
    return true;
}

bool Account::withdraw(double amount, const std::string& description) {
    if (amount <= 0 || amount > balance_) {
        DataManager::getInstance().logEvent(
            DataLogEntry::LogLevel::WARNING,
            "Withdrawal failed for " + accountNumber_ + ": Invalid amount $" + std::to_string(amount) + " or insufficient funds. Current balance: $" + std::to_string(balance_)
        );
        return false;
    }
    balance_ -= amount;
    addTransaction(Transaction::Type::WITHDRAWAL, amount, description);
    DataManager::getInstance().logEvent(
        DataLogEntry::LogLevel::INFO,
        "Withdrew $" + std::to_string(amount) + " from " + accountNumber_ + ". New balance: $" + std::to_string(balance_)
    );
    return true;
}

void Account::addTransaction(Transaction::Type type, double amount, const std::string& description) {
    transactionHistory_.emplace_back(type, amount, accountNumber_, description);
}

std::ostream& operator<<(std::ostream& os, const Account& account) {
    os << "Account Number: " << account.getAccountNumber() << "\n"
       << "  Owner ID: " << account.getOwnerId() << "\n"
       << "  Balance: $" << std::fixed << std::setprecision(2) << account.getBalance() << "\n";
    return os;
}