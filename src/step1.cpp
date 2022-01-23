#include <Rcpp.h>
using namespace Rcpp;

void echo(std::string message) {
  Rcout << message << " from C++" << std::endl;
}
