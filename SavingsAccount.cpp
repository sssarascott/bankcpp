#include "Account.cpp"
#include "DataManager.cpp" // For logging
#include "Transaction.cpp"
using namespace std;

#ifndef SAVINGSACCOUNT
#define SAVINGSACCOUNT
class SavingsAccount : public Account
{
public:
    SavingsAccount(const string &ownerId, double initialBalance, double interestRate)
        : Account(ownerId, initialBalance), interestRate_(interestRate)
    {
        DataManager::getInstance().logEvent(
            DataLogEntry::LogLevel::INFO,
            "SavingsAccount created: " + accountNumber_ + " for owner " + ownerId_ + " with interest rate " + to_string(interestRate_));
    }

    double getInterestRate() const
    {
        return interestRate_;
    }

    void applyInterest()
    { // Applies interest to the account balance
        double interestAmount = balance_ * interestRate_;
        if (deposit(interestAmount, "Monthly Interest"))
        {
            DataManager::getInstance().logEvent(
                DataLogEntry::LogLevel::INFO,
                "Interest of $" + to_string(interestAmount) + " applied to SavingsAccount " + accountNumber_ + ". New balance: $" + to_string(balance_));
        }
        else
        {
            DataManager::getInstance().logEvent(
                DataLogEntry::LogLevel::ERROR,
                "Failed to apply interest to SavingsAccount " + accountNumber_);
        }
    }

    // Override pure virtual function from base class
    void performMonthlyMaintenance() override
    {
        applyInterest();
    }

private:
    double interestRate_; // e.g., 0.01 for 1%
};

#endif // SAVINGSACCOUNT