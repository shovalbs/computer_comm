#include "test_lib.h"

test_class::test_class(std::string name){
  this->name = name;
}

std::string test_class::get_name(){
  return this->name;
}