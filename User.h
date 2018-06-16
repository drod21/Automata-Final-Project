//
//  User.h
//  
//
//  Created by Derek Rodriguez on 6/15/18.
//
#include <iostream>
#ifndef User_h
#define User_h

class User {
    private:
    int accountNumber;
    int pin;
    double checkingAmount;
    double savingsAmount;
    
    public:
    // generates new user
    User(int accountNumber, int pin) {
        this.accountNumber = accountNumber;
        // whatever else we need to do for this.
        
    }
    
    string withdrawFromChecking(double withdrawAmount) {
        string success = "Successful withdrawal";
        string error = "Insufficient funds";
        
        if (withdrawAmount > this.checkingAmount)
            return error;
        
        this.checkingAmount -= withdrawAmount;
        return success;
    }
    
    string withdrawFromSavings(double withdrawAmount) {
        string success = "Successful withdrawal";
        string error = "Insufficient funds";
        if (withdrawAmount > this.savingsAmount)
            return error;
        
        this.savingsAmount -= withdrawAmount;
        return success;
    }
    
    string depositToChecking(double depositAmount) {
        this.checkingAmount += depositAmount;
        return "Successful deposit to Checking";
    }
    
    string depositToSavings(double depositAmount) {
        this.savingsAmount += depositAmount;
        return "Successful deposit to Savings";
    }
    
    string transferMoney(string toAccount, double transferAmount) {
        string success = "Successful transfer to " + toAccount + " account.";
        if (toAccount == "checking" && this.savingsAmount >= transferAmount) {
            this.savingsAmount -= transferAmount;
            this.checkingAmount += transferAmount;
        }
        
        if (toAccount == "savings" && this.checkingAmount >= transferAmount) {
            this.checkingAmount -= transferAmount;
            this.savingsAmount += transferAmount;
        }
        
        return success;
    }
    
    boolean withdrawAmountValid(double withdrawAmount) {
        return (withdrawAmount >= 10 && withdrawAmount <= 500) ? true : false;
    }
    
    boolean validatePIN(int pin) {
        return (pin == this.pin) ? true : false;
    }
    
    void printCheckingInfo() {
        cout << "Remaining Checking account balance"
    }
};
#endif /* User_h */
