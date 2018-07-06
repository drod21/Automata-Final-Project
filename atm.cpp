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

enum States{ MAIN,MAIN_WAIT,REGISTER,REGISTER_CHECK,PIN_ENTER,ACCT_CREATED,
            LOGIN,LOGIN_INVALID,LOGIN_OPTIONS,INVALID_PIN,OPTIONS,WITHDRAW,
            WITHDRAW_CHECKING,WITHDRAW_SAVINGS,DEPOSIT,DEPOSIT_CHECKING,DEPOSIT_SAVINGS,
            CHECK_BALANCE,CHECK_CHECKING,CHECK_SAVINGS,LOCKED_OUT,TRANSFER,TRANSFER_STC,TRANSFER_CTS};

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


                if(userinput == "R" || userinput == "r") {
					state = REGISTER;
					break;
				}

				if(userinput == "L" || userinput == "l") {
					state = LOGIN;
					break;
				}

				cout << "Invalid option " + userinput + " please choose one of the below\n" << endl;
				state = MAIN_WAIT;
				break;


			case REGISTER: // Register (Ask ACCT #, checks if 7 numbers only)
				cout << "Please enter your desired account number, must be 7 numbers only. Q: Quit" << endl;
                cout << "\n==> ";
                cin >> userinput;

                if(userinput == "Q" || userinput == "q") {
                    state = MAIN;
                    break;
                }

                validAcct = validateAccountNumber(userinput);

				if(validAcct) {
					state = REGISTER_CHECK;

                } else {
					cout << "Invalid account #, try again!" << endl;
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
                    cout << "Account already exists! Try again" << endl;
                    state = REGISTER;
                }

                break;

            case PIN_ENTER: // Enter/validate pin
                cout << "Please enter your desired PIN. Q: Quit | B: Back" << endl;
                cout << "\n==> ";
                cin >> pin;

                if(pin == "Q" || pin == "q" || pin == "b" || pin == "B") {
                    state = (pin == "Q") ? MAIN : REGISTER;
                    break;
                }

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
                clearScreen();
                cout << "***** ACCOUNT CREATED! *****" << endl;
                Accounts.push_back(*temp); // AFTER PIN is valid
                state = MAIN_WAIT;
                break;

			case LOGIN: // Login
                cout << "Please enter your 7 digit account number. Q: Quit" << endl;
                cout << "\n==> ";
                cin >> accountNum;
                clearScreen();
                
                if(accountNum == "Q" || accountNum == "q") {
                    state = MAIN;
                    break;
                }
                
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
                clearScreen();
                cout << "+++++++++++++++++++++++++++++++++++++++" << endl;
                cout << "|       Select an option:             |" << endl;
                cout << "|                                     |" << endl;
                cout << "|            W: Withdraw              |" << endl;
                cout << "|            D: Deposit               |" << endl;
		cout << "|            T: Transfer              |" << endl;
                cout << "|            B: Account Balance       |" << endl;
                cout << "|            Q: Quit                  |" << endl;
                cout << "|                                     |" << endl;
                cout << "+++++++++++++++++++++++++++++++++++++++" << endl;
                cout << "\n==> ";
                cin >> userinput;

                if(userinput == "Q" || userinput == "q"){
                    state = MAIN;
                    break;
                }

                if(userinput == "W" || userinput == "w") {
            		state = WITHDRAW;
            		break;
				}

				if(userinput == "D" || userinput == "d") {
					state = DEPOSIT;
					break;
				}

				if(userinput == "B" || userinput == "b") {
					state = CHECK_BALANCE;
					break;
				}

				if(userinput == "t" || userinput == "T")
				{
					state = TRANSFER;
					break;
				}

				cout << "Invalid Option! Please try again." << endl;
				state = OPTIONS;
                break;

			case WITHDRAW: // Withdraw
                cout << "What account would you like to withdraw from? C: Checking | S: Savings | Q: Quit | B: Back" << endl;
                cout << "\n==> ";
                cin >> userinput;

                if(userinput == "Q" || userinput == "q" || userinput == "b" || userinput == "B") {
                    state = (userinput == "Q" || userinput == "q") ? MAIN : OPTIONS;
                    break;
                }

                if(userinput == "C" || userinput == "c") {
                    state = WITHDRAW_CHECKING;
                    break;
                }

                if(userinput == "S" || userinput == "s") {
                    state = WITHDRAW_SAVINGS;
                    break;
                }

                cout << "Invalid option, please try again!" << endl;
                state = WITHDRAW;
                break;


			case WITHDRAW_CHECKING: // Withdraw-Checking
                cout << "How much would you like to withdraw? MIN: $10 | MAX: $500 | Q: Quit | B: Back" << endl;
                cout << "\n==> ";
                cin >> userinput;

                if(userinput == "Q" || userinput == "q") {
                    state =  MAIN;
                    break;
                }

                if(userinput == "B" || userinput == "b"){
                    state = WITHDRAW;
                    break;
                }

                withdrawAmountValid = ATM.withdrawAmountValid(stoi(userinput));

                if(!withdrawAmountValid) {
                    cout << "Invalid withdraw amount, please try again." << endl;
                    state = WITHDRAW_CHECKING;
                    break;
                }

                message = ATM.withdrawal(stoi(userinput), "checking");
                if(message == "Insufficient funds.") {
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
                }
                break;

			case WITHDRAW_SAVINGS: // Withdraw-Savings
                cout << "How much would you like to withdraw? MIN: $10 | MAX: $500 | Q: Quit | B: Back" << endl;
                cout << "\n==> ";
                cin >> userinput;

                if(userinput == "Q" || userinput == "q") {
                    state =  MAIN;
                    break;
                }

                if(userinput == "B" || userinput == "b"){
                    state = WITHDRAW;
                    break;
                }

                if (stoi(userinput) > ATM.getCheckingAmount()) {
                    cout << "Sorry, the ATM does not currently have sufficient funds" << endl;
                    state = WITHDRAW_SAVINGS;
                }
                
                withdrawAmountValid = ATM.withdrawAmountValid(stoi(userinput));
                
                if(!withdrawAmountValid) {
                    cout << "Invalid withdraw amount, please try again." << endl;
                    state = WITHDRAW_SAVINGS;
                    break;
                }
                
                message = ATM.withdrawal(stoi(userinput), "checking");
                if(message == "Insufficient funds.") {
                    cout << "ATM has insufficient funds."<< endl;
                    state = WITHDRAW;
                    break;
                }
                
                message = Accounts[acctIndex].withdrawal(stoi(userinput), "savings");
                state = (message == "Insufficient funds.") ? WITHDRAW_SAVINGS : OPTIONS;
                if(state == WITHDRAW) {
                    cout << message << endl;
                    this_thread::sleep_for (std::chrono::seconds(2));
                } else {
                    Accounts[acctIndex].printSavingsAmount(); // TEMP TO SEE ACCOUNTS CHECKING BALANCE AFTER
                }
                break;

			case DEPOSIT: // Deposit
                cout << "What account would you like to deposit to? C: Checking | S: Savings | Q: Quit | B: Back" << endl;
                cout << "\n==> ";
                cin >> userinput;

                if(userinput == "Q" || userinput == "q" || userinput == "b" || userinput == "B") {
                    state = (userinput == "Q") ? MAIN : OPTIONS;
                    break;
                }

                if(userinput == "C" || userinput == "c") {
                    state = DEPOSIT_CHECKING;
                } else if(userinput == "S" || userinput == "s") {
                    state = DEPOSIT_SAVINGS;
                } else {
                    cout << "Invalid option, try again." << endl;
                    state = DEPOSIT;
                }
                break;


			case DEPOSIT_CHECKING: // Deposit-Checking
                cout << "Enter the amount you would like to deposit? Q: Quit | B: Back" << endl;
                cout << "\n==> ";
                cin >> userinput;

                if(userinput == "Q" || userinput == "q") {
                    state =  MAIN;
                    break;
                }

                if(userinput == "B" || userinput == "b"){
                    state = DEPOSIT;
                    break;
                }

                cout << "Depositing " << stoi(userinput) << " into checking account" << endl;
                Accounts[acctIndex].depositToChecking(stoi(userinput));
                ATM.setCheckingAmount(ATM.getCheckingAmount() + stoi(userinput));

                state = OPTIONS;
                this_thread::sleep_for (std::chrono::seconds(2));
                break;

			case DEPOSIT_SAVINGS: // Deposit-Savings
                cout << "Enter the amount you would like to deposit? Q: Quit | B: Back" << endl;
                cout << "\n==> ";
                cin >> userinput;

                if(userinput == "Q" || userinput == "q") {
                    state =  MAIN;
                    break;
                }

                if(userinput == "B" || userinput == "b"){
                    state = DEPOSIT;
                    break;
                }

                cout << "Depositing " << stoi(userinput) << " into savings account" << endl;
                Accounts[acctIndex].depositToSavings(stoi(userinput));
                ATM.setCheckingAmount(ATM.getCheckingAmount() + stoi(userinput));

                state = OPTIONS;
                this_thread::sleep_for (std::chrono::seconds(2));
                break;

			case CHECK_BALANCE: // Account Balance
                cout << "What account balance would you like to see? C: Checking | S: Savings |  Q: Quit | B: Back" << endl;
                cout << "\n==> ";
                cin >> userinput;

                if(userinput == "Q" || userinput == "q" || userinput == "b" || userinput == "B") {
                    state = (userinput == "Q" || userinput == "q") ? MAIN : OPTIONS;
                    break;
                }

                if(userinput == "C" || userinput == "c") {
                    state = CHECK_CHECKING;
                } else if(userinput == "S" || userinput == "s") {
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
                this_thread::sleep_for (std::chrono::seconds(2));
                state = OPTIONS;
                break;

			case CHECK_SAVINGS: // Acct Bal Savings
                cout << "Your savings account balance is: $" << Accounts[acctIndex].getSavingsAmount() << endl;
                this_thread::sleep_for (std::chrono::seconds(2));
                state = OPTIONS;
                break;

			case LOCKED_OUT: // locked state (NEEDS COMPLETED)
                cout << "LOCKED OUT FOR 30 SECONDS" << endl;
                count = 0;
                Accounts[acctIndex].setTimerLockout();
                state = MAIN_WAIT; //Back to main menu
                break;

			case TRANSFER: // Transfer funds
		cout << "Transfer from: | S: Savings-to-Checking | C: Checking-to-Savings | Q: Quit | B: Back |" << endl;
		cin >> userinput;
		if(userinput == "S" || userinput == "s")
		{
			state = TRANSFER_STC;
			break;
		} 
		
		if(userinput == "C" || userinput == "c")
		{
			state = TRANSFER_CTS;
			break;
		}
		
		if(userinput == "b" || userinput == "B")
		{
			state = OPTIONS;
			break;			
		}
	
		if(userinput == "q" || userinput == "Q")
		{
			state = MAIN;
			break;
		}

		cout << "Invalid option, try again." << endl;
		break;

			case TRANSFER_STC:
			cout << "How much would you like to transfer to your Checking account? | Q: Quit | B: Back" << endl;
			cout << "Current savings balance is: " << Accounts[acctIndex].getSavingsAmount() << endl;
			cin >> userinput;

			if(userinput == "b" || userinput == "B")
			{
				state = TRANSFER;
				break;
			}

			if(userinput == "Q" || userinput == "q")
			{
				state = MAIN;
				break;
			}

			// ADD CHECK THAT USERINPUT WAS DIGIT

			if(stod(userinput) <= Accounts[acctIndex].getSavingsAmount())
			{
				cout << userinput << " transferred to checking." << endl;
				cout << "New savings account balance: " << Accounts[acctIndex].getSavingsAmount() << endl; // For some reason not appearing
				cout << "New Checking account balance: " << Accounts[acctIndex].getCheckingAmount() << endl; // For some reason not appearing
				Accounts[acctIndex].transferMoney("checking", stod(userinput));
				state = OPTIONS;
				break;
			}
			

			cout << "Invalid amount / option, try again." << endl;
			break;
			
			case TRANSFER_CTS:
		
			cout << "How much would you like to transfer to your Savings account? | Q: Quit | B: Back" << endl;
			cout << "Current checking balance is: " << Accounts[acctIndex].getCheckingAmount() << endl;
			cin >> userinput;

			if(userinput == "b" || userinput == "B")
			{
				state = TRANSFER;
				break;
			}

			if(userinput == "Q" || userinput == "q")
			{
				state = MAIN;
				break;
			}

			// ADD CHECK THAT USERINPUT WAS DIGIT

			if(stod(userinput) <= Accounts[acctIndex].getCheckingAmount())
			{
				cout << userinput << " transferred to checking." << endl;
				cout << "New savings account balance: " << Accounts[acctIndex].getSavingsAmount() << endl; // For some reason not appearing
				cout << "New Checking account balance: " << Accounts[acctIndex].getCheckingAmount() << endl; // For some reason not appearing
				Accounts[acctIndex].transferMoney("savings", stod(userinput));
				state = OPTIONS;
				break;
			}
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

void clearScreen()
{
    if (!cur_term)
    {
        int result;
        setupterm( NULL, STDOUT_FILENO, &result );
        if (result <= 0) return;
    }

    putp( tigetstr( "clear" ) );
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

