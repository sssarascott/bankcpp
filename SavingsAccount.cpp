#include "SavingsAccount.hpp"
#include "DataManager.hpp" // For logging

SavingsAccount::SavingsAccount(const std::string& ownerId, double initialBalance, double interestRate)
    : Account(ownerId, initialBalance), interestRate_(interestRate) {
    DataManager::getInstance().logEvent(
        DataLogEntry::LogLevel::INFO,
        "SavingsAccount created: " + accountNumber_ + " for owner " + ownerId_ + " with interest rate " + std::to_string(interestRate_)
    );
}

double SavingsAccount::getInterestRate() const {
    return interestRate_;
}

void SavingsAccount::applyInterest() {
    double interestAmount = balance_ * interestRate_;
    if (deposit(interestAmount, "Monthly Interest")) {
        DataManager::getInstance().logEvent(
            DataLogEntry::LogLevel::INFO,
            "Interest of $" + std::to_string(interestAmount) + " applied to SavingsAccount " + accountNumber_ + ". New balance: $" + std::to_string(balance_)
        );
    } else {
        DataManager::getInstance().logEvent(
            DataLogEntry::LogLevel::ERROR,
            "Failed to apply interest to SavingsAccount " + accountNumber_
        );
    }
}

void SavingsAccount::performMonthlyMaintenance() {
    applyInterest();
}