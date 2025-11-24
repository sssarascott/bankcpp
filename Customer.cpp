#include <chrono> // For timestamp
#include <ctime>  // For time_t
#include <map>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <atomic>  // For unique ID generation
#include <ostream> // For ostream

#include "DataManager.cpp"    // For logging
#include "Account.cpp"        // For Account class
#include "InvalidAccount.cpp" // For InvalidAccount singleton
using namespace std;

#ifndef CUSTOMER
#define CUSTOMER

class Customer
{

public:
    static Customer &getInvalidCustomer()
    {
        static Customer instance = Customer("-99999", "Invalid", "Invalid", "Invalid"); // Guaranteed to be destroyed, instantiated on first use.
        return instance;
    }

    Customer(const string &name, const string &address, const string &phone)
        : customerId_("CUS" + to_string(nextCustomerId++)),
          name_(name),
          address_(address),
          phone_(phone)
    {
        DataManager::getInstance().logEvent(
            DataLogEntry::LogLevel::INFO,
            "Customer created: " + customerId_ + " (" + name_ + ")");
    }

    string getCustomerId() const
    {
        return customerId_;
    }

    const string &getName() const
    {
        return name_;
    }

    const string &getAddress() const
    {
        return address_;
    }

    const string &getPhone() const
    {
        return phone_;
    }

    void addAccount(Account *account)
    {
        if (*account == *(InvalidAccount::getInstance()))
        {
            DataManager::getInstance().logEvent(
                DataLogEntry::LogLevel::INFO,
                "Account " + account->getAccountNumber() + " added to customer " + customerId_);
            accounts_.push_back(account); // Transfer ownership
        }
        else
        {
            DataManager::getInstance().logEvent(
                DataLogEntry::LogLevel::WARNING,
                "Attempted to add a null account to customer " + customerId_);
        }
    }

    const vector<Account *> getAccounts() const
    {
        return accounts_;
    }

    Account *getAccount(const string &accountNumber) const
    {
        for (Account *acct : accounts_)
        {
            if (acct->getAccountNumber() == accountNumber)
            {
                return acct; // Return raw pointer to the account
            }
        }
        return InvalidAccount::getInstance(); 
    }

    // Overload stream insertion operator for easy printing
    friend ostream &operator<<(ostream &os, const Customer &customer)
    {
        os << "Customer ID: " << customer.getCustomerId() << "\n"
           << "  Name: " << customer.getName() << "\n"
           << "  Address: " << customer.getAddress() << "\n"
           << "  Phone: " << customer.getPhone() << "\n"
           << "  Accounts (" << customer.getAccounts().size() << "): \n";
        for (Account *acc : customer.getAccounts())
        {
            os << "    - " << acc->getAccountNumber() << " (Balance: $" << fixed << setprecision(2) << acc->getBalance() << ")\n";
        }
        return os;
    }

    bool operator==(const Customer &cust) const
    {
        return (this->customerId_ == cust.getCustomerId());
    }

private:
    static int nextCustomerId;
    string customerId_;
    string name_;
    string address_;
    string phone_;
    vector<Account *> accounts_; // Customer owns their accounts

    Customer(const string &customerID, const string &name, const string &address, const string &phone)
        : customerId_(customerID),
          name_(name),
          address_(address),
          phone_(phone) {}
};

// Initialize the static atomic counter
int Customer::nextCustomerId(10000); // Start customer IDs from 10000

#endif // CUSTOMER