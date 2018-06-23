//
//  User.h
//  
//
//  Created by Derek Rodriguez on 6/15/18.
//
#include <iostream>
#include <string>
#ifndef User_h
#define User_h

using namespace std;

class User {
    private:
    string accountNumber;
    string pin;
    double checkingAmount;
    double savingsAmount;
    
    
    public:
    // generates new user
    User(string accountNumber, string pin, double savingsAmount, double checkingAmount) {
        this->accountNumber = accountNumber;
        this->pin = pin;
        this->savingsAmount = savingsAmount;
        this->checkingAmount = checkingAmount;
    }
    
    User(string accountNumber, string pin) {
        bool validAccount = validateAccountNumber(accountNumber);
        bool pinValid = validatePin(pin);
        
        if(validAccount && pinValid) {
            this->accountNumber = accountNumber;
            this->pin = pin;
        }
    }
    
    User() {
        savingsAmount = 0;
        checkingAmount = 0;
    }
    
    string withdrawal(double withdrawAmount, string account) {
        if (account == "checking")
            return withdrawFromChecking(withdrawAmount);
        else
            return withdrawFromSavings(withdrawAmount);
    }
    
   string withdrawFromChecking(double withdrawAmount) {
       string success = "Successful withdrawal";
       string error = "Insufficient funds";
        
        if (withdrawAmount > this->checkingAmount)
            return error;
        
        this->checkingAmount -= withdrawAmount;
        return success;
    }
    
    string withdrawFromSavings(double withdrawAmount) {
        string success = "Successful withdrawal";
        string error = "Insufficient funds";
        if (withdrawAmount > this->savingsAmount)
            return error;
        
        this->savingsAmount -= withdrawAmount;
        return success;
    }
    
    string depositToChecking(double depositAmount) {
        this->checkingAmount += depositAmount;
        return "Successful deposit to Checking";
    }
    
    string depositToSavings(double depositAmount) {
        this->savingsAmount += depositAmount;
        return "Successful deposit to Savings";
    }
    
    string transferMoney(string toAccount, double transferAmount) {
        string success = "Successful transfer to " + toAccount + " account.";
        if (toAccount == "checking" && this->savingsAmount >= transferAmount) {
            this->savingsAmount -= transferAmount;
            this->checkingAmount += transferAmount;
        }
        
        if (toAccount == "savings" && this->checkingAmount >= transferAmount) {
            this->checkingAmount -= transferAmount;
            this->savingsAmount += transferAmount;
        }
        
        return success;
    }
    
    bool withdrawAmountValid(double withdrawAmount) {
        return (withdrawAmount >= 10 && withdrawAmount <= 500) ? true : false;
    }
    
    bool validateAccountNumber(string accountNumber) {
        if(accountNumber.length() != 7)
            return false;
        for(int i = 0; i < accountNumber.length(); i++) {
            if(!isdigit(accountNumber[i])) {
                return false;
            }
        }
        return true;
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
    
    bool authenticatePIN(string pin) {
        return (pin == this->pin) ? true : false;
    }
    
    void printCheckingInfo() {
        cout << "Remaining checking account balance: " << this->checkingAmount << "." << endl;
    }

    void printSavingsAmount() {
        cout << "Remaining savings account balance: " << this->savingsAmount << "." << endl;
    }
    
    string getaccountNumber() {
    	return accountNumber;
	}
    
    string getPin() {
        return pin;
    }
    
    double getCheckingAmount() {
        return checkingAmount;
    }
    
    double getSavingsAmount() {
        return savingsAmount;
    }
    
    void setCheckingAmount(double checkingAmount) {
        this->checkingAmount = checkingAmount;
    }
    
    void setSavingsAmount(double savingsAmount) {
        this->savingsAmount = savingsAmount;
    }
    
    void setPin(string pin) {
        this->pin = pin;
    }
    
    void setAccountNumber(string acctNumber) {
        this->accountNumber = acctNumber;
    }
    
};
#endif /* User_h */
