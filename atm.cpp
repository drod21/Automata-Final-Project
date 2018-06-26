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

int main() {
	
	int state = 0;
	string userinput;
	int i;
	string accountNum;
	string pin;
	bool validAcct = true;
    int acctIndex;
	User *temp;
	vector<User> Accounts;
    int count = 0;
    double usernumberinput;
    double difference;
    time_t curtime;
    
    string message;
    
    User ATM;
    ATM.setCheckingAmount(500);
    cout << ATM.getCheckingAmount() << endl; // TEMP (STARTING AMOUNT IN ATM)
	
	while(true) {
		switch(state) {
			case 0: // Main
				cout << "Welcome to AMDO ATM!" << endl;
				state = 1;
				break;
				
			case 1: // Mainwait
				cout << "R: Register|L: Login"<< endl;
				cin >> userinput;
				
				cout << userinput << endl;
				
                if(userinput == "R") {
					state = 2;
					break;
				}
				
				if(userinput == "L") {
					state = 11;
					break;
				}
				
				cout << "Invalid option,  please choose one of the below" << endl;
				state = 1;
				break;
				
				
			case 2: // Register (Ask ACCT #, checks if 7 numbers only)
				cout << "Please enter your desired account number, must be 7 numbers only" << endl;
				cin >> userinput;
				
                 validAcct = validateAccountNumber(userinput);
				
				if(validAcct) {
					state = 3;
					
                } else {
					cout << "Invalid account #, try again!" << endl;
					state = 2;
					validAcct = true;
				}
                
                break;
                
			case 3: // Register (Checks if acct exists)
                cout << "LIST OF USERS" << endl;
                acctIndex = findAccount(Accounts, userinput);
                
                if(acctIndex == -1) {
                    temp = new User;
                    temp->setAccountNumber(userinput);
                    cout << temp->getaccountNumber() << endl; // TEMP, TO CHECK USER LIST AS BEING BUILT 
                    state = 4;
                } else {
                    cout << "Account already exists! Try again" << endl;
                    state = 2;
                }
			
                break;
			
            case 4: // Enter/validate pin
                cout << "Please enter your desired PIN." << endl;
                cin >> pin;
                
                if (!validatePin(pin)) {
                    cout << "Invalid PIN, please try again." << endl;
                    state = 4;
                } else {
                    temp->setPin(pin);
                    temp->setCheckingAmount(400); // TEMP, GIVING STARTING BALANCE TO TEST WITHDRAW + DEPOSIT
                    temp->setSavingsAmount(400); // TEMP, GIVING STARTING BALANCE TO TEST WITHDRAW + DEPOSIT
                    state = 5;
                }
                break;
                
            case 5: // REQUEST + VALIDATE PIN before creating user
                cout << "ACCT CREATED!" << endl;
                
                Accounts.push_back(*temp); // AFTER PIN is valid
                // TEMP, JUST MAKING SURE USERS ARE BEING ADDED TO LIST CORRECTLY
                for(i = 0; i < Accounts.size(); i++) {
                	cout << Accounts[i].getaccountNumber() << endl;
                	cout << Accounts[i].getPin() << endl;
				}
                
                state = 1;
                break;
            
			case 11: // Login
                cout << "Please enter your 7 digit account number." << endl;
                cin >> accountNum;
                acctIndex = findAccount(Accounts, accountNum);
                if (acctIndex >= 0)
                    state = 13;
                else
                    state = 12;
				break;
            case 12: // reset state for login
                cout << "Invalid account number, please try again." << endl;
                state = 11;
                break;
                
            case 13: //state for account options
            	time(&curtime);
            	cout << curtime << endl;
            	 difference = difftime(curtime,Accounts[acctIndex].getTimerLockout());
            	if(difference >= 30) {
	            	cout << "Please input your 4 digit PIN number." << endl;
	                cin >> pin;
	                
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
                cout << "Invalid PIN. " << 3-count << " Attempts remaining, Please try again." << endl;
                break;
                
            case 15:
                cout << "Please select an option; W: Withdraw | D: Deposit | B: Account Balance" << endl;
                cin >> userinput;
                if(userinput == "W") {
            		state = 16;
            		break;
				}
				
				if(userinput == "D") {
					state = 17;
					break;
				}
				
				if(userinput == "B") {
					state = 18;
					break;
				}
				
				cout << "Invalid Option! Please try again." << endl;
				state = 15;
                break;
                
			case 16: // Withdraw
                cout << "What account would you like to withdraw from? C: Checking | S: Savings" << endl;
                cin >> userinput;
                
                if(userinput == "C") {
                    state = 161;
                    break;
                }
                
                if(userinput == "S") {
                    state = 162;
                    break;
                }
                
                cout << "Invalid option, please try again!" << endl;
                state = 16;
                break;
			
			
			case 161: // Withdraw-Checking
                cout << "How much would you like to withdraw? MIN: $10 | MAX: $500" << endl;
                cin >> usernumberinput;
                
                if (usernumberinput > ATM.getCheckingAmount()) {
                    cout << "Sorry, the ATM does not currently have sufficient funds" << endl;
                    state = 161;
                }
                
                if(Accounts[acctIndex].withdrawAmountValid(usernumberinput)) {
                    message = Accounts[acctIndex].withdrawFromChecking(usernumberinput);
                    if(message == "Insufficient funds")
                        state = 161;
                    else
                        state = 15;
                    cout << Accounts[acctIndex].getCheckingAmount() << endl; // TEMP TO SEE ACCOUNTS CHECKING BALANCE AFTER
                    cout << ATM.getCheckingAmount() << endl; // TEMP TO SEE ATM CHECKING BALANCE AFTER
                    state = 15;
                    break;
                    if(usernumberinput > Accounts[acctIndex].getCheckingAmount()) {
                        cout << "Insufficient funds: Try again" << endl;
                        state = 161;
                    }
                    break;
                } else {
                    cout << "Invalid withdraw amount, please try again!" << endl;
                    state = 161;
                }
                
                break;
			
			case 162: // Withdraw-Savings
                cout << "How much would you like to withdraw? MIN: $10 | MAX: $500" << endl;
                cin >> usernumberinput;
                
                if (usernumberinput > ATM.getCheckingAmount()) {
                    cout << "Sorry, the ATM does not currently have sufficient funds" << endl;
                    state = 162;
                }
                
                
                if(Accounts[acctIndex].withdrawAmountValid(usernumberinput)) {
                    message = Accounts[acctIndex].withdrawFromSavings(usernumberinput);
                    if(message == "Insufficient funds")
                        state = 162;
                    else
                        state = 15;
                    cout << Accounts[acctIndex].getCheckingAmount() << endl; // TEMP TO SEE ACCOUNTS CHECKING BALANCE AFTER
                    cout << ATM.getCheckingAmount() << endl; // TEMP TO SEE ATM CHECKING BALANCE AFTER
                } else {
                    cout << "Invalid withdraw amount, please try again!" << endl;
                    state = 162;
                }
                
                break;
			case 17: // Deposit
                cout << "What account would you like to deposit to? C: Checking | S: Savings" << endl;
                cin >> userinput;
                
                if(userinput == "C") {
                    state = 171;
                } else if(userinput == "S") {
                    state = 172;
                } else {
                    cout << "Invalid option, try again!" << endl;
                    state = 17;
                }
                break;
			
			
			case 171: // Deposit-Checking
                cout << "Enter the amount you would like to deposit?" << endl;
                cin >> usernumberinput;
                
                cout << "Depositing " << usernumberinput << " into checking account" << endl;
                Accounts[acctIndex].depositToChecking(usernumberinput);
                ATM.setCheckingAmount(ATM.getCheckingAmount() + usernumberinput);
                
                cout << Accounts[acctIndex].getCheckingAmount() << endl; // TEMP, CHECKING AMOUNTS CHANGED
                cout << ATM.getCheckingAmount() << endl; // TEMP, CHECKING AMOUNTS CHANGED
                state = 15;
                break;
			
			case 172: // Deposit-Savings
                cout << "Enter the amount you would like to deposit?" << endl;
                cin >> usernumberinput;
                
                cout << "Depositing " << usernumberinput << " into savings account" << endl;
                Accounts[acctIndex].depositToSavings(usernumberinput);
                ATM.setCheckingAmount(ATM.getCheckingAmount() + usernumberinput);
                
                cout << Accounts[acctIndex].getSavingsAmount() << endl; // TEMP, CHECKING AMOUNTS CHANGED
                cout << ATM.getCheckingAmount() << endl; // TEMP, CHECKING AMOUNTS CHANGED
                state = 15;
                break;
			
			case 18: // Account Balance
                cout << "What account balance would you like to see? C: Checking | S: Savings" << endl;
                cin >> userinput;
                
                if(userinput == "C") {
                    state = 181;
                    break;
                } else if(userinput == "S") {
                    state = 182;
                    break;
                } else {
                    cout << "Invalid option, try again" << endl;
                    state = 18;
                    break;
                }
                break;
			
			case 181: // Acct Bal Checking
                cout << "Your checking account balance is: $" << Accounts[acctIndex].getCheckingAmount() << endl;
                state = 15;
                break;
			
			case 182: // Acct Bal Savings
                cout << "Your savings account balance is: $" << Accounts[acctIndex].getSavingsAmount() << endl;
                state = 15;
                break;
			
			case 26: // locked state (NEEDS COMPLETED) 
                cout << "LOCKED OUT FOR 30 SECONDS" << endl;
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

