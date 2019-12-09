//
// Created by yalavrinenko on 09.12.2019.
//
#include "intcomputer.hpp"
#include <iostream>
#include <fstream>
#include <iterator>
#include <numeric>

int main(int argc, char** argv){
  std::ifstream in("../in.map");
  std::string code;
  in >> code;
  IntComputer c(code);
  c.mem().input(2);
  c.mem().output_cb_ = [](auto v) { std::cout << v << " ";};
  c.run();
  return 0;
}