#include <iostream>
#include <ostream> // For ostream
#include <limits> // For numeric_limits
#include <iomanip> // For fixed, setprecision
#include <vector>  // Ensure vector is included for clarity
#include "Bank.cpp" // For Bank class
#include "DataManager.cpp" // For DataManager singleton
using namespace std;

// Helper function to print a separator for better readability in console output
void printSeparator() {
    cout << "\n-----------------------------------------\n";
}

int main() {
    // Get the DataManager instance for logging setup
    // DataManager is a Singleton, so we get its single instance
    DataManager& dataManager = DataManager::getInstance();
    
    cout << "--- C++ Banking System Simulation ---\n";

    // 1. Create a Bank instance
    Bank myBank("Global Bank");
    printSeparator();

    // 2. Create Customers
    Customer cust1 = myBank.createCustomer("Alice Smith", "123 Main St", "555-1111");
    Customer cust2 = myBank.createCustomer("Bob Johnson", "456 Oak Ave", "555-2222");
    Customer cust3 = myBank.createCustomer("Charlie Brown", "789 Pine Ln", "555-3333");
    printSeparator();

    // Verify customers were created
    cout << "Current Customers:\n";
    cout << cust1 << endl;
    cout << cust2 << endl;
    cout << cust3 << endl;
    printSeparator();

    // 3. Create Accounts for Customers
    Account aliceSavings = myBank.createSavingsAccount(cust1.getCustomerId(), 1000.00, 0.01); // 1% interest
    Account aliceChecking = myBank.createCheckingAccount(cust1.getCustomerId(), 500.00, 200.00); // $200 overdraft
    
    Account bobChecking = myBank.createCheckingAccount(cust2.getCustomerId(), 2000.00, 500.00);
    Account bobSavings = myBank.createSavingsAccount(cust2.getCustomerId(), 5000.00, 0.015); // 1.5% interest

    // Charlie only has a checking account for now
    Account charlieChecking = myBank.createCheckingAccount(cust3.getCustomerId(), 100.00, 100.00);
    printSeparator();

    // Display initial account states
    cout << "Initial Account States:\n";
    cout << aliceSavings;
    cout << aliceChecking;
    cout << bobChecking;
    cout << bobSavings;
    cout << charlieChecking;
    printSeparator();

    // 4. Perform Transactions
    cout << "--- Performing Transactions ---\n";
    
    // Deposit to Alice's Savings
    cout << "Alice deposits $200 into Savings.\n";
    if (aliceSavings.deposit(200.00, "Cash deposit")) {
        cout << "Alice's Savings Balance: $" << fixed << setprecision(2) << aliceSavings.getBalance() << "\n";
    }
    printSeparator();

    // Withdraw from Bob's Checking
    cout << "Bob withdraws $300 from Checking.\n";
    if (bobChecking.withdraw(300.00, "ATM withdrawal")) {
        cout << "Bob's Checking Balance: $" << fixed << setprecision(2) << bobChecking.getBalance() << "\n";
    }
    printSeparator();

    // Attempt an overdraft withdrawal from Charlie's Checking
    cout << "Charlie attempts to withdraw $250 from Checking (balance $100, overdraft $100).\n";
    if (charlieChecking.withdraw(250.00, "Large purchase")) {
        // This should fail, so this block should not execute
        cout << "Charlie's Checking Balance: $" << fixed << setprecision(2) << charlieChecking.getBalance() << "\n";
    } else {
        cout << "Charlie's withdrawal failed as expected. Current Balance: $" << fixed << setprecision(2) << charlieChecking.getBalance() << "\n";
    }
    printSeparator();

    // Successful overdraft withdrawal from Charlie's Checking
    cout << "Charlie attempts to withdraw $150 from Checking (balance $100, overdraft $100 -> total $200 available).\n";
    if (charlieChecking.withdraw(150.00, "Small purchase")) {
        cout << "Charlie's Checking Balance: $" << fixed << setprecision(2) << charlieChecking.getBalance() << "\n";
    } else {
        cout << "Charlie's withdrawal failed.\n"; // This should not happen now
    }
    printSeparator();

    // Transfer funds between accounts
    cout << "Transferring $150 from Bob's Checking to Alice's Checking.\n";
    if (myBank.transferFunds(bobChecking.getAccountNumber(), aliceChecking.getAccountNumber(), 150.00, "Gift")) {
        cout << "Bob's Checking Balance: $" << fixed << setprecision(2) << bobChecking.getBalance() << "\n";
        cout << "Alice's Checking Balance: $" << fixed << setprecision(2) << aliceChecking.getBalance() << "\n";
    } else {
        cout << "Transfer failed.\n";
    }
    printSeparator();

    // 5. Run Monthly Maintenance
    // This function inside Bank already iterates through all customer's accounts
    // and calls performMonthlyMaintenance on them.
    cout << "--- Running Monthly Maintenance ---\n";
    myBank.runMonthlyMaintenance(); 
    printSeparator();

    cout << "Account Balances after Monthly Maintenance (Interest Applied, etc.):\n";
    cout << aliceSavings;
    cout << aliceChecking;
    cout << bobChecking;
    cout << bobSavings;
    cout << charlieChecking;
    printSeparator();

    // 6. Display Transaction Histories
    cout << "--- Transaction Histories ---\n";
    cout << "Alice's Savings Account (" << aliceSavings.getAccountNumber() << ") Transactions:\n";
    vector<Transaction> transactionHistory = aliceSavings.getTransactionHistory();
    for (size_t i = 0; i < transactionHistory.size(); ++i) {
        cout << "  " << transactionHistory[i] << "\n";
    }
    printSeparator();

    cout << "Bob's Checking Account (" << bobChecking.getAccountNumber() << ") Transactions:\n";
    for (const auto& trx : bobChecking.getTransactionHistory()) {
        cout << "  " << trx << "\n";
    }
    printSeparator();
    
    // Can also get all accounts and print transactions for each
    cout << "--- All Account Transaction Histories (Summary) ---\n";
    // Store the returned vector in a local const variable to ensure its lifetime
    const vector<Account*> allAccountsInBank = myBank.getAllAccounts();
    for(Account* acc_ptr : allAccountsInBank) { // This loop is now robust
        cout << "\nAccount: " << acc_ptr->getAccountNumber() << " (Owner: " << acc_ptr->getOwnerId() << ")\n";
        for (const auto& trx : acc_ptr->getTransactionHistory()) {
            cout << "  " << trx << "\n";
        }
    }
    printSeparator();


    // 7. Display all logged events
    cout << "--- All System Logs ---\n";
    // Store the returned vector in a local const variable to ensure its lifetime
    const vector<DataLogEntry> allLogs = dataManager.getAllLogs();
    for (const auto& entry : allLogs) {
        cout << entry << "\n";
    }
    printSeparator();

    cout << "--- End of Simulation ---\n";

    return 0;
}