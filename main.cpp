#include "Bank.hpp"
#include "DataManager.hpp"
#include <iostream>
#include <limits> // For std::numeric_limits

void printSeparator() {
    std::cout << "\n-----------------------------------------\n";
}

int main() {
    // Get the DataManager instance for logging setup
    DataManager& dataManager = DataManager::getInstance();