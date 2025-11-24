#include <stdexcept>   // For runtime_error
#include <iomanip>     // For fixed, setprecision
#include <string>
#include <vector>
#include <atomic>  // For unique ID generation
#include <ostream> // For ostream
#include "Transaction.cpp"  // For Transaction class
#include "DataManager.cpp" // For logging 
using namespace std;

class Account {
public:
    string getAccountNumber() const {
        return accountNumber_;
    }

    double getBalance() const {
        return balance_;
    }

    const string& getOwnerId() const {
        return ownerId_;
    }

    const vector<Transaction>& getTransactionHistory() const {
        return transactionHistory_;
    }

    virtual bool deposit(double amount, const string& description) {
        if (amount <= 0) {
            DataManager::getInstance().logEvent(
                DataLogEntry::LogLevel::WARNING,
                "Deposit failed for " + accountNumber_ + ": Invalid amount $" + to_string(amount)
            );
            return false;
        }
        balance_ += amount;
        addTransaction(Transaction::Type::DEPOSIT, amount, description);
        DataManager::getInstance().logEvent(
            DataLogEntry::LogLevel::INFO,
            "Deposited $" + to_string(amount) + " into " + accountNumber_ + ". New balance: $" + to_string(balance_)
        );
        return true;
    }

    virtual bool withdraw(double amount, const string& description) {
        if (amount <= 0 || amount > balance_) {
            DataManager::getInstance().logEvent(
                DataLogEntry::LogLevel::WARNING,
                "Withdrawal failed for " + accountNumber_ + ": Invalid amount $" + to_string(amount) + " or insufficient funds. Current balance: $" + to_string(balance_)
            );
            return false;
        }
        balance_ -= amount;
        addTransaction(Transaction::Type::WITHDRAWAL, amount, description);
        DataManager::getInstance().logEvent(
            DataLogEntry::LogLevel::INFO,
            "Withdrew $" + to_string(amount) + " from " + accountNumber_ + ". New balance: $" + to_string(balance_)
        );
        return true; 
    }

    virtual void performMonthlyMaintenance() {};

    // Overload stream insertion operator for easy printing
    friend ostream& operator<<(ostream& os, const Account& account) {
        os << "Account Number: " << account.getAccountNumber() << "\n"
        << "  Owner ID: " << account.getOwnerId() << "\n"
        << "  Balance: $" << fixed << setprecision(2) << account.getBalance() << "\n";
        return os;
    }

    Account(const string& ownerId, double initialBalance = 0.0)
        : accountNumber_("ACC" + to_string(nextAccountNumber++)),
        ownerId_(ownerId),
        balance_(initialBalance) {
        
        DataManager::getInstance().logEvent(
            DataLogEntry::LogLevel::INFO,
            "Account created: " + accountNumber_ + " for owner " + ownerId_ + " with initial balance $" + to_string(initialBalance)
        );
    }

    bool operator == (const Account& acct) const {
        return (this->accountNumber_ == acct.getAccountNumber());
        // return this->ownerId_._Equal(acct.getOwnerId()) && this->accountNumber_._Equal(acct.getAccountNumber());
    }

protected:
    // Protected members
    static int nextAccountNumber; 
    string accountNumber_;
    string ownerId_; // ID of the customer who owns this account
    double balance_;
    vector<Transaction> transactionHistory_;

    // Helper to add transaction and log it
    void addTransaction(Transaction::Type type, double amount, const string& description) {
        Transaction trans = Transaction(type, amount, accountNumber_, description);
        transactionHistory_.push_back(trans);
    }
};

int Account::nextAccountNumber = 20001; 