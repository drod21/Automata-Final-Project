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
        
        
    }
    
    void withdrawFromAcct(string account, double withdrawAmount) {
        
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
        if (withdrawAmount > this.checkingAmount)
            return error;
        
        this.savingsAmount -= withdrawAmount;
        return success;
    }
    
    boolean withdrawAmountValid(double withdrawAmount) {
        return (withdrawAmount >= 10 && withdrawAmount <= 500) ? true : false;
    }
    
    boolean validatePIN(int pin) {
        return (pin == this.pin) ? true : false;
    }
    
};
#endif /* User_h */
