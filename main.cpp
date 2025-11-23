#include "Bank.hpp"
#include "DataManager.hpp"
#include <iostream>
#include <limits> // For std::numeric_limits
#include <iomanip> // For std::fixed, std::setprecision
#include <vector>  // Ensure std::vector is included for clarity

// Helper function to print a separator for better readability in console output
void printSeparator() {
    std::cout << "\n-----------------------------------------\n";
}

int main() {
    // Get the DataManager instance for logging setup
    // DataManager is a Singleton, so we get its single instance
    DataManager& dataManager = DataManager::getInstance();
    
    std::cout << "--- C++ Banking System Simulation ---\n";

    // 1. Create a Bank instance
    Bank myBank("Global Bank");
    printSeparator();

    // 2. Create Customers
    Customer* cust1 = myBank.createCustomer("Alice Smith", "123 Main St", "555-1111");
    Customer* cust2 = myBank.createCustomer("Bob Johnson", "456 Oak Ave", "555-2222");
    Customer* cust3 = myBank.createCustomer("Charlie Brown", "789 Pine Ln", "555-3333");
    printSeparator();

    // Verify customers were created
    std::cout << "Current Customers:\n";
    if (cust1) std::cout << *cust1 << std::endl;
    if (cust2) std::cout << *cust2 << std::endl;
    if (cust3) std::cout << *cust3 << std::endl;
    printSeparator();

    // 3. Create Accounts for Customers
    Account* aliceSavings = myBank.createSavingsAccount(cust1->getCustomerId(), 1000.00, 0.01); // 1% interest
    Account* aliceChecking = myBank.createCheckingAccount(cust1->getCustomerId(), 500.00, 200.00); // $200 overdraft
    
    Account* bobChecking = myBank.createCheckingAccount(cust2->getCustomerId(), 2000.00, 500.00);
    Account* bobSavings = myBank.createSavingsAccount(cust2->getCustomerId(), 5000.00, 0.015); // 1.5% interest

    // Charlie only has a checking account for now
    Account* charlieChecking = myBank.createCheckingAccount(cust3->getCustomerId(), 100.00, 100.00);
    printSeparator();

    // Display initial account states
    std::cout << "Initial Account States:\n";
    if (aliceSavings) std::cout << *aliceSavings;
    if (aliceChecking) std::cout << *aliceChecking;
    if (bobChecking) std::cout << *bobChecking;
    if (bobSavings) std::cout << *bobSavings;
    if (charlieChecking) std::cout << *charlieChecking;
    printSeparator();

    // 4. Perform Transactions
    std::cout << "--- Performing Transactions ---\n";
    
    // Deposit to Alice's Savings
    std::cout << "Alice deposits $200 into Savings.\n";
    if (aliceSavings && aliceSavings->deposit(200.00, "Cash deposit")) {
        std::cout << "Alice's Savings Balance: $" << std::fixed << std::setprecision(2) << aliceSavings->getBalance() << "\n";
    }
    printSeparator();

    // Withdraw from Bob's Checking
    std::cout << "Bob withdraws $300 from Checking.\n";
    if (bobChecking && bobChecking->withdraw(300.00, "ATM withdrawal")) {
        std::cout << "Bob's Checking Balance: $" << std::fixed << std::setprecision(2) << bobChecking->getBalance() << "\n";
    }
    printSeparator();

    // Attempt an overdraft withdrawal from Charlie's Checking
    std::cout << "Charlie attempts to withdraw $250 from Checking (balance $100, overdraft $100).\n";
    if (charlieChecking && charlieChecking->withdraw(250.00, "Large purchase")) {
        // This should fail, so this block should not execute
        std::cout << "Charlie's Checking Balance: $" << std::fixed << std::setprecision(2) << charlieChecking->getBalance() << "\n";
    } else {
        std::cout << "Charlie's withdrawal failed as expected. Current Balance: $" << std::fixed << std::setprecision(2) << charlieChecking->getBalance() << "\n";
    }
    printSeparator();

    // Successful overdraft withdrawal from Charlie's Checking
    std::cout << "Charlie attempts to withdraw $150 from Checking (balance $100, overdraft $100 -> total $200 available).\n";
    if (charlieChecking && charlieChecking->withdraw(150.00, "Small purchase")) {
        std::cout << "Charlie's Checking Balance: $" << std::fixed << std::setprecision(2) << charlieChecking->getBalance() << "\n";
    } else {
        std::cout << "Charlie's withdrawal failed.\n"; // This should not happen now
    }
    printSeparator();

    // Transfer funds between accounts
    std::cout << "Transferring $150 from Bob's Checking to Alice's Checking.\n";
    if (myBank.transferFunds(bobChecking->getAccountNumber(), aliceChecking->getAccountNumber(), 150.00, "Gift")) {
        std::cout << "Bob's Checking Balance: $" << std::fixed << std::setprecision(2) << bobChecking->getBalance() << "\n";
        std::cout << "Alice's Checking Balance: $" << std::fixed << std::setprecision(2) << aliceChecking->getBalance() << "\n";
    } else {
        std::cout << "Transfer failed.\n";
    }
    printSeparator();

    // 5. Run Monthly Maintenance
    // This function inside Bank already iterates through all customer's accounts
    // and calls performMonthlyMaintenance on them.
    std::cout << "--- Running Monthly Maintenance ---\n";
    myBank.runMonthlyMaintenance(); 
    printSeparator();

    std::cout << "Account Balances after Monthly Maintenance (Interest Applied, etc.):\n";
    if (aliceSavings) std::cout << *aliceSavings;
    if (aliceChecking) std::cout << *aliceChecking;
    if (bobChecking) std::cout << *bobChecking;
    if (bobSavings) std::cout << *bobSavings;
    if (charlieChecking) std::cout << *charlieChecking;
    printSeparator();

    // 6. Display Transaction Histories
    std::cout << "--- Transaction Histories ---\n";
    if (aliceSavings) {
        std::cout << "Alice's Savings Account (" << aliceSavings->getAccountNumber() << ") Transactions:\n";
        std::vector<Transaction> transactionHistory = aliceSavings->getTransactionHistory();
        for (size_t i = 0; i < transactionHistory.size(); ++i) {
            std::cout << "  " << transactionHistory[i] << "\n";
        }
    }
    printSeparator();

    if (bobChecking) {
        std::cout << "Bob's Checking Account (" << bobChecking->getAccountNumber() << ") Transactions:\n";
        for (const auto& trx : bobChecking->getTransactionHistory()) {
            std::cout << "  " << trx << "\n";
        }
    }
    printSeparator();
    
    // Can also get all accounts and print transactions for each
    std::cout << "--- All Account Transaction Histories (Summary) ---\n";
    // Store the returned vector in a local const variable to ensure its lifetime
    const std::vector<Account*> allAccountsInBank = myBank.getAllAccounts();
    for(Account* acc_ptr : allAccountsInBank) { // This loop is now robust
        std::cout << "\nAccount: " << acc_ptr->getAccountNumber() << " (Owner: " << acc_ptr->getOwnerId() << ")\n";
        for (const auto& trx : acc_ptr->getTransactionHistory()) {
            std::cout << "  " << trx << "\n";
        }
    }
    printSeparator();


    // 7. Display all logged events
    std::cout << "--- All System Logs ---\n";
    // Store the returned vector in a local const variable to ensure its lifetime
    const std::vector<DataLogEntry> allLogs = dataManager.getAllLogs();
    for (const auto& entry : allLogs) {
        std::cout << entry << "\n";
    }
    printSeparator();

    std::cout << "--- End of Simulation ---\n";

    return 0;
}