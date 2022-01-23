#include <string>
#include <Rcpp.h>
using namespace Rcpp;

class Echo {
public:
  Echo(std::string message) : message(message) {}
  std::string get() { return message; }
  void print() { Rcout << message << " from C++" << std::endl; };

private:
  std::string message;
};
