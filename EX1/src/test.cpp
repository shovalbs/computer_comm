#include <iostream>
#include "test_lib.h"


int main(int argc, char** argv){
  std::cout << "hello world" << std::endl;
  test_class test_1("test name");
  std::string got = test_1.get_name();
  std::cout << got << std::endl;
  return 0;
}