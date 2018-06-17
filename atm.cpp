// this file will be our main file
#include <stdlib.h>
#include "User.h"
#include <iostream>
#include <string>
#include <iterator>
#include <vector>

using namespace std;
int main() {
	
	int state = 0;
	string userinput;
	int i;
	int accountnum;
	int pin;
	bool valid = true;
	vector<User> Accounts;
	vector<User>::iterator iter;
	
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
				
				
			case 2:
				cout << "Please enter your desired account number, must be 7 numbers only" << endl;
				cin >> userinput;
				valid = true;
				
				for(i = 0; i < userinput.length() ;i++) {
					if(!isdigit(userinput[i]) || userinput.length() != 7) {
						valid = false;
					}
				}
				
				if(valid == true) {
					state = 3;
					break;
				}
				
				if(valid == false) {
					cout << "Invalid account #, try again!" << endl;
					state = 2;
					break;
				}
				
				
			case 3: // CheckAcctExists
		
		/*	for (iter = Accounts.begin(); iter < Accounts.end(); iter++) {
       		 	if(*iter.getaccountNumber() == (int)userinput) {
					  valid = false;	
					}	
			 }
			 
			 if(valid == true) {
			 	User temp = new User;
			 	temp.getaccountNumber() = (int)userinput;
			 	cout << temp.getaccountNumber() << endl;
			 	state = 4;
			 	break;
			 }
			
			*/
			cout << "valid test" << endl;
			state = 1;
			break;
			case 11: // Login
				cout << "OPTION NOT YET AVAILABLE, PLEASE TRY AGAIN LATER." << endl;
				state = 0;
				break;
		}
	}
    return 0;
}
