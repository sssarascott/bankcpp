#include "CheckingAccount.hpp"
#include "DataManager.hpp" // For logging

CheckingAccount::CheckingAccount(const std::string& ownerId, double initialBalance, double overdraftLimit)
    : Account(ownerId, initialBalance), overdraftLimit_(overdraftLimit) {
    DataManager::getInstance().logEvent(
        DataLogEntry::LogLevel::INFO,
        "CheckingAccount created: " + accountNumber_ + " for owner " + ownerId_ + " with overdraft limit $" + std::to_string(overdraftLimit_)
    );
}

double CheckingAccount::getOverdraftLimit() const {
    return overdraftLimit_;
}

bool CheckingAccount::withdraw(double amount, const std::string& description) {
    if (amount <= 0) {
        DataManager::getInstance().logEvent(
            DataLogEntry::LogLevel::WARNING,
            "Withdrawal failed for " + accountNumber_ + ": Invalid amount $" + std::to_string(amount)
        );
        return false;
    }

    // Check if withdrawal exceeds balance plus overdraft limit
    if (balance_ + overdraftLimit_ < amount) {
        DataManager::getInstance().logEvent(
            DataLogEntry::LogLevel::WARNING,
            "Withdrawal failed for " + accountNumber_ + ": Exceeds overdraft limit. Attempted: $" + std::to_string(amount) +
            ", Available (with overdraft): $" + std::to_string(balance_ + overdraftLimit_)
        );
        return false;
    }

    balance_ -= amount;
    addTransaction(Transaction::Type::WITHDRAWAL, amount, description);
    
    if (balance_ < 0) {
        DataManager::getInstance().logEvent(
            DataLogEntry::LogLevel::WARNING,
            "Overdraft incurred for " + accountNumber_ + ". New balance: $" + std::to_string(balance_)
        );
    } else {
        DataManager::getInstance().logEvent(
            DataLogEntry::LogLevel::INFO,
            "Withdrew $" + std::to_string(amount) + " from " + accountNumber_ + ". New balance: $" + std::to_string(balance_)
        );
    }
    return true;
}

void CheckingAccount::performMonthlyMaintenance() {
    // No specific monthly maintenance for CheckingAccount in this basic model,
    // but this method exists for extensibility (e.g., overdraft fees)
    DataManager::getInstance().logEvent(
        DataLogEntry::LogLevel::INFO,
        "Performed monthly maintenance for CheckingAccount " + accountNumber_
    );
}