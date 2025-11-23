#ifndef BANK_HPP
#define BANK_HPP

#include "Customer.hpp"
#include "Account.hpp"
#include <string>
#include <vector>
#include <memory>  // For std::unique_ptr
#include <algorithm> // For std::find_if
#include <numeric>   // For std::accumulate

class Bank {
public:
    Bank(const std::string& name);

    const std::string& getName() const;

    // Customer Management
    Customer* createCustomer(const std::string& name, const std::string& address, const std::string& phone);
    Customer* getCustomer(const std::string& customerId) const;
    Customer* getCustomerByName(const std::string& name) const;
    std::vector<Customer*> getAllCustomers() const; // Returns raw pointers to managed customers

    // Account Management
    Account* createSavingsAccount(const std::string& customerId, double initialBalance, double interestRate);
    Account* createCheckingAccount(const std::string& customerId, double initialBalance, double overdraftLimit);
    Account* getAccount(const std::string& accountNumber) const; // Finds account across all customers
    std::vector<Account*> getAllAccounts() const; // Returns raw pointers to managed accounts

    // Transaction Processing
    bool transferFunds(const std::string& fromAccountNum, const std::string& toAccountNum, double amount, const std::string& description = "");
    
    // System Operations
    void runMonthlyMaintenance();

private:
    std::string name_;
    std::vector<std::unique_ptr<Customer>> customers_; // Bank owns its customers
};

#endif // BANK_HPP