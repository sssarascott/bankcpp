#include "Customer.hpp"
#include "DataManager.hpp" // For logging
#include <chrono> // For timestamp
#include <ctime>   // For std::time_t
#include <map>
#include <iomanip>
#include <iostream>

// Initialize the static atomic counter
std::atomic<long long> Customer::nextCustomerId(10000); // Start customer IDs from 10000

Customer::Customer(const std::string& name, const std::string& address, const std::string& phone)
    : customerId_("CUS" + std::to_string(nextCustomerId++)),
      name_(name),
      address_(address),
      phone_(phone) {
    DataManager::getInstance().logEvent(
        DataLogEntry::LogLevel::INFO,
        "Customer created: " + customerId_ + " (" + name_ + ")"
    );
}

std::string Customer::getCustomerId() const {
    return customerId_;
}

const std::string& Customer::getName() const {
    return name_;
}

const std::string& Customer::getAddress() const {
    return address_;
}

const std::string& Customer::getPhone() const {
    return phone_;
}

void Customer::addAccount(std::unique_ptr<Account> account) {
    if (account) {
        DataManager::getInstance().logEvent(
            DataLogEntry::LogLevel::INFO,
            "Account " + account->getAccountNumber() + " added to customer " + customerId_
        );
        accounts_.push_back(std::move(account)); // Transfer ownership
    } else {
        DataManager::getInstance().logEvent(
            DataLogEntry::LogLevel::WARNING,
            "Attempted to add a null account to customer " + customerId_
        );
    }
}

const std::vector<std::unique_ptr<Account>>& Customer::getAccounts() const {
    return accounts_; 
}

Account* Customer::getAccount(const std::string& accountNumber) const {
    for (const auto& acc : accounts_) {
        if (acc->getAccountNumber() == accountNumber) {
            return acc.get(); // Return raw pointer to the account
        }
    }
    return nullptr; // Account not found
}

std::ostream& operator<<(std::ostream& os, const Customer& customer) {
    os << "Customer ID: " << customer.getCustomerId() << "\n"
       << "  Name: " << customer.getName() << "\n"
       << "  Address: " << customer.getAddress() << "\n"
       << "  Phone: " << customer.getPhone() << "\n"
       << "  Accounts (" << customer.getAccounts().size() << "): \n";
    for (const auto& acc : customer.getAccounts()) {
        os << "    - " << acc->getAccountNumber() << " (Balance: $" << std::fixed << std::setprecision(2) << acc->getBalance() << ")\n";
    }
    return os;
}