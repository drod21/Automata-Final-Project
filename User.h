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

class User {
    private:
    int accountNumber;
    int pin;
    double checkingAmount;
    double savingsAmount;
    
    public:
    // generates new user
    User(int accountNumber, int pin) {
        this->accountNumber = accountNumber;
        // whatever else we need to do for this.
        
    }
    
   std::string withdrawFromChecking(double withdrawAmount) {
        std::string success = "Successful withdrawal";
       std::string error = "Insufficient funds";
        
        if (withdrawAmount > this->checkingAmount)
            return error;
        
        this->checkingAmount -= withdrawAmount;
        return success;
    }
    
    std::string withdrawFromSavings(double withdrawAmount) {
        std::string success = "Successful withdrawal";
        std::string error = "Insufficient funds";
        if (withdrawAmount > this->savingsAmount)
            return error;
        
        this->savingsAmount -= withdrawAmount;
        return success;
    }
    
    bool withdrawAmountValid(double withdrawAmount) {
        return (withdrawAmount >= 10 && withdrawAmount <= 500) ? true : false;
    }
    
    bool validatePIN(int pin) {
        return (pin == this->pin) ? true : false;
    }
    
    int getaccountNumber()
    {
    	return accountNumber;
	}
    
};
#endif /* User_h */
