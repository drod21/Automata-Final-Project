// this file will be our main file
#include <stdlib.h>
#include "User.h"
#include <iostream>
#include <string>
#include <time.h>
#include <vector>
#include <thread>
#include <mutex>
#include <unistd.h>
#include <term.h>
#include <chrono>
#include <string>

#define TIME_OUT_VAL 5 // 300 seconds = 5 minutes

using namespace std;

bool validateAccountNumber(string acctNumber);
bool validatePin(string pin);
int findAccount(vector<User> accounts, string accountNum);
void timer();
void clearScreen();

enum States {
    MAIN,MAIN_WAIT,
    REGISTER,REGISTER_CHECK,PIN_ENTER, PIN_CONFIRM, ACCT_CREATED,
    LOGIN,LOGIN_INVALID,LOGIN_OPTIONS,INVALID_PIN,OPTIONS,WITHDRAW,
    WITHDRAW_CHECKING,WITHDRAW_SAVINGS,DEPOSIT,DEPOSIT_CHECKING,DEPOSIT_SAVINGS,
    CHECK_BALANCE,CHECK_CHECKING,CHECK_SAVINGS,LOCKED_OUT,TRANSFER,TRANSFER_STC,TRANSFER_CTS
};

States state = MAIN;
time_t start = time(0);
mutex m;

