//
//  User.h
//  
//
//  Created by Derek Rodriguez on 6/15/18.
//
#include <iostream>
#include <string>
#include <time.h>
#ifndef User_h
#define User_h

using namespace std;

class User {
    private:
        string accountNumber;
        string pin;
        double checkingAmount;
        double savingsAmount;
        time_t lockoutTimer;

public:
    // generates new user
    User(string accountNumber, string pin, double savingsAmount, double checkingAmount) {
        this->accountNumber = accountNumber;
        this->pin = pin;
        this->savingsAmount = savingsAmount;
        this->checkingAmount = checkingAmount;
        this->lockoutTimer = 0;
    }
    
    User(string accountNumber, string pin) {
        this->accountNumber = accountNumber;
        this->pin = pin;
        this->lockoutTimer = 0;
    }
    
    User() {
        savingsAmount = 0;
        checkingAmount = 0;
        lockoutTimer = 0;
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
        
        if (!sufficientFunds(withdrawAmount, "checking"))
            return error;
        
        this->checkingAmount -= withdrawAmount;
        return success;
    }
    
    string withdrawFromSavings(double withdrawAmount) {
        string success = "Successful withdrawal";
        string error = "Insufficient funds";

        if (!sufficientFunds(withdrawAmount, "savings"))
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
        string message = "Successful transfer to " + toAccount + " account.";
        
        if (toAccount == "checking" && sufficientFunds(transferAmount, "savings")) {
            this->savingsAmount -= transferAmount;
            this->checkingAmount += transferAmount;
        } else if (toAccount == "savings" && sufficientFunds(transferAmount, "checking")) {
            this->checkingAmount -= transferAmount;
            this->savingsAmount += transferAmount;
        } else {
            message = "Insufficient funds in " + toAccount + " for transfer."
        }
        
        return message;
    }
    
    bool sufficientFunds(double withdrawAmount, string account) {
        return (account == "checking") ? (withdrawAmount <= this->checkingAmount) : (withdrawAmount <= this->savingsAmount);
    }

    bool withdrawAmountValid(double withdrawAmount) {
        return (withdrawAmount > 9 && withdrawAmount < 501) ? true : false;
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
    
    void printCheckingAmount() {
        cout << "Checking account balance: $" << this->checkingAmount << "." << endl;
    }

    void printSavingsAmount() {
        cout << "Savings account balance: $" << this->savingsAmount << "." << endl;
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
    
    time_t getTimerLockout() {
        return this->lockoutTimer;
    }

    void setTimerLockout() {
        time(&lockoutTimer);
    }
};
#endif /* User_h */
