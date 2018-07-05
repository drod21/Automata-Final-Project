// this file will be our main file
#include <stdlib.h>
#include "User.h"
#include <iostream>
#include <string>
#include <time.h>
#include <vector>

using namespace std;
bool validateAccountNumber(string acctNumber);
bool validatePin(string pin);
int findAccount(vector<User> accounts, string accountNum);
void clearScreen();

int main() {
    
    int state = 0;
    string userInput;
    int i;
    string accountNum;
    string pin;
    bool validAcct = true;
    bool withdrawAmountValid = true;
    int acctIndex;
    User *temp;
    vector<User> Accounts;
    int count = 0;
    double userNumberInput;
    double difference;
    time_t curTime;
    
    string message;
    
    User ATM;
    ATM.setCheckingAmount(90000);
   // ATM.printCheckingAmount(); // TEMP (STARTING AMOUNT IN ATM)
    User *tempUser = new User("1234567", "1990", 600, 600);
    Accounts.push_back(*tempUser);
    
    while(true) {
        switch(state) {
            case 0: // Main
                cout << "Welcome to AMDO ATM!" << endl;
                state = 1;
                break;
            case 1: // Mainwait
                cout << "R/r: Register | L/l: Login" << endl;
                cin >> userInput;
                clearScreen();
                if((userInput != "R" || userInput != "r") && (userInput != "L" || userInput != "l")) {
                    state = 1;
                    cout << "Invalid option, please choose one of the below." << endl;
                }
                
                if(userInput == "R" || userInput == "r") {
                    state = 2;
                } else if(userInput == "L" || userInput == "l") {
                    state = 11;
                }

                break;
            case 2: // Register (Ask ACCT #, checks if 7 numbers only)
                cout << "Please enter your desired account number (it must be 7 numbers only). Q/q: Quit" << endl;
                cin >> accountNum;
                clearScreen();
                
                if(accountNum == "Q" || accountNum == "q") {
                    state = 0;
                    break;
                }
                
                validAcct = validateAccountNumber(accountNum);
                state = (validAcct) ? 3 : 2;
                
                if(state == 2)
                    cout << "Invalid account number, please try again." << endl;
                
                break;
                
            case 3: // Register (Checks if acct exists)
                //cout << "LIST OF USERS" << endl;
                acctIndex = findAccount(Accounts, accountNum);
                state = (acctIndex == -1) ? 4 : 2;
                
                if(state == 2) {
                    cout << "The account already exists. Please try again" << endl;
                }
                break;
            case 4: // Enter/validate pin
                cout << "Please enter your desired PIN. Q/q: Quit | B/b: Back" << endl;
                cin >> pin;
                clearScreen();
                
                if(pin == "Q" || pin == "q" || pin == "b" || pin == "B") {
                    state = (pin == "Q") ? 0 : 2;
                    break;
                }
                
                state = (validatePin(pin)) ? 5 : 4;
                if(state == 4) {
                    cout << "Invalid PIN, please try again." << endl;
                } else {
                    temp = new User(accountNum, pin, 400, 400); // temp to set initial account balances
                    //temp = new User(accountNum, pin) // uncomment this when done testing
                }
                break;
                
            case 5: // REQUEST + VALIDATE PIN before creating user
                cout << "Account created." << endl;
                
                Accounts.push_back(*temp); // AFTER PIN is valid
                state = 1;
                break;
                
            case 11: // Login
                cout << "Please enter your 7 digit account number. Q/q: Quit" << endl;
                cin >> accountNum;
                clearScreen();
                
                if(accountNum == "Q" || accountNum == "q") {
                    state = 0;
                    break;
                }
                
                acctIndex = findAccount(Accounts, accountNum);
                state = (acctIndex >= 0) ? 13 : 12;
                break;
            case 12: // reset state for login
                cout << "Invalid account number, please try again." << endl;
                state = 11;
                break;
            case 13: //state for account options
                time(&curTime);
                difference = difftime(curTime,Accounts[acctIndex].getTimerLockout());
                if(difference >= 30) {
                    cout << "Please input your 4 digit PIN number." << endl;
                    cin >> pin;
                    clearScreen();
                    
                    if(Accounts[acctIndex].getPin() != pin && count < 2) {
                        state = 14;
                    } else if(count == 2) {
                        state = 26; // Locked state;
                    } else {
                        state = 15;
                    }
                } else {
                    cout << "Account locked for " << 30 - difference << " more seconds." <<endl;
                    state = 1;
                    break;
                }
                break;
                
            case 14:
                state = 13;
                count++;
                cout << "Invalid PIN. " << 3 - count << " attempts remaining, please try again." << endl;
                break;
                
            case 15:
                cout << "Please select an option. W/w: Withdraw | D/d: Deposit | B/b: Account Balance | Q/q: Quit" << endl;
                cin >> userInput;
                clearScreen();
                
                if(userInput == "Q" || userInput == "q" || userInput == "b"|| userInput == "B") {
                    state = (userInput == "Q" || userInput == "q") ? 0 : 18;
                    break;
                }
                
                if(userInput == "W" || userInput == "w") {
                    state = 16;
                } else if(userInput == "D" || userInput == "d") {
                    state = 17;
                } else {
                    cout << "Invalid option! Please try again." << endl;
                    state = 15;
                }
                break;
                
            case 16: // Withdraw
                cout << "What account would you like to withdraw from? C: Checking | S/s: Savings | Q/q: Quit | B/b: Back" << endl;
                cin >> userInput;
                clearScreen();
                
                
                if(userInput == "Q" || userInput == "q" || userInput == "b" || userInput == "B") {
                    state = (userInput == "Q" || userInput == "q") ? 0 : 15;
                    break;
                }
                
                if(userInput == "C" || userInput == "c") {
                    state = 161;
                } else if(userInput == "S" || userInput == "s") {
                    state = 162;
                } else {
                    cout << "Invalid option, please try again." << endl;
                    state = 16;
                }
                
                break;
            case 161: // Withdraw-Checking
                cout << "How much would you like to withdraw? MIN: $10 | MAX: $500 | 0: Quit | 1: Back" << endl;
                cin >> userNumberInput;
                clearScreen();
                
                if(userNumberInput == 0 || userNumberInput == 1) {
                    state = (userNumberInput == 0) ? 0 : 16;
                    break;
                }
                
                withdrawAmountValid = ATM.withdrawAmountValid(userNumberInput);
                
                if(!withdrawAmountValid) {
                    cout << "Invalid withdraw amount, please try again." << endl;
                    state = 161;
                    break;
                }
                
                message = ATM.withdrawal(userNumberInput, "checking");
                if(message == "Insufficient funds.") {
                    cout << "ATM has insufficient funds." << endl;
                    state = 16;
                    break;
                }
                
                message = Accounts[acctIndex].withdrawal(userNumberInput, "checking");
                state = (message == "Insufficient funds.") ? 16 : 15;
                if(state == 16) {
                    cout << message << endl;
                } else {
                    Accounts[acctIndex].printCheckingAmount(); // TEMP TO SEE ACCOUNTS CHECKING BALANCE AFTER
                    //ATM.printCheckingAmount(); // TEMP TO SEE ATM CHECKING BALANCE AFTER
                }
                break;
                
            case 162: // Withdraw-Savings
                cout << "How much would you like to withdraw? MIN: $10 | MAX: $500 | 0: Quit | 1: Back" << endl;
                cin >> userNumberInput;
                clearScreen();
                
                if(userNumberInput == 0 || userNumberInput == 1) {
                    state = (userNumberInput == 0) ? 0 : 16;
                    break;
                }
                
                withdrawAmountValid = ATM.withdrawAmountValid(userNumberInput);
                
                if(!withdrawAmountValid) {
                    cout << "Invalid withdraw amount, please try again." << endl;
                    state = 162;
                    break;
                }
                
                message = ATM.withdrawal(userNumberInput, "checking");
                if(message == "Insufficient funds.") {
                    cout << "ATM has insufficient funds."<< endl;
                    state = 16;
                    break;
                }
                
                message = Accounts[acctIndex].withdrawal(userNumberInput, "savings");
                state = (message == "Insufficient funds.") ? 16 : 15;
                if(state == 16) {
                    cout << message << endl;
                } else {
                    Accounts[acctIndex].printSavingsAmount(); // TEMP TO SEE ACCOUNTS CHECKING BALANCE AFTER
                   // ATM.printCheckingAmount(); // TEMP TO SEE ATM CHECKING BALANCE AFTER
                }
                break;
            case 17: // Deposit
                cout << "What account would you like to deposit to? C/c: Checking | S/s: Savings | Q/q: Quit | B/b: Back" << endl;
                cin >> userInput;
                clearScreen();
                
                if(userInput == "Q" || userInput == "q" || userInput == "b" || userInput == "B") {
                    state = (userInput == "Q") ? 0 : 15;
                    break;
                }
                if((userInput != "C" || userInput != "c") && (userInput != "S" || userInput != "s")) {
                    cout << "Invalid option, try again." << endl;
                    state = 17;
                } else if(userInput == "C" || userInput == "c") {
                    state = 171;
                } else if(userInput == "S" || userInput == "s") {
                    state = 172;
                }
                break;
            case 171: // Deposit-Checking
                cout << "Enter the amount you would like to deposit in Checking account? 0: Quit | 1: Back" << endl;
                cin >> userNumberInput;
                clearScreen();
                
                if(userNumberInput == 0 || userNumberInput == 1) {
                    state = (userNumberInput == 0) ? 0 : 17;
                    break;
                }

                cout << "Depositing " << userNumberInput << " into checking account." << endl;
                Accounts[acctIndex].depositToChecking(userNumberInput);
                ATM.depositToChecking(userNumberInput);
                
                Accounts[acctIndex].printCheckingAmount(); // TEMP, CHECKING AMOUNTS CHANGED
                ATM.getCheckingAmount(); // TEMP, CHECKING AMOUNTS CHANGED
                state = 15;
                break;
                
            case 172: // Deposit-Savings
                cout << "Enter the amount you would like to deposit in Savings account? 0: Quit | 1: Back" << endl;
                cin >> userNumberInput;
                clearScreen();
                
                if(userNumberInput == 0 || userNumberInput == 1) {
                    state = (userNumberInput == 0) ? 0 : 17;
                    break;
                }
                
                cout << "Depositing " << userNumberInput << " into savings account." << endl;
                Accounts[acctIndex].depositToSavings(userNumberInput);
                ATM.depositToChecking(userNumberInput);
                
              //  cout << "ACCOUNT AMOUNT " << endl;
              //  Accounts[acctIndex].printSavingsAmount(); // TEMP, CHECKING AMOUNTS CHANGED
              //  cout << "ATM AMOUNT " << endl;
              //  ATM.printCheckingAmount(); // TEMP, CHECKING AMOUNTS CHANGED
                state = 15;
                break;
                
            case 18: // Account Balance
                cout << "What account balance would you like to see? C/c: Checking | S/s: Savings | Q/q: Quit | B/b: Back" << endl;
                cin >> userInput;
                clearScreen();
                
                if(userInput == "Q" || userInput == "q" || userInput == "b" || userInput == "B") {
                    state = (userInput == "Q" || userInput == "q") ? 0 : 15;
                    break;
                }
                
                if((userInput != "C" || userInput != "c") && (userInput != "S" || userInput != "s")) {
                    cout << "Invalid option, try again." << endl;
                    state = 18;
                } else if(userInput == "C" || userInput == "c") {
                    state = 181;
                } else if(userInput == "S" || userInput == "s") {
                    state = 182;
                }
                
                break;
                
            case 181: // Acct Bal Checking
                Accounts[acctIndex].printCheckingAmount();
                state = 15;
                break;
                
            case 182: // Acct Bal Savings
                Accounts[acctIndex].printSavingsAmount();
                state = 15;
                break;
                
            case 26: // locked state (NEEDS COMPLETED)
                cout << "Locked out for 30 seconds." << endl;
                count = 0;
                Accounts[acctIndex].setTimerLockout();
                state = 1; //Back to main menu
                break;
        }
    }
    return 0;
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

void clearScreen()
{
	int i;
	for(i = 0; i < 80; i++)
	{
		cout << endl;
	}
}

