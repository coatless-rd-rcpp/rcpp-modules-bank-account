## Exposing _C++_ Classes into _R_ Through Rcpp Modules

<!-- badges: start -->
[![R-CMD-check](https://github.com/coatless-rd-rcpp/rcpp-modules-bank-account/actions/workflows/R-CMD-check.yaml/badge.svg)](https://github.com/coatless-rd-rcpp/rcpp-modules-bank-account/actions/workflows/R-CMD-check.yaml)
<!-- badges: end -->

The `RcppBankAccount` _R_ package provides an example of using [Rcpp Modules](https://cran.r-project.org/web/packages/Rcpp/vignettes/rcpp-modules-bank-account.pdf)
to expose _C++_ classes and their methods to _R_. The example hails from 
working with a `BankAccount` class that is a staple of tutorials on _C++_ classes.

### Usage

To install the package, you must first have a compiler on your system that is
compatible with R. For help on obtaining a compiler consult either
[macOS](http://thecoatlessprofessor.com/programming/r-compiler-tools-for-rcpp-on-os-x/)
or
[Windows](http://thecoatlessprofessor.com/programming/rcpp/install-rtools-for-rcpp/)
guides.

With a compiler in hand, one can then install the package from GitHub by:

```r
# install.packages("remotes")
remotes::install_github("coatless-rd-rcpp/rcpp-modules-bank-account")
library("RcppBankAccount")
```

### Implementation Details

This guide focuses on providing an implementation along the path suggested
in [Rcpp Modules Section 2.2: Exposing C++ classes using Rcpp modules](https://cran.r-project.org/web/packages/Rcpp/vignettes/rcpp-modules-bank-account.pdf#page=4).
In particular, the focus is to expose a pure _C++_ class inside of _R_ without
modifying the underlying _C++_ class. Largely, this means that the _C++_ class
must be "marked up" for export using `RCPP_MODULE( ... )` macro in a separate
file. 

```bash
RcppBankAccount
├── DESCRIPTION                  # Package metadata
├── NAMESPACE                    # Function and dependency registration
├── R
│   ├── RcppExports.R
│   ├── BankAccount-exports.R        # Exporting Rcpp Module's BankAccount into R
│   └── BankAccount-pkg.R            # NAMESPACE Import Code for Rcpp Modules
├── README.md                        # Implementation Overview
├── RcppBankAccount.Rproj
├── man
│   ├── RcppBankAccount-package.Rd
│   └── BankAccount.Rd
└── src
    ├── Makevars                     # Enable C++11
    ├── RcppExports.cpp
    ├── BankAccount.cpp              # Class Implementation
    ├── BankAccount.h                # Class Definition
    └── BankAccount_export.cpp       # Exporting the C++ Class with RCPP_MODULE
```


### C++ Class Definition

Inside of [src/BankAccount.h](src/BankAccount.h), the **definition** of the _C++_ class
is written with an inclusion guard. The definition is a "bare-bones" overview
of what to expect. The meat or the implementation of the class is given in the
[src/BankAccount.cpp](src/BankAccount.cpp) file.

```c++
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
```

#### C++ Class Implementation

In [src/BankAccount.cpp](src/BankAccount.cpp), the **meat** behind the _C++_
class is implemented. The "meat" emphasizes how different methods within the
class should behave. By 

```cpp
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
```

### Writing the Glue

With the class definition and implementation in hand, the task switches to exposing
the definition into _R_ by creating [src/BankAccount_export.cpp](src/BankAccount_export.cpp).
Within this file, the Rcpp Module is defined using the `RCPP_MODULE( ... )` macro.
Through the macro, the class' information must be specified using different
member functions of `Rcpp::class_`. A subset of these member functions is
given next:

- Constructor: 
     - `.default_constructor()`
         - Exposes the default class constructor that does _not_ have any parameters.
     - `.constructor<PARAMTYPE1, PARAMTYPE2>()`
         - Exposes a constructor with recognizable _C++_ data types.
         - e.g. `double`, `int`, `std::string`, and so on.
- Methods: 
     - `.method("FunctionName", &ClassName::FunctionName)`
         - Exposes a class method from `ClassName` given by `FunctionName`.
     - `.property("VariableName", &ClassName::GetFunction, &ClassName::SetFunction )`
         - Indirect access to a Class' fields through getter and setter functions.
- Fields: 
     - `.field("VariableName", &ClassName::VariableName, "documentation for VariableName")`
         - Exposes a public field with **read and write** access from R
     - `.field_readonly("VariableName", &Foo::VariableName, "documentation for VariableName read only")`
         - Exposes a public field with read access from R


```c++
// BankAccount_export.cpp

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
```

### Exporting and documenting an Rcpp Module

In the [R/BankAccount-exports.R](R/BankAccount-exports.R) file, write the load 
statement for the module exposed via the `RCPP_MODULE( ... )` macro. In addition,
make sure to export the class name so that when the package is loaded anyone 
can access it via `new()`.

```r
# Export the "BankAccount" C++ class by explicitly requesting BankAccount be
# exported via roxygen2's export tag.
#' @export BankAccount

loadModule(module = "RcppBankAccountEx", TRUE)
```

### Package Structure

To register the required components for Rcpp Modules, the [`NAMESPACE`](NAMESPACE) file 
must be populated with imports for `Rcpp` and the `methods` _R_ packages.
In addition, the package's dynamic library must be specified as well.

There are two ways to go about this:

1. Let `roxygen2` automatically generate the `NAMESPACE` file; or
2. Manually specify in the `NAMESPACE` file.

The `roxygen2` markup required can be found in [R/RcppBankAccount-package.R](R/RcppBankAccount-package.R).

```r
#' @useDynLib RcppBankAccount, .registration = TRUE
#' @import methods Rcpp
"_PACKAGE"
```

Once the above is run during the documentation generation phase, the
[`NAMESPACE`](NAMESPACE) file will be created with:

```r
# Generated by roxygen2: do not edit by hand

export(BankAccount)
import(Rcpp)
import(methods)
useDynLib(RcppBankAccount, .registration = TRUE)
```

Make sure to build and reload the package prior to accessing methods.

### Calling an Rcpp Module in R

At this point, everything boils down to constructing an object from the class
using `new()` from the `methods` package. The `new()` function initializes a
_C++_ object from the specified _C++_ class and treats it like a 
traditional **S4** object.

```r
##################
## Constructor

# Use a default constructor
jjb_bank = new(BankAccount)

# Supply values to the constructor
pete_bank = new(BankAccount, starting_balance = 1000)

##################
## Setters

jjb_bank$deposit(10)

jjb_bank$withdraw(20)

# This will generate an error
jjb_bank$withdraw(10000)
# Error in jjb_bank$withdraw(10000) : 
#   Current balance is 240 so we cannot withdraw 10000 without going negative.


##################
## Getters

jjb_bank$get_current_balance
# [1] 240
```

## License

GPL (\>= 2)
