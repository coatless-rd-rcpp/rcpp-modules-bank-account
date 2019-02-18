// Include Rcpp system header file (e.g. <>)
#include <Rcpp.h>

// Include our definition of the BankAccount file (e.g. "")
#include "BankAccount.h"

// Expose (some of) the Student class
RCPP_MODULE(RcppBankAccountEx){
  Rcpp::class_<BankAccount>("BankAccount")
  .default_constructor()
  .constructor<int>()
  .method("deposit", &BankAccount::deposit)
  .method("withdraw", &BankAccount::withdraw)
  .property("get_current_balance", &BankAccount::get_current_balance);
}
