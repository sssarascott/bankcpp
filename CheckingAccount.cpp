#include "DataManager.cpp" // For logging
#include "Account.cpp"
// #include "Transaction.cpp"  // For adding transactions
using namespace std;
 
class CheckingAccount : public Account {
public:
    CheckingAccount(const string& ownerId, double initialBalance, double overdraftLimit)
        : Account(ownerId, initialBalance), overdraftLimit_(overdraftLimit) {
        DataManager::getInstance().logEvent(
            DataLogEntry::LogLevel::INFO,
            "CheckingAccount created: " + accountNumber_ + " for owner " + ownerId_ + " with overdraft limit $" + to_string(overdraftLimit_)
        );
    }

    double getOverdraftLimit() const {
        return overdraftLimit_;
    }

    // Override base class withdraw for overdraft logic
    bool withdraw(double amount, const string& description) override {
        if (amount <= 0) {
            DataManager::getInstance().logEvent(
                DataLogEntry::LogLevel::WARNING,
                "Withdrawal failed for " + accountNumber_ + ": Invalid amount $" + to_string(amount)
            );
            return false;
        }

        // Check if withdrawal exceeds balance plus overdraft limit
        if (balance_ + overdraftLimit_ < amount) {
            DataManager::getInstance().logEvent(
                DataLogEntry::LogLevel::WARNING,
                "Withdrawal failed for " + accountNumber_ + ": Exceeds overdraft limit. Attempted: $" + to_string(amount) +
                ", Available (with overdraft): $" + to_string(balance_ + overdraftLimit_)
            );
            return false;
        }
    
        balance_ -= amount;
        addTransaction(Transaction::Type::WITHDRAWAL, amount, description);
        
        if (balance_ < 0) {
            DataManager::getInstance().logEvent(
                DataLogEntry::LogLevel::WARNING,
                "Overdraft incurred for " + accountNumber_ + ". New balance: $" + to_string(balance_)
            );
        } else {
            DataManager::getInstance().logEvent(
                DataLogEntry::LogLevel::INFO,
                "Withdrew $" + to_string(amount) + " from " + accountNumber_ + ". New balance: $" + to_string(balance_)
            );
        }
        return true;
    }

    // Override pure virtual function from base class
    void performMonthlyMaintenance() override{
        // No specific monthly maintenance for CheckingAccount in this basic model,
        // but this method exists for extensibility (e.g., overdraft fees)
        DataManager::getInstance().logEvent(
            DataLogEntry::LogLevel::INFO,
            "Performed monthly maintenance for CheckingAccount " + accountNumber_
        );
    }

private:
    double overdraftLimit_;
};

