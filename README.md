Introduction
============

The aim of this vignette is to describe all the aspects of Rcpp Modules,
from the very basic to the more advanced. If you’re not familiar with
Rcpp please refer to Allaire, Eddelbuettel and Francois (2017). Some C++
knowledge is assumed but only at a basic level. I’m not a C++ expert
either (I did get expert advice though). Stubbornness helps.
[StackOverFlow](https://stackoverflow.com/questions/tagged/rcpp) too.
The code does not include any functionality except for concatenating,
printing and multiplying of strings by design. The only goal of the
package is to give minimal examples of all Rcpp Modules features. The
package around this vignette is fully functional and should be loadable
using the tooling you’re used to such as building and installing the
package or using
[`devtools`](https://cran.r-project.org/web/packages/devtools/) or
[`pkgload`](https://cran.r-project.org/web/packages/pkgload/).

The following sections contain a step by step guide of creating the
package, starting with the most basic features and building up from
there.

All steps can be referred to by their own branch, to get to the first
step check out branch `Step1`

``` bash
git checkout Step1
```

and so on. If you want to start a project but don’t need all the
functionality, you can just copy the source code contained in that
branch and get started.

Step 0: Creating an Rcpp package
================================

If Rcpp is not yet installed, install it. Now you can create a package
skeleton that is ready to go with Rcpp using the following command:

``` r
#install.packages('Rcpp')
Rcpp::Rcpp.package.skeleton(
  name = 'RcppModulesExample',
  # Note that Rcpp itself comes with examples as well which we don't use.
  module = FALSE,
  example_code = FALSE,
  author = "Bob Jansen",
  email = "bobjansen@hey.com"
)
```

Step 1: Exposing a function
===========================

The simplest way to run C++ code from R is by using functions. Suppose
we want to expose the following function (the source can be found in
`src/step1.cpp`):

``` cpp
#include <Rcpp.h>
using namespace Rcpp;

void echo(std::string message) {
  Rcpp::Rcout << message << " from C++" << std::endl;
}
```

We need to define a module in C++ and then load the module into R. To
create the module use the aptly named `RCPP_MODULE` macro

``` cpp
// An RCPP_MODULE name step1_module
RCPP_MODULE(step1_module) {
  // This module has a function called function which is the echo function
  // defined above.
  function("echo", &echo)
}
```

Note that if your goal is to just expose a number of C++-functions to R,
it will probably be easier to use Rcpp attributes.

This code can now be loaded using `Rcpp::loadModule('step1', TRUE)`.

``` r
.onLoad <- function(pkgname, libname) {
    Rcpp::loadModule('step1_module', TRUE)
}
```

If you load this pacakge (using installation and `library()` or through
`devtools::load_all()`) you can use the `echo` function:

``` r
echo("Hello World")
#> Hello World from C++
```

Step 2: Exposing a class
========================

Alternatively, we can create an `Echo` class that does the echo’ing in a
more stateful manner:

``` cpp
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
```

This class can be constructed with a message and this message can be
retrieved with the `get()` method or printed using the `print()` method.

It can be exposed to R with another module (or the same module if you’re
so inclined):

``` cpp
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
```

Now make sure to load the new module using `loadModule()`:

``` r
.onLoad <- function(pkgname, libname) {
  Rcpp::loadModule('step1_module', TRUE)
  Rcpp::loadModule('step2_module', TRUE)
}
```

and use the Echo object:

``` r
echoObject <- new(Echo, 'Hello World')
echoObject$print()
#> Hello World from C++
echoObject$get()
#> [1] "Hello World"
```

It’s that simple.

Step 3: Modules with multiple parts
-----------------------------------

Combining multiple functions and classes into one module is as easy as
you might hope. For example, take these new C++ functions and classes
which can be found in `src/step3.cpp` as well. This code defines to add
or multiple ints or strings.

``` cpp
#include "step3.h"

int addNumbers(int number1, int number2) {
  return number1 + number2;
}

std::string combineStrings(std::string string1, std::string string2) {
  return string1 + string2;
}

std::string StringMultiplier::add(std::string str2) {
  return combineStrings(str, str2);
}

std::string StringMultiplier::multiply(int count) {
  std::string output = "";
  for (int i = 0; i < count; i++) {
    output = combineStrings(output, str);
  }
  return output;
}

int NumberMultiplier::add(int number2) {
  return number + number2;
}

int NumberMultiplier::multiply(int count) {
  return number * count;
}
```

The `RCPP_MODULE` to use these is defined below. It has entries for the
functions that combine to ints or strings and the objects that use
these:

``` r
RCPP_MODULE(step3_module) {
  function("combineStrings", &combineStrings);
  function("addNumbers", &addNumbers);

  class_<StringMultiplier>("StringMultiplier")
    .constructor<std::string>()
    .method("add", &StringMultiplier::add)
    .method("multiply", &StringMultiplier::multiply)
  ;

  class_<NumberMultiplier>("NumberMultiplier")
    .constructor<int>()
    .method("add", &NumberMultiplier::add)
    .method("multiply", &NumberMultiplier::multiply)
  ;
}
```

To activate the module, update the `.onLoad()` function and reload the
package:

``` r
.onLoad <- function(pkgname, libname) {
  Rcpp::loadModule('step1_module', TRUE)
  Rcpp::loadModule('step2_module', TRUE)
  Rcpp::loadModule('step3_module', TRUE)
}
```

and use it like before:

``` r
addNumbers(2L, 3)
#> [1] 5
numberMultiplier <- new(NumberMultiplier, 2L)
numberMultiplier$add(5)
#> [1] 7
numberMultiplier$multiply(5L)
#> [1] 10

combineStrings('hello world', '!')
#> [1] "hello world!"
stringMultiplier <- new(StringMultiplier, 'hello ')
stringMultiplier$add('world!')
#> [1] "hello world!"
combineStrings(stringMultiplier$multiply(3), 'how low')
#> [1] "hello hello hello how low"
```
