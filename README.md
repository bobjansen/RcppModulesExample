Introduction
============

The aim of this vignette is to describe all the aspects of Rcpp Modules,
from the very basic to the more advanced. If you’re not familiar with
Rcpp please refer to Allaire, Eddelbuettel and Francois (2017). Some C++
knowledge is assumed but only at a basic level. I’m not a C++ expert
either (I did get expert advice though). Stubbornness helps. The code
does not include any functionality except for concatenating, printing
and multiplying of strings by design. The only goal of the package is to
give minimal examples of all Rcpp Modules features. The package around
this vignette is fully functional and should be loadable using the
tooling you’re used to such as building and installing the package or
using [`devtools`](https://cran.r-project.org/web/packages/devtools/) or
[`pkgload`](https://cran.r-project.org/web/packages/pkgload/).

The following sections contain a step by step guide of creating the
package, starting with the most basic features and building up from
there.

All steps can be referred to by their own branch, to get to the first
step check out branch `Step1`

    git checkout Step1

and so on.

Step 0: Creating an Rcpp package
================================

If Rcpp is not yet installed, install it. Now you can create a package
skeleton that is ready to go with Rcpp using the following command:

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

Step 2: Exposing a class
========================

Alternatively, we can create an `Echo` class that does the echo’ing in a
more stateful manner:

    #include <string>
    #include <Rcpp.h>
    using namespace Rcpp;

    class Echo {
    public:
      Echo(std::string message) : message(message) {}
      std::string get() { return message; };
      void print() { Rcout << message << " from C++" << std::endl; };

    private:
      std::string message;
    };

This class can be constructed with a message and this message can be
retrieved with the `get()` method or printed using the `print()` method.

It can be exposed to R with another module (or the same module if you’re
so inclined):

    RCPP_MODULE(step2_module) {
      // Instead of a function we expose class.
      class_<Echo>("Echo")
      // The class has a constructor which takes a string argument. The arguments
      // to the constructor are put in the brackets. The .constructor method
      // doesn't take any arguments.
      .constructor<std::string>()
      // .method works the same as function but don't forget to specify the class
      // in the second argument.
      .method("get", &Echo::get)
      .method("print", &Echo::print)
      ;
    };

Now make sure to load the new module using `loadModule()`:

    .onLoad <- function(pkgname, libname) {
      Rcpp::loadModule('step1_module', TRUE)
      Rcpp::loadModule('step2_module', TRUE)
    }

and use the Echo object:

    echoObject <- new(Echo, 'Hello World')
    echoObject$print()
    #> Hello World from C++
    echoObject$get()
    #> [1] "Hello World"

It’s that simple.
