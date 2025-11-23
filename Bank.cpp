#include "Bank.hpp"
#include "DataManager.hpp" // For logging
#include "SavingsAccount.hpp"
#include "CheckingAccount.hpp"
#include <iostream>

Bank::Bank(const std::string& name) : name_(name) {
    DataManager::getInstance().logEvent(
        DataLogEntry::LogLevel::INFO,
        "Bank '" + name_ + "' initialized."
    );
}

const std::string& Bank::getName() const {
    return name_;
}

// --- Customer Management ---

Customer* Bank::createCustomer(const std::string& name, const std::string& address, const std::string& phone) {
    auto newCustomer = std::make_unique<Customer>(name, address, phone);
    Customer* rawPtr = newCustomer.get();
    customers_.push_back(std::move(newCustomer));
    DataManager::getInstance().logEvent(
        DataLogEntry::LogLevel::INFO,
        "New customer registered: " + rawPtr->getCustomerId() + " (" + rawPtr->getName() + ")"
    );
    return rawPtr;
}

Customer* Bank::getCustomer(const std::string& customerId) const {
    for (const auto& cust : customers_) {
        if (cust->getCustomerId() == customerId) {
            return cust.get();
        }
    }
    return nullptr;
}

Customer* Bank::getCustomerByName(const std::string& name) const {
    for (const auto& cust : customers_) {
        if (cust->getName() == name) {
            return cust.get();
        }
    }
    return nullptr;
}

std::vector<Customer*> Bank::getAllCustomers() const {
    std::vector<Customer*> allCustomers;
    for (const auto& cust : customers_) {
        allCustomers.push_back(cust.get());
    }
    return allCustomers;
}

// --- Account Management ---

Account* Bank::createSavingsAccount(const std::string& customerId, double initialBalance, double interestRate) {
    Customer* customer = getCustomer(customerId);
    if (!customer) {
        DataManager::getInstance().logEvent(
            DataLogEntry::LogLevel::ERROR,
            "Failed to create SavingsAccount: Customer " + customerId + " not found."
        );
        return nullptr;
    }
    auto newAccount = std::make_unique<SavingsAccount>(customerId, initialBalance, interestRate);
    Account* rawPtr = newAccount.get();
    customer->addAccount(std::move(newAccount));
    return rawPtr;
}

Account* Bank::createCheckingAccount(const std::string& customerId, double initialBalance, double overdraftLimit) {
    Customer* customer = getCustomer(customerId);
    if (!customer) {
        DataManager::getInstance().logEvent(
            DataLogEntry::LogLevel::ERROR,
            "Failed to create CheckingAccount: Customer " + customerId + " not found."
        );
        return nullptr;
    }
    auto newAccount = std::make_unique<CheckingAccount>(customerId, initialBalance, overdraftLimit);
    Account* rawPtr = newAccount.get();
    customer->addAccount(std::move(newAccount));
    return rawPtr;
}

Account* Bank::getAccount(const std::string& accountNumber) const {
    for (const auto& cust : customers_) {
        Account* acc = cust->getAccount(accountNumber);
        if (acc) {
            return acc;
        }
    }
    return nullptr;
}

std::vector<Account*> Bank::getAllAccounts() const {
    std::vector<Account*> allAccounts;
    for (const auto& cust : customers_) {
        for (const auto& acc : cust->getAccounts()) {
            allAccounts.push_back(acc.get());
        }
    }
    return allAccounts;
}


// --- Transaction Processing ---

bool Bank::transferFunds(const std::string& fromAccountNum, const std::string& toAccountNum, double amount, const std::string& description) {
    if (amount <= 0) {
        DataManager::getInstance().logEvent(
            DataLogEntry::LogLevel::WARNING,
            "Transfer failed: Invalid amount $" + std::to_string(amount)
        );
        return false;
    }

    Account* fromAccount = getAccount(fromAccountNum);
    Account* toAccount = getAccount(toAccountNum);

    if (!fromAccount) {
        DataManager::getInstance().logEvent(
            DataLogEntry::LogLevel::ERROR,
            "Transfer failed: Source account " + fromAccountNum + " not found."
        );
        return false;
    }
    if (!toAccount) {
        DataManager::getInstance().logEvent(
            DataLogEntry::LogLevel::ERROR,
            "Transfer failed: Destination account " + toAccountNum + " not found."
        );
        return false;
    }

    if (fromAccount->withdraw(amount, "Transfer to " + toAccountNum + (description.empty() ? "" : ": " + description))) {
        toAccount->deposit(amount, "Transfer from " + fromAccountNum + (description.empty() ? "" : ": " + description));
        DataManager::getInstance().logEvent(
            DataLogEntry::LogLevel::INFO,
            "Successfully transferred $" + std::to_string(amount) + " from " + fromAccountNum + " to " + toAccountNum + (description.empty() ? "" : " (" + description + ")")
        );
        return true;
    } else {
        DataManager::getInstance().logEvent(
            DataLogEntry::LogLevel::WARNING,
            "Transfer failed between " + fromAccountNum + " and " + toAccountNum + " due to withdrawal issue."
        );
        return false; 
    }
}

// --- System Operations ---

void Bank::runMonthlyMaintenance() {
    DataManager::getInstance().logEvent(
        DataLogEntry::LogLevel::INFO,
        "Starting monthly maintenance for all accounts..."
    );
    for (auto& cust : customers_) {
        for (auto& acc : cust->getAccounts()) {
            acc->performMonthlyMaintenance();
        }
    }
    DataManager::getInstance().logEvent(
        DataLogEntry::LogLevel::INFO,
        "Monthly maintenance completed."
    );
}