// this file will be our main file
#include <stdlib.h>
#include "User.h"
#include <iostream>
#include <string>
#include <time.h>
#include <vector>
#include <thread>
#include <mutex>

#define TIME_OUT_VAL 5 // 300 seconds = 5 minutes

using namespace std;
bool validateAccountNumber(string acctNumber);
bool validatePin(string pin);
int findAccount(vector<User> accounts, string accountNum);
void timer();

enum States{ MAIN,MAIN_WAIT,REGISTER,REGISTER_CHECK,PIN_ENTER,ACCT_CREATED,
            LOGIN,LOGIN_INVALID,LOGIN_OPTIONS,INVALID_PIN,OPTIONS,WITHDRAW,
            WITHDRAW_CHECKING,WITHDRAW_SAVINGS,DEPOSIT,DEPOSIT_CHECKING,DEPOSIT_SAVINGS,
            CHECK_BALANCE,CHECK_CHECKING,CHECK_SAVINGS,LOCKED_OUT};

States state = MAIN;
time_t start = time(0);
mutex m;

int main() {
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

    thread timer_thread(timer);
    User ATM;
    ATM.setCheckingAmount(500);
    /*cout << ATM.getCheckingAmount() << endl; // TEMP (STARTING AMOUNT IN ATM)*/

	while(true) {
		switch(state) {
			case MAIN: // Main
				cout << "******** Welcome to AMDO ATM! *********" << endl;

				state = MAIN_WAIT;
				break;

			case MAIN_WAIT: // Mainwait
                cout << "+++++++++++++++++++++++++++++++++++++++" << endl;
                cout << "|                                     |" << endl;
				cout << "|      R: Register    L: Login        |" << endl;
                cout << "|                                     |" << endl;
                cout << "+++++++++++++++++++++++++++++++++++++++" << endl;
                cout << "\n==> ";
				cin >> userinput;


                if(userinput == "R") {
					state = REGISTER;
					break;
				}

				if(userinput == "L") {
					state = LOGIN;
					break;
				}

				cout << "Invalid option " + userinput + " please choose one of the below\n" << endl;
				state = MAIN_WAIT;
				break;


			case REGISTER: // Register (Ask ACCT #, checks if 7 numbers only)
				cout << "Please enter your desired account number, must be 7 numbers only" << endl;
                cout << "\n==> ";
                cin >> userinput;

                 validAcct = validateAccountNumber(userinput);

				if(validAcct) {
					state = REGISTER_CHECK;

                } else {
					cout << "Invalid account #, try again!" << endl;
					state = REGISTER;
					validAcct = true;
				}

                break;

			case REGISTER_CHECK: // Register (Checks if acct exists)
                acctIndex = findAccount(Accounts, userinput);

                if(acctIndex == -1) {
                    temp = new User;
                    temp->setAccountNumber(userinput);
                    /*cout << temp->getaccountNumber() << endl; // TEMP, TO CHECK USER LIST AS BEING BUILT */
                    state = PIN_ENTER;
                } else {
                    cout << "Account already exists! Try again" << endl;
                    state = REGISTER;
                }

                break;

            case PIN_ENTER: // Enter/validate pin
                cout << "Please enter your desired PIN." << endl;
                cout << "\n==> ";
                cin >> pin;

                if (!validatePin(pin)) {
                    cout << "INVALID PIN, please try again." << endl;
                    state = PIN_ENTER;
                } else {
                    temp->setPin(pin);
                    temp->setCheckingAmount(400); // TEMP, GIVING STARTING BALANCE TO TEST WITHDRAW + DEPOSIT
                    temp->setSavingsAmount(400); // TEMP, GIVING STARTING BALANCE TO TEST WITHDRAW + DEPOSIT
                    state = ACCT_CREATED;
                }
                break;

            case ACCT_CREATED: // REQUEST + VALIDATE PIN before creating user
                cout << "***** ACCOUNT CREATED! *****" << endl;

                Accounts.push_back(*temp); // AFTER PIN is valid
                // TEMP, JUST MAKING SURE USERS ARE BEING ADDED TO LIST CORRECTLY
                /*for(i = 0; i < Accounts.size(); i++) {
                	cout << Accounts[i].getaccountNumber() << endl;
                	cout << Accounts[i].getPin() << endl;
				}*/

                state = MAIN_WAIT;
                break;

			case LOGIN: // Login
                cout << "Please enter your 7 digit account number." << endl;
                cout << "\n==> ";
                cin >> accountNum;
                acctIndex = findAccount(Accounts, accountNum);
                if (acctIndex >= 0)
                    state = LOGIN_OPTIONS;
                else
                    state = LOGIN_INVALID;
				break;

            case LOGIN_INVALID: // reset state for login
                cout << "Invalid account number, please try again." << endl;
                state = LOGIN;
                break;

            case LOGIN_OPTIONS: //state for account options
            	time(&curtime);
            	//cout << curtime << endl;
                difference = difftime(curtime,Accounts[acctIndex].getTimerLockout());
            	if(difference >= 30) {
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
				} else {
					cout << "Account locked for " << 30 - difference << " more seconds." <<endl;
					state = MAIN_WAIT;
					break;
				}


                break;

            case INVALID_PIN:
                state = LOGIN_OPTIONS;
                count++;
                cout << "INVALID PIN. " << 3-count << " Attempts remaining, Please try again." << endl;
                break;

            case OPTIONS:
                cout << endl;
                cout << "+++++++++++++++++++++++++++++++++++++++" << endl;
                cout << "|       Select an option:             |" << endl;
                cout << "|                                     |" << endl;
                cout << "|            W: Withdraw              |" << endl;
                cout << "|            D: Deposit               |" << endl;
                cout << "|            B: Account Balance       |" << endl;
                cout << "|                                     |" << endl;
                cout << "+++++++++++++++++++++++++++++++++++++++" << endl;
                cout << "\n==> ";
                cin >> userinput;

                if(userinput == "W") {
            		state = WITHDRAW;
            		break;
				}

				if(userinput == "D") {
					state = DEPOSIT;
					break;
				}

				if(userinput == "B") {
					state = CHECK_BALANCE;
					break;
				}

				cout << "Invalid Option! Please try again." << endl;
				state = OPTIONS;
                break;

			case WITHDRAW: // Withdraw
                cout << "What account would you like to withdraw from? C: Checking | S: Savings" << endl;
                cout << "\n==> ";
                cin >> userinput;

                if(userinput == "C") {
                    state = WITHDRAW_CHECKING;
                    break;
                }

                if(userinput == "S") {
                    state = WITHDRAW_SAVINGS;
                    break;
                }

                cout << "Invalid option, please try again!" << endl;
                state = WITHDRAW;
                break;


			case WITHDRAW_CHECKING: // Withdraw-Checking
                cout << "How much would you like to withdraw? MIN: $10 | MAX: $500" << endl;
                cout << "\n==> ";
                cin >> usernumberinput;

                if (usernumberinput > ATM.getCheckingAmount()) {
                    cout << "Sorry, the ATM does not currently have sufficient funds" << endl;
                    state = WITHDRAW_CHECKING;
                }

                if(Accounts[acctIndex].withdrawAmountValid(usernumberinput)) {
                    message = Accounts[acctIndex].withdrawFromChecking(usernumberinput);
                    state = OPTIONS;
                    /*cout << Accounts[acctIndex].getCheckingAmount() << endl; // TEMP TO SEE ACCOUNTS CHECKING BALANCE AFTER
                    cout << ATM.getCheckingAmount() << endl; // TEMP TO SEE ATM CHECKING BALANCE AFTER*/
                    cout << message << endl;
                    break;
                } else {
                    cout << "Invalid withdraw amount, please try again!" << endl;
                    state = WITHDRAW_CHECKING;
                }

                break;

			case WITHDRAW_SAVINGS: // Withdraw-Savings
                cout << "How much would you like to withdraw? MIN: $10 | MAX: $500" << endl;
                cout << "\n==> ";
                cin >> usernumberinput;

                if (usernumberinput > ATM.getCheckingAmount()) {
                    cout << "Sorry, the ATM does not currently have sufficient funds" << endl;
                    state = WITHDRAW_SAVINGS;
                }


                if(Accounts[acctIndex].withdrawAmountValid(usernumberinput)) {
                    message = Accounts[acctIndex].withdrawFromSavings(usernumberinput);
                    state = OPTIONS;
                    /*cout << Accounts[acctIndex].getCheckingAmount() << endl; // TEMP TO SEE ACCOUNTS CHECKING BALANCE AFTER
                    cout << ATM.getCheckingAmount() << endl; // TEMP TO SEE ATM CHECKING BALANCE AFTER*/
                    cout << message << endl;
                } else {
                    cout << "Invalid withdraw amount, please try again!" << endl;
                    state = WITHDRAW_SAVINGS;
                }

                break;
			case DEPOSIT: // Deposit
                cout << "What account would you like to deposit to? C: Checking | S: Savings" << endl;
                cout << "\n==> ";
                cin >> userinput;

                if(userinput == "C") {
                    state = DEPOSIT_CHECKING;
                } else if(userinput == "S") {
                    state = DEPOSIT_SAVINGS;
                } else {
                    cout << "Invalid option, try again!" << endl;
                    state = DEPOSIT;
                }
                break;


			case DEPOSIT_CHECKING: // Deposit-Checking
                cout << "Enter the amount you would like to deposit?" << endl;
                cout << "\n==> ";
                cin >> usernumberinput;

                cout << "Depositing " << usernumberinput << " into checking account" << endl;
                Accounts[acctIndex].depositToChecking(usernumberinput);
                ATM.setCheckingAmount(ATM.getCheckingAmount() + usernumberinput);

        /*        cout << Accounts[acctIndex].getCheckingAmount() << endl; // TEMP, CHECKING AMOUNTS CHANGED
                cout << ATM.getCheckingAmount() << endl; // TEMP, CHECKING AMOUNTS CHANGED*/
                state = OPTIONS;
                break;

			case DEPOSIT_SAVINGS: // Deposit-Savings
                cout << "Enter the amount you would like to deposit?" << endl;
                cout << "\n==> ";
                cin >> usernumberinput;

                cout << "Depositing " << usernumberinput << " into savings account" << endl;
                Accounts[acctIndex].depositToSavings(usernumberinput);
                ATM.setCheckingAmount(ATM.getCheckingAmount() + usernumberinput);

                /*cout << Accounts[acctIndex].getSavingsAmount() << endl; // TEMP, CHECKING AMOUNTS CHANGED
                cout << ATM.getCheckingAmount() << endl; // TEMP, CHECKING AMOUNTS CHANGED*/
                state = OPTIONS;
                break;

			case CHECK_BALANCE: // Account Balance
                cout << "What account balance would you like to see? C: Checking | S: Savings" << endl;
                cout << "\n==> ";
                cin >> userinput;

                if(userinput == "C") {
                    state = CHECK_CHECKING;
                    break;
                } else if(userinput == "S") {
                    state = CHECK_SAVINGS;
                    break;
                } else {
                    cout << "Invalid option, try again" << endl;
                    state = CHECK_BALANCE;
                    break;
                }
                break;

			case CHECK_CHECKING: // Acct Bal Checking
                cout << "Your checking account balance is: $" << Accounts[acctIndex].getCheckingAmount() << endl;
                state = OPTIONS;
                break;

			case CHECK_SAVINGS: // Acct Bal Savings
                cout << "Your savings account balance is: $" << Accounts[acctIndex].getSavingsAmount() << endl;
                state = OPTIONS;
                break;

			case LOCKED_OUT: // locked state (NEEDS COMPLETED)
                cout << "LOCKED OUT FOR 30 SECONDS" << endl;
                count = 0;
                Accounts[acctIndex].setTimerLockout();
                state = MAIN_WAIT; //Back to main menu
                break;
		}
	}
    return 0;
}

void timer(){
    while(true){
        double seconds_since_start = difftime( time(0), start);
        if(seconds_since_start == TIME_OUT_VAL){
            cout << "ATM TIMED OUT" << endl;
            std::lock_guard<std::mutex> lock(m);
            state = MAIN;
            break;
        }
    }
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

