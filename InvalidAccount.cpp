#include "DataManager.cpp" // For logging
#include "Account.cpp"
#include "Transaction.cpp" // For adding transactions
using namespace std;

#ifndef INVALIDACCOUNT
#define INVALIDACCOUNT

class InvalidAccount : public Account
{
private:
    InvalidAccount()
        : Account("Invalid", 0)
    {
        accountNumber_ = "-9999"; // Special invalid account number
    }

public:
    static Account* getInstance()
    {
        static InvalidAccount instance = InvalidAccount(); // Guaranteed to be destroyed, instantiated on first use.
        static Account account = (Account)instance;
        return &account;
    }

    // Override pure virtual function from base class
    void performMonthlyMaintenance() override
    {
        // No specific monthly maintenance for InvalidAccount
    }
};

#endif // INVALIDACCOUNT