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

int main() {
	
	int state = 0;
	string userinput;
	int i;
	string accountnum;
	string pin;
	bool validAcct = true;
	User *temp;
	vector<User> Accounts;
	
	
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
			for (i = 0; i < Accounts.size(); i++) {
				string checker = Accounts[i].getaccountNumber();
				
				cout << checker << endl;
				cout << Accounts[i].getCheckingAmount() << endl;
       		 	if(!checker.compare(userinput)) {
					  validAcct = false;	
					}	
			 }
			 
			 if(validAcct) {
			 	temp = new User;
			 	temp->setAccountNumber(userinput);
			 	cout << temp->getaccountNumber() << endl; // TEMP, TO CHECK USER LIST AS BEING BUILT
			 	state = 4;
			 	break;
			 }
			
			if(!validAcct) {
				cout << "Account already exists! Try again" << endl;
				state = 2;
				break;
			}
                
			
            case 4: // Enter/validate pin
                cout << "Please enter your desired PIN." << endl;
                cin >> pin;
                
                if (!validatePin) {
                    cout << "Invalid PIN, please try again." << endl;
                    state = 2;
                } else {
                    temp->setPin(pin);
                    state = 4;
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
				state = 0;
				break;
		}
	}
    return 0;
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
