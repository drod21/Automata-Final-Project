// this file will be our main file
#include <stdlib.h>
#include "User.h"
#include <iostream>
#include <string>
// #include <iterator>
#include <vector>
// #include <sstream>

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
                    state = 5;
                }
                break;
                
            case 5: // REQUEST + VALIDATE PIN before creating user
                cout << "ACCT CREATED!" << endl;
                
                Accounts.push_back(*temp); // AFTER PIN is valid
                state = 1;
                break;
            
			case 11: // Login
				cout << "OPTION NOT YET AVAILABLE, PLEASE TRY AGAIN LATER." << endl;
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
                cout << "Please input your 4 digit PIN number." << endl;
                cin >> pin;
                
                if(Accounts[acctIndex] != pin && count < 2) {
                    state = 14;
                } else if(count == 2) {
                    state = 26; // Locked state;
                } else {
                    state = 15;
                }
                    
                break;
            case 14:
                cout << "Invalid PIN. Please try again." << endl;
                state = 13;
                count++;
                break;
            case 15:
                cout << "Loggined in menu goes here" << endl;
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
