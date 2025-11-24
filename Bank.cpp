#include "DataManager.cpp" // For logging
#include "SavingsAccount.cpp" // For SavingsAccount
#include "CheckingAccount.cpp" // For CheckingAccount
#include "Customer.cpp" // For Customer class
#include "Account.cpp" // For Account class
#include <iostream>
#include <string>
#include <vector>
#include <algorithm> // For find_if
#include <numeric>   // For accumulate
using namespace std;

class Bank {
public:
    Bank::Bank(const string& name) : name_(name) {
        DataManager::getInstance().logEvent(
            DataLogEntry::LogLevel::INFO,
            "Bank '" + name_ + "' initialized."
        );
    }

    const string& Bank::getName() const {
        return name_;
    }

    // --- Customer Management ---
    Customer createCustomer(const string& name, const string& address, const string& phone) {
        const Customer newCustomer = Customer(name, address, phone);
        customers_.push_back(newCustomer);
        DataManager::getInstance().logEvent(
            DataLogEntry::LogLevel::INFO,
            "New customer registered: " + newCustomer.getCustomerId() + " (" + newCustomer.getName() + ")"
        );
        return newCustomer;
    }

    Customer getCustomer(const string& customerId) const {
        for (Customer cust : customers_) {
            if (cust.getCustomerId() == customerId) {
                return cust;
            }
        }
        return Customer::getInvalidCustomer();
    }

    Customer getCustomerByName(const string& name) const {
        for (Customer cust : customers_) {
            if (cust.getName().compare(name) == 0) {
                return cust;
            }
        }
        return Customer::getInvalidCustomer();
    }

    vector<Customer> getAllCustomers() const {
        return customers_;
    }

    // --- Account Management ---
    Account& createSavingsAccount(const string& customerId, double initialBalance, double interestRate) {
        Customer& customer = getCustomer(customerId);
        if (customer == Customer::getInvalidCustomer()) {
            DataManager::getInstance().logEvent(
                DataLogEntry::LogLevel::ERROR,
                "Failed to create SavingsAccount: Customer " + customerId + " not found."
            );
            throw runtime_error("Customer not found");
        }
        Account* newAccount = new SavingsAccount(customerId, initialBalance, interestRate);
        customer.addAccount (newAccount);
        return newAccount;
    }

    Account& createCheckingAccount(const string& customerId, double initialBalance, double overdraftLimit) {
        Customer customer = getCustomer(customerId);
        if (customer == Customer::getInvalidCustomer()) {
            DataManager::getInstance().logEvent(
                DataLogEntry::LogLevel::ERROR,
                "Failed to create CheckingAccount: Customer " + customerId + " not found."
            );
            throw runtime_error("Customer not found");
        }
        CheckingAccount newAccount = CheckingAccount(customerId, initialBalance, overdraftLimit);
        customer.addAccount((Account*) &newAccount);
        return newAccount;
    }

    Account* getAccount(const string& accountNumber) const {
        for (Customer cust : customers_) {
            Account* acct = cust.getAccount(accountNumber);
            if (acct != nullptr) {
                if (acct->getAccountNumber() == accountNumber) {
                    return acct;
                }
            }
        }
        return nullptr;
    }

    vector<Account*> getAllAccounts() const {
        vector<Account*> allAccounts;
        for (Customer cust : customers_) {
            for (Account* acct : cust.getAccounts()) {
                allAccounts.push_back(acct);
            }
        }
        return allAccounts;
    }

    // --- Transaction Processing ---
    bool transferFunds(const string& fromAccountNum, const string& toAccountNum, double amount, const string& description) {
        if (amount <= 0) {
            DataManager::getInstance().logEvent(
                DataLogEntry::LogLevel::WARNING,
                "Transfer failed: Invalid amount $" + to_string(amount)
            );
            return false;
        }

        Account* fromAccount = getAccount(fromAccountNum);
        Account* toAccount = getAccount(toAccountNum);

        if (*fromAccount == InvalidAccount::getInstance()) {
            DataManager::getInstance().logEvent(
                DataLogEntry::LogLevel::ERROR,
                "Transfer failed: Source account " + fromAccountNum + " not found."
            );
            return false;
        }
        if (*toAccount == InvalidAccount::getInstance()) {
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
                "Successfully transferred $" + to_string(amount) + " from " + fromAccountNum + " to " + toAccountNum + (description.empty() ? "" : " (" + description + ")")
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
    void runMonthlyMaintenance() {
        DataManager::getInstance().logEvent(
            DataLogEntry::LogLevel::INFO,
            "Starting monthly maintenance for all accounts..."
        );
        for (Customer cust : customers_) {
            for (Account* acct : cust.getAccounts()) {
                acct->performMonthlyMaintenance();
            }
        }
        DataManager::getInstance().logEvent(
            DataLogEntry::LogLevel::INFO,
            "Monthly maintenance completed."
        );
    }

private:
    string name_;
    vector<Customer> customers_; // Bank owns its customers
};