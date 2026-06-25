# Create a BankAccount Object from the BankAccount C++ Class

Allows for the creation of a BankAccount Object in *C++* from *R* using
the *C++* BankAccount class.

## Arguments

- starting_balance:

  Amount in the BankAccount. Default: Start with 250.

## Value

A `BankAccount` object from the *C++* BankAccount Class.

## Examples

``` r
##################
## Constructor

# Construct new BankAccount object called "jjb_bank"
jjb_bank = new(BankAccount)

##################
## Setters

jjb_bank$deposit(10)
jjb_bank$withdraw(20)

##################
## Getters

jjb_bank$get_current_balance
#> [1] 240
```
