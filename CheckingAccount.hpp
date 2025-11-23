#ifndef CHECKINGACCOUNT_HPP
#define CHECKINGACCOUNT_HPP

#include "Account.hpp"
 
class CheckingAccount : public Account {
public:
    CheckingAccount(const std::string& ownerId, double initialBalance, double overdraftLimit);

    double getOverdraftLimit() const;
    
    // Override base class withdraw for overdraft logic
    bool withdraw(double amount, const std::string& description = "") override;

    // Override pure virtual function from base class
    void performMonthlyMaintenance() override;

private:
    double overdraftLimit_;
};

#endif // CHECKINGACCOUNT_HPP