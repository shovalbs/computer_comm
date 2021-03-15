#pragma once
#include <iostream>
#include <string>

class test_class{
  private:
   std::string name;
  public:
   test_class(std::string name);
   std::string get_name();
};