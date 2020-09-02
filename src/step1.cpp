#include <Rcpp.h>
using namespace Rcpp;

void echo(std::string message) {
  Rcpp::Rcout << message << " from C++" << std::endl;
}
