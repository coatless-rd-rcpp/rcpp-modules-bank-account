
#' Create a BankAccount Object from the BankAccount C++ Class
#'
#' Allows for the creation of a BankAccount Object in _C++_ from _R_
#' using the _C++_ BankAccount class.
#'
#' @param starting_balance Amount in the BankAccount. Default: Start with 250.
#'
#' @return
#' A `BankAccount` object from the _C++_ BankAccount Class.
#'
#' @examples
#' ##################
#' ## Constructor
#'
#' # Construct new BankAccount object called "jjb_bank"
#' jjb_bank = new(BankAccount)
#'
#' ##################
#' ## Setters
#'
#' jjb_bank$deposit(10)
#' jjb_bank$withdraw(20)
#'
#' ##################
#' ## Getters
#'
#' jjb_bank$get_current_balance
#'
#' @name BankAccount
#' @export BankAccount

# ^^^^^^^^^^^^^^^^
# Export the "BankAccount" C++ class by explicitly requesting BankAccount be
# exported via roxygen2's export tag.
# Also, provide a name for the Rd file.

# Load the Rcpp module exposed with RCPP_MODULE( ... ) macro.
loadModule(module = "RcppBankAccountEx", TRUE)
