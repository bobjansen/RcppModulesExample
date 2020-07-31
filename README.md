Introduction
============

The aim of this vignette is to describe all the aspects of Rcpp Modules,
from the very basic to the more advanced. If your not familiar with Rcpp
please refer to Allaire, Eddelbuettel and Francois (2017). Some `C++`
knowledge is assumed but only at a basic level. I’m not a `C++` expert
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

Step 1: Creating an Rcpp package
================================

If Rcpp is not yet installed, install it. Now you can create a package
skeleton that is ready to use Rcpp with the following command:

    #install.packages('Rcpp')
    Rcpp::Rcpp.package.skeleton(
      name = 'RcppModulesExample',
      module = FALSE, # Rcpp comes with some examples as well.
      example_code = FALSE,
      author = "Bob Jansen",
      email = "bobjansen@hey.com"
    )
