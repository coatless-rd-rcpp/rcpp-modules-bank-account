//  BankAccount.cpp

// Required to use stop()
#include <Rcpp.h>

// Retrieve the definition of our BankAccount class
#include "BankAccount.h"

// Constructors ----
BankAccount::BankAccount() {
  current_balance = 250;
}

BankAccount::BankAccount(int starting_balance) {
  current_balance = starting_balance;
}

// Modifiers ----
void BankAccount::deposit(int amount) {
  current_balance += amount;
}

void BankAccount::withdraw(int amount) {
  if (current_balance >= amount) {
    current_balance -= amount;
  } else {
    Rcpp::stop("Current balance is %s so we cannot withdraw %s without going negative.", current_balance, amount);
  }
}

// Accessors ----
int BankAccount::get_current_balance() {
  return current_balance;
}
