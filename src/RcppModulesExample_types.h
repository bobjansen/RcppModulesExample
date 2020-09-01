#include "step1.h"

RCPP_MODULE(step1) {
  function("echo", &echo);

  class_<A>("A")
    .field("A", &A::a);
}
 