int main() {
    // Initialize variables //
    int state = 0;
    int i;
    int count = 0;
    int acctIndex;
    
    bool validAcct = true;
    bool withdrawAmountValid = true;
    double userNumberInput;
    double difference;
    time_t curtime;
    
    string userinput;
    string accountNum;
    string pin;
    string tempPin;
    string message;
    
    User *temp;
    vector<User> Accounts;
    User ATM;
    
    ATM.setCheckingAmount(90000);
    User *tempUser = new User("1234567", "1990", 600, 600);
    Accounts.push_back(*tempUser);
    
    while(true) {
        switch(state) {
            case MAIN: // Main
                clearScreen();
                cout << "*********** Welcome to AMDO ATM! **********" << endl;
                cout << "\n";
                state = MAIN_WAIT;
                break;
                
            case MAIN_WAIT: // Mainwait
                cout << "+++++++++++++++++++++++++++++++++++++++++++" << endl;
                cout << "|                                         |" << endl;
                cout << "|      R/r: Register    L/l: Login        |" << endl;
                cout << "|                                         |" << endl;
                cout << "+++++++++++++++++++++++++++++++++++++++++++" << endl;
                cout << "\n==> ";
                cin >> userinput;
                
                if(userinput == "R" || userinput == "r") {
                    state = REGISTER;
                } else if(userinput == "L" || userinput == "l") {
                    state = LOGIN;
                } else {
                    cout << "Invalid option " + userinput + ", please choose one of the below. \n" << endl;
                    state = MAIN_WAIT;
                }
                break;
            case REGISTER: // Register (Ask ACCT #, checks if 7 numbers only)
                cout << "Please enter your desired account number, must be 7 numbers only. Q/q: Quit" << endl;
                cout << "\n==> ";
                cin >> userinput;
                
                if(userinput == "Q" || userinput == "q") {
                    state = MAIN;
                } else if(validateAccountNumber(userinput)) {
                    state = REGISTER_CHECK;
                } else {
                    cout << "Invalid account number, please try again." << endl;
                    state = REGISTER;
                }
                
                break;
            case REGISTER_CHECK: // Register (Checks if acct exists)
                acctIndex = findAccount(Accounts, userinput);
                
                if(acctIndex == -1) {
                    temp = new User;
                    temp->setAccountNumber(userinput);
                    state = PIN_ENTER;
                } else {
                    cout << "Account already exists! Please try again." << endl;
                    state = REGISTER;
                }
                
                break;
            case PIN_ENTER: // Enter pin
                cout << "Please enter your desired PIN. Q/q: Quit | B/b: Back" << endl;
                cout << "\n==> ";
                cin >> tempPin;
                
                if(tempPin == "Q" || tempPin == "B" || tempPin == "q" || tempPin == "b") {
                    state = (tempPin == "Q") ? MAIN : REGISTER;
                } else if (!validatePin(pin)) {
                    cout << "Invalid PIN, please try again." << endl;
                    state = PIN_ENTER;
                } else {
                    state = PIN_CONFIRM;
                }
                
                break;
            case PIN_CONFIRM:
                cout << "Please re-enter your PIN." << endl;
                cout << "\n==> ";
                cin >> pin;
                
                if (pin == tempPin) {
                    temp->setPin(pin);
                    temp->setCheckingAmount(400); // TEMP, GIVING STARTING BALANCE TO TEST WITHDRAW + DEPOSIT
                    temp->setSavingsAmount(400); // TEMP, GIVING STARTING BALANCE TO TEST WITHDRAW + DEPOSIT
                    state = ACCT_CREATED;
                } else {
                    cout << "PIN does not match, please try again." << endl;
                    state = PIN_CONFIRM;
                }
                
                break;
            case ACCT_CREATED: // REQUEST + VALIDATE PIN before creating user
                clearScreen();
                cout << "*********** Account created! *************" << endl;
                cout << "\n";
                Accounts.push_back(*temp); // AFTER PIN is valid
                state = MAIN_WAIT;
                break;
            case LOGIN: // Login
                cout << "Please enter your 7 digit account number. Q/q: Quit" << endl;
                cout << "\n==> ";
                cin >> accountNum;
                clearScreen();
                
                if(accountNum == "Q" || accountNum == "q") {
                    state = MAIN;
                } else {
                    acctIndex = findAccount(Accounts, accountNum);
                    state = (acctIndex >= 0) ? LOGIN_OPTIONS : LOGIN_INVALID;
                }
                break;
            case LOGIN_INVALID: // reset state for login
                cout << "Invalid account number, please try again." << endl;
                state = LOGIN;
                break;
            case LOGIN_OPTIONS: //state for account options
                time(&curtime);
                difference = difftime(curtime,Accounts[acctIndex].getTimerLockout());
                if(difference < 30) {
                    cout << "Account locked for " << 30 - difference << " more seconds." <<endl;
                    state = MAIN_WAIT;
                    break;
                }
                
                cout << "Please input your 4 digit PIN number." << endl;
                cout << "\n==> ";
                cin >> pin;
                
                if(Accounts[acctIndex].getPin() != pin && count < 2) {
                    state = INVALID_PIN;
                } else if(count == 2) {
                    state = LOCKED_OUT; // Locked state;
                } else {
                    state = OPTIONS;
                }
                
                break;
            case INVALID_PIN:
                state = LOGIN_OPTIONS;
                count++;
                cout << "Invalid PIN. " << 3-count << " attempts remaining, please try again." << endl;
                break;
            case OPTIONS:
                clearScreen();
                cout << "+++++++++++++++++++++++++++++++++++++++++" << endl;
                cout << "|       Select an option:               |" << endl;
                cout << "|                                       |" << endl;
                cout << "|            W/w: Withdraw              |" << endl;
                cout << "|            D/d: Deposit               |" << endl;
                cout << "|            T/t: Transfer              |" << endl;
                cout << "|            B/b: Account Balance       |" << endl;
                cout << "|            Q/q: Quit                  |" << endl;
                cout << "|                                       |" << endl;
                cout << "+++++++++++++++++++++++++++++++++++++++++" << endl;
                cout << "\n==> ";
                cin >> userinput;
                
                if(userinput == "Q" || userinput == "q") {
                    state = MAIN;
                } else if(userinput == "W" || userinput == "w") {
                    state = WITHDRAW;
                } else if(userinput == "D" || userinput == "d") {
                    state = DEPOSIT;
                } else if(userinput == "B" || userinput == "b") {
                    state = CHECK_BALANCE;
                } else if(userinput == "T" || userinput == "t") {
                    state = TRANSFER;
                } else {
                    cout << "Invalid Option! Please try again." << endl;
                    state = OPTIONS;
                }
                break;
            case WITHDRAW: // Withdraw
                cout << "Which account would you like to withdraw from? C/c: Checking | S/s: Savings | Q/q: Quit | B/b: Back" << endl;
                cout << "\n==> ";
                cin >> userinput;
                
                if(userinput == "Q" || userinput == "B" || userinput == "q" || userinput == "b") {
                    state = (userinput == "Q" || userinput == "q") ? MAIN : OPTIONS;
                } else if(userinput == "C" || userinput == "c") {
                    state = WITHDRAW_CHECKING;
                } else if(userinput == "S" || userinput == "s") {
                    state = WITHDRAW_SAVINGS;
                } else {
                    cout << "Invalid option, please try again." << endl;
                    state = WITHDRAW;
                }
                break;
            case WITHDRAW_CHECKING: // Withdraw-Checking
                cout << "How much would you like to withdraw? MIN: $10 | MAX: $500 | Q/q: Quit | B/b: Back" << endl;
                cout << "\n==> ";
                cin >> userinput;
                
                if(userinput == "Q" || userinput == "B" || userinput == "q" || userinput == "b") {
                    state = (userinput == "Q" || userinput == "q") ? MAIN : WITHDRAW;
                    break;
                }
                
                withdrawAmountValid = ATM.withdrawAmountValid(stoi(userinput));
                if(!withdrawAmountValid) {
                    cout << "Invalid withdraw amount, please try again." << endl;
                    this_thread::sleep_for (std::chrono::seconds(4));
                    state = WITHDRAW_CHECKING;
                    break;
                }
                
                message = ATM.withdrawal(stoi(userinput), "checking");
                if(message == "Insufficient funds.") {
                    this_thread::sleep_for (std::chrono::seconds(4));
                    cout << "ATM has insufficient funds." << endl;
                    state = WITHDRAW;
                    break;
                }
                
                message = Accounts[acctIndex].withdrawal(stoi(userinput), "checking");
                state = (message == "Insufficient funds.") ? WITHDRAW : OPTIONS;
                if(state == WITHDRAW) {
                    cout << message << endl;
                } else {
                    Accounts[acctIndex].printCheckingAmount(); // TEMP TO SEE ACCOUNTS CHECKING BALANCE AFTER
                    this_thread::sleep_for (std::chrono::seconds(4));
                }
                break;
                
            case WITHDRAW_SAVINGS: // Withdraw-Savings
                cout << "How much would you like to withdraw? MIN: $10 | MAX: $500 | Q/q: Quit | B/b: Back" << endl;
                cout << "\n==> ";
                cin >> userinput;
                
                if(userinput == "Q" || userinput == "q") {
                    state =  MAIN;
                    break;
                }
                
                if(userinput == "B" || userinput == "b") {
                    state = WITHDRAW;
                    break;
                }
                
                withdrawAmountValid = ATM.withdrawAmountValid(stoi(userinput));
                
                if(!withdrawAmountValid) {
                    cout << "Invalid withdraw amount, please try again." << endl;
                    this_thread::sleep_for (std::chrono::seconds(4));
                    state = WITHDRAW_SAVINGS;
                    break;
                }
                
                message = ATM.withdrawal(stoi(userinput), "checking");
                if(message == "Insufficient funds.") {
                    cout << "ATM has insufficient funds."<< endl;
                    this_thread::sleep_for (std::chrono::seconds(4));
                    state = WITHDRAW;
                    break;
                }
                
                message = Accounts[acctIndex].withdrawal(stoi(userinput), "savings");
                state = (message == "Insufficient funds.") ? WITHDRAW : OPTIONS;
                
                if(state == WITHDRAW) {
                    cout << message << endl;
                } else {
                    Accounts[acctIndex].printSavingsAmount(); // TEMP TO SEE ACCOUNTS CHECKING BALANCE AFTER
                }
                this_thread::sleep_for (std::chrono::seconds(4));
                break;
                
            case DEPOSIT: // Deposit
                cout << "Which account would you like to deposit to? C/c: Checking | S/s: Savings | Q/q: Quit | B/b: Back" << endl;
                cout << "\n==> ";
                cin >> userinput;
                
                if(userinput == "Q" || userinput == "q" || userinput == "B" || userinput == "b") {
                    state = (userinput == "Q") ? MAIN : OPTIONS;
                } else if(userinput == "C" || userinput == "c") {
                    state = DEPOSIT_CHECKING;
                } else if(userinput == "S" || userinput == "s") {
                    state = DEPOSIT_SAVINGS;
                } else {
                    cout << "Invalid option, try again." << endl;
                    state = DEPOSIT;
                }
                
                break;
            case DEPOSIT_CHECKING: // Deposit-Checking
                cout << "Enter the amount you would like to deposit? Q/q: Quit | B/b: Back" << endl;
                cout << "\n==> ";
                cin >> userinput;
                
                if(userinput == "Q" || userinput == "q") {
                    state =  MAIN;
                    break;
                }
                
                if(userinput == "B" || userinput == "b") {
                    state = DEPOSIT;
                    break;
                }
                
                cout << "Depositing " << stoi(userinput) << " into checking account." << endl;
                Accounts[acctIndex].depositToChecking(stoi(userinput));
                ATM.depositToChecking(stoi(userinput));
                
                state = OPTIONS;
                Accounts[acctIndex].printCheckingAmount();
                this_thread::sleep_for (std::chrono::seconds(4));
                break;
                
            case DEPOSIT_SAVINGS: // Deposit-Savings
                cout << "Enter the amount you would like to deposit? Q/q: Quit | B/b: Back" << endl;
                cout << "\n==> ";
                cin >> userinput;
                
                if(userinput == "Q" || userinput == "q") {
                    state =  MAIN;
                    break;
                }
                
                if(userinput == "B" || userinput == "b") {
                    state = DEPOSIT;
                    break;
                }
                
                cout << "Depositing " << stoi(userinput) << " into savings account." << endl;
                Accounts[acctIndex].depositToSavings(stoi(userinput));
                ATM.depositToChecking(stoi(userinput));
                Accounts[acctIndex].printSavingsAmount();
                state = OPTIONS;
                this_thread::sleep_for (std::chrono::seconds(4));
                break;
                
            case CHECK_BALANCE: // Account Balance
                cout << "Which account balance would you like to see? C/c: Checking | S/s: Savings |  Q/q: Quit | B/b: Back" << endl;
                cout << "\n==> ";
                cin >> userinput;
                
                if(userinput == "Q" || userinput == "q" || userinput == "b" || userinput == "B") {
                    state = (userinput == "Q" || userinput == "q") ? MAIN : OPTIONS;
                } else if(userinput == "C" || userinput == "c") {
                    state = CHECK_CHECKING;
                } else if(userinput == "S" || userinput == "s") {
                    state = CHECK_SAVINGS;
                } else {
                    cout << "Invalid option, please try again." << endl;
                    this_thread::sleep_for (std::chrono::seconds(4));
                    state = CHECK_BALANCE;
                }
                
                break;
            case CHECK_CHECKING: // Acct Bal Checking
                cout << "Your checking account balance is: $" << Accounts[acctIndex].getCheckingAmount() << endl;
                this_thread::sleep_for (std::chrono::seconds(4));
                state = OPTIONS;
                break;
                
            case CHECK_SAVINGS: // Acct Bal Savings
                cout << "Your savings account balance is: $" << Accounts[acctIndex].getSavingsAmount() << endl;
                this_thread::sleep_for (std::chrono::seconds(4));
                state = OPTIONS;
                break;
                
            case LOCKED_OUT: // locked state (NEEDS COMPLETED)
                cout << "Locked out for 30 seconds." << endl;
                count = 0;
                Accounts[acctIndex].setTimerLockout();
                state = MAIN_WAIT; //Back to main menu
                break;
                
            case TRANSFER: // Transfer funds
                cout << "Transfer from: S/s: Savings-to-Checking | C/c: Checking-to-Savings | Q/q: Quit | B/b: Back" << endl;
                cin >> userinput;
                if(userinput == "S" || userinput == "s") {
                    state = TRANSFER_STC;
                } else if(userinput == "C" || userinput == "c") {
                    state = TRANSFER_CTS;
                } else if(userinput == "B" || userinput == "b") {
                    state = OPTIONS;
                } else if(userinput == "Q" || userinput == "q") {
                    state = MAIN;
                } else {
                    cout << "Invalid option, please try again." << endl;
                }
                break;
                
            case TRANSFER_STC:
                Accounts[acctIndex].printSavingsAmount();
                cout << "How much would you like to transfer to your Checking account? | Q/q: Quit | B/b: Back" << endl;
                cin >> userinput;
                
                if(userinput == "B" || userinput == "b") {
                    state = TRANSFER;
                    break;
                }
                
                if(userinput == "Q" || userinput == "q") {
                    state = MAIN;
                    break;
                }
                
                // ADD CHECK THAT USERINPUT WAS DIGIT
                message = Accounts[acctIndex].transferMoney("checking", stod(userinput));
                if(message == "Successful transfer to checking account.") {
                    cout << message << "\nNew ";
                    Accounts[acctIndex].printCheckingAmount();
                    cout << "New balance: $";
                    Accounts[acctIndex].printSavingsAmount();
                    this_thread::sleep_for (std::chrono::seconds(4));
                    state = OPTIONS;
                } else {
                    cout << message << endl;
                }
                break;
            case TRANSFER_CTS:
                Accounts[acctIndex].printCheckingAmount();
                cout << "How much would you like to transfer to your Savings account? Q/q: Quit | B/b: Back" << endl;
                cin >> userinput;
                
                if(userinput == "B" || userinput == "b") {
                    state = TRANSFER;
                    break;
                }
                
                if(userinput == "Q" || userinput == "q") {
                    state = MAIN;
                    break;
                }
                
                // ADD CHECK THAT USERINPUT WAS DIGIT
                
                message = Accounts[acctIndex].transferMoney("savings", stod(userinput));
                if(message == "Successful transfer to savings account.") {
                    cout << message << "\nNew ";
                    Accounts[acctIndex].printCheckingAmount();
                    cout << "New balance: $";
                    Accounts[acctIndex].printSavingsAmount();
                    this_thread::sleep_for (std::chrono::seconds(4));
                    state = OPTIONS;
                } else {
                    cout << message << endl;
                }
                break;
        }
    }
    return 0;
}

void timer(){
    while(true){
        double seconds_since_start = difftime( time(0), start);
        if(seconds_since_start == TIME_OUT_VAL) {
            cout << "ATM timed out." << endl;
            std::lock_guard<std::mutex> lock(m);
            state = MAIN;
            break;
        }
    }
}

void clearScreen() {
    if (!cur_term) {
        int result;
        setupterm(NULL, STDOUT_FILENO, &result);
        if (result <= 0) return;
    }
    
    putp(tigetstr("clear"));
}

// returns the index of the account
int findAccount(vector<User> accounts, string accountNum) {
    for(int i = 0; i < accounts.size(); i++) {
        if (accounts[i].getaccountNumber() == accountNum)
            return i;
    }
    return -1;
}

bool validatePin(string pin) {
    if(pin.length() != 4)
        return false;
    
    for(int i = 0; i < pin.length(); i++) {
        if(!isdigit(pin[i])) {
            return false;
        }
    }
    return true;
}

bool validateAccountNumber(string acctNumber) {
    if(acctNumber.length() != 7)
        return false;
    
    for(int i = 0; i < acctNumber.length(); i++) {
        if(!isdigit(acctNumber[i])) {
            return false;
        }
    }
    return true;
}

