<!-- .github/copilot-instructions.md for bankcpp -->
# Copilot / AI Agent Instructions — bankcpp

Purpose: provide focused, actionable guidance for AI coding agents working on this C++ banking sample.

- **Big picture:** This is a small single-process banking simulation. Core responsibilities are split across:
  - `Bank` (in `Bank.hpp`/`Bank.cpp`): high-level orchestration — customer/account lifecycle, transfers, monthly maintenance.
  - `Customer` (in `Customer.*`): owns accounts via `unique_ptr<Account>`; returns raw pointers for callers.
  - `Account` and subclasses `SavingsAccount` / `CheckingAccount`: balance logic, `deposit`/`withdraw`, `performMonthlyMaintenance`.
  - `Transaction` (in `Transaction.*`): immutable record objects; operator<< implemented for log-friendly printing.
  - `DataManager` (in `DataManager.*`) + `DataLogEntry`: process-wide singleton used for logging; keeps an in-memory `vector` and prints to console.

- **Ownership & memory patterns:**
  - `Customer` stores `vector<unique_ptr<Account>>` and transfers ownership via `addAccount(unique_ptr<Account>)`.
  - Many APIs return raw pointers (e.g., `Customer* Bank::getCustomer(...)`, `Account* Bank::getAccount(...)`). Do NOT `delete` those — ownership remains with the container.
  - IDs are generated using `atomic` counters in `.cpp` files (see `Customer.cpp`, `Account.cpp`, `Transaction.cpp`). When adding new ID counters follow the same pattern and initial values (e.g., `Customer::nextCustomerId(10000)`).

- **Logging & concurrency:**
  - Use `DataManager::getInstance().logEvent(...)` for all runtime events. `DataManager` already holds a mutex for thread-safety for logs.
  - Other classes are NOT automatically thread-safe — wrap higher-level coordination if adding concurrency.

- **Naming conventions & formats:**
  - Account numbers start with `ACC` (see `Account.cpp`), customer IDs with `CUS`, transaction IDs use numeric `TRX`-style counters.
  - Filenames use `CamelCase` per class (e.g., `SavingsAccount.cpp` / `SavingsAccount.hpp`). Keep new class files following this pattern.

- **Common code edits:**
  - To add a new account type: add `NewTypeAccount.hpp/.cpp`, implement `performMonthlyMaintenance` and constructors, then update `Bank.cpp` to add a factory method (similar to `createSavingsAccount`).
  - To add new log kinds or structure, update `DataLogEntry` and use `DataManager::logEvent` in the same pattern as existing uses in `Bank`, `Account`, `Customer`.

- **Build / run (developer guidance):**
  - No build script is provided. Example compiles:
    - with MinGW/G++ (PowerShell): `g++ -std=c++17 -O2 -pthread *.cpp -o bankapp.exe`
    - with MSVC (PowerShell): `cl /EHsc /std:c++17 *.cpp` (then run `.ankapp.exe`).
  - The program prints logs to stdout via `DataManager` — use console output for quick debugging.

- **Where to make small changes safely:**
  - Prefer modifying behavior in the relevant class file (`Account::deposit`, `Account::withdraw`, `Bank::transferFunds`) rather than changing ownership/storage layout.
  - When adding public APIs, update the corresponding header and keep implementation details in `.cpp` files (mirrors current project structure).

- **Examples of patterns to copy when implementing features:**
  - Logging on important events:
    DataManager::getInstance().logEvent(DataLogEntry::LogLevel::INFO, "Message");
  - Creating an account and handing ownership to a customer:
    `auto a = make_unique<SavingsAccount>(customerId, bal, rate); customer->addAccount(move(a));`

- **Gotchas & checks for PRs:**
  - Ensure you do not return dangling pointers — keep ownership semantics consistent.
  - If introducing threads, ensure non-DataManager state is protected.
  - Keep ID initial values and prefixes consistent with existing ones to avoid surprising identifiers.

If anything here is unclear or you want additional examples (for testing, CI, or a sample CLion/VSCode launch configuration), tell me which area to expand. 
