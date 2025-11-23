#ifndef CUSTOMER_HPP
#define CUSTOMER_HPP

#include "Account.hpp"
#include <string>
#include <vector>
#include <memory> // For std::unique_ptr
#include <atomic> // For unique ID generation
#include <ostream> // For std::ostream

class Customer {
public:
    Customer(const std::string& name, const std::string& address, const std::string& phone);

    std::string getCustomerId() const;
    const std::string& getName() const;
    const std::string& getAddress() const;
    const std::string& getPhone() const;

    void addAccount(std::unique_ptr<Account> account); // Takes ownership
    const std::vector<std::unique_ptr<Account>>& getAccounts() const;
    Account* getAccount(const std::string& accountNumber) const; // Raw pointer for lookup

    // Overload stream insertion operator for easy printing
    friend std::ostream& operator<<(std::ostream& os, const Customer& customer);

private:
    static std::atomic<long long> nextCustomerId;
    std::string customerId_;
    std::string name_;
    std::string address_;
    std::string phone_;
    std::vector<std::unique_ptr<Account>> accounts_; // Customer owns their accounts
};

#endif // CUSTOMER_HPP