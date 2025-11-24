#include "DataManager.cpp" // For logging
#include "Account.cpp"
#include "Transaction.cpp"  // For adding transactions
using namespace std;
 
class InvalidAccount : public Account {
private:
    InvalidAccount()
        : Account("Invalid", 0) {
            accountNumber_ = "-9999"; // Special invalid account number
    }

    public:
        static InvalidAccount getInstance() {
            static InvalidAccount instance = InvalidAccount(); // Guaranteed to be destroyed, instantiated on first use.
            return instance;
        }

    // Override pure virtual function from base class
    void performMonthlyMaintenance() override{
        // No specific monthly maintenance for InvalidAccount
    }
};

