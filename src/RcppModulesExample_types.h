#include "step1.h"
#include "step2.h"
#include "step3.h"

RCPP_MODULE(step1_module) {
  function("echo", &echo);
}

RCPP_MODULE(step2_module) {
  class_<Echo>("Echo")
  .constructor<std::string>()
  .method("get", &Echo::get)
  .method("print", &Echo::print)
  ;
};

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
