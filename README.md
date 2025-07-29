# 💳 Bank Management System

A **C-based console application** that simulates a basic bank management system. It allows authenticated employees to manage bank accounts, perform transactions, and generate reports.

---

## 📌 Features

### 🔐 1. Login
- Users must log in using credentials stored in `users.txt`
- Format: `username password` (one user per line)

### 📥 2. Load Accounts
- Reads account data from `accounts.txt`
- Format per line:  
  `account_number,name,email,balance,mobile,MM-YYYY`

### 🔎 3. Query (Search)
- Search for an account by account number and display all its data

### 🧠 4. Advanced Search
- Search by keyword in the account name (e.g., “Ahmed”)

### ➕ 5. Add Account
- Add a new account with a unique account number
- `Date Opened` is auto-filled with the system's current month/year

### ❌ 6. Delete Account
- Only accounts with **zero balance** can be deleted
- Validates existence of account

### ✏️ 7. Modify Account
- Modify only name, mobile, or email
- Cannot modify account number or balance

### 💸 8. Withdraw
- Withdraw up to `$10,000` per transaction
- Validates account and sufficient balance

### 💰 9. Deposit
- Deposit up to `$10,000` per transaction
- Validates account number

### 🔁 10. Transfer
- Transfer between two existing accounts
- Updates balances and logs transaction history for both

### 📄 11. Report
- Print the **last 5 transactions** from the file `account_number.txt`

### 📊 12. Print (Sorted)
- Print all accounts sorted by:
  - Name
  - Balance
  - Date Opened

### 💾 13. Save
- Save updated account data back to `accounts.txt`
- Prompts to confirm/discard unsaved changes after any modification

### 🚪 14. Quit
- Exit the program safely

---

## 📂 Files

- `main.c`: Main source code file
- `users.txt`: List of valid usernames and passwords
- `accounts.txt`: Bank account records
- `account_number.txt`: Transaction history for each account

---


## 📝 Data Format

### users.txt
```
username password
ahmed.mohamed 123$@1
ali.ahmed 654321
ziad.ali 123abc
```

### accounts.txt
```
9780136019,Mohamed Ali,m.ali@gmail.
