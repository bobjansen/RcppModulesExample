#include <string>
#include <Rcpp.h>
using namespace Rcpp;

int addNumbers(int number1, int number2) {
  return number1 + number2;
}

class NumberMultiplier {
public:
  NumberMultiplier(int number) : number(number) {}
  int add(int number2) { return number + number2; };
  int multiply(int count) { return number * count; };

private:
  int number;
};

std::string combineStrings(std::string string1, std::string string2) {
  return string1 + string2;
}

class StringMultiplier {
public:
  StringMultiplier(std::string str) : str(str) {}
  std::string add(std::string string2) { return combineStrings(str, string2); };
  std::string multiply(int count) {
    std::string output = "";
    for (int i = 0; i < count; i++) {
      output = combineStrings(output, str);
    }
    return output;
  };

private:
  std::string str;
};

