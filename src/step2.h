#include <string>
#include <Rcpp.h>
using namespace Rcpp;

class Echo {
public:
  Echo(std::string message) : message_(message) {}
  std::string get() { return message_; };

private:
  std::string message_;
};
