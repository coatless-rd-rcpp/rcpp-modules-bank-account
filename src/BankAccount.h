//  BankAccount.h

#ifndef BankAccount_H
#define BankAccount_H

class BankAccount
{

public:

  // Constructors
  BankAccount();
  BankAccount(int starting_balance);

  // Modifiers
  void deposit(int amount);
  void withdraw(int amount);

  // Accessor
  int get_current_balance();

private:

  // Member variables
  int current_balance;
};

#endif /* BankAccount_H */
