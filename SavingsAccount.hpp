#ifndef SAVINGSACCOUNT_HPP
#define SAVINGSACCOUNT_HPP

#include "Account.hpp"

class SavingsAccount : public Account {
public:
    SavingsAccount(const std::string& ownerId, double initialBalance, double interestRate);

    double getInterestRate() const;
    void applyInterest(); // Applies interest to the account balance

    // Override pure virtual function from base class
    void performMonthlyMaintenance() override;

private:
    double interestRate_; // e.g., 0.01 for 1% 
};

#endif // SAVINGSACCOUNT_HPP