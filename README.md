Introduction
============

The aim of this vignette is to describe all the aspects of Rcpp Modules,
from the very basic to the more advanced. If your not familiar with Rcpp
please refer to Allaire, Eddelbuettel and Francois (2017). Some C++
knowledge is assumed but only at a basic level. I’m not a C++ expert
either (I did get advice though). Stubbornness might help. The code does
not include any functionality except for printing and multiplying
strings by design. The only goal of the package is to give minimal
examples of all Rcpp Modules features. This vignette is contained in a
fully functional Rcpp package.

The following sections contain a step by step guide of creating the
package, starting with the most basic features.

All steps can be referred to by their own branch, to get to the first
step check out branch `Step1`

    git checkout Step1

and so on. *Branches have not been created yet.*

Step 0: Creating an Rcpp package
================================

If Rcpp is not yet installed, install it. Now you can create a package
skeleton that is ready to use Rcpp with the following command:

    #install.packages('Rcpp')
    Rcpp::Rcpp.package.skeleton(
      name = 'RcppModulesExample',
      # Note that Rcpp itself comes with examples as well which we don't use.
      module = FALSE,
      example_code = FALSE,
      author = "Bob Jansen",
      email = "bobjansen@hey.com"
    )

Step 1: Exposing a function
===========================

The simplest way to run C++ code from R is by using functions. Suppose
we want to expose the following function (the source can be found in
`src/step1.cpp`):

    #include <Rcpp.h>
    using namespace Rcpp;

    void echo(std::string message) {
      Rcpp::Rcout << message << " from C++" << std::endl;
    }

We need to define a module in C++ and then load the module into R. To
create the module use the aptly named `RCPP_MODULE` macro

    // An RCPP_MODULE name step1_module
    RCPP_MODULE(step1_module) {
      // This module has a function called function which is the echo function
      // defined above.
      function("echo", &echo)
    }

Note that if your goal is to just expose a number of C++-functions to R,
it will probably be easier to use Rcpp attributes.

This code can now be loaded using `Rcpp::loadModule('step1', TRUE)`.

    .onLoad <- function(pkgname, libname) {
        Rcpp::loadModule('step1_module', TRUE)
    }

If you load this pacakge (using installation and `library()` or through
`devtools::load_all()`) you can use the Echo function:

    echo("Hello World")
    #> Hello World from C++

