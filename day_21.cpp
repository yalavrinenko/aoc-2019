//
// Created by yalavrinenko on 21.12.2019.
//
#include "intcomputer.hpp"
#include <iostream>
#include <array>
#include <fstream>
#include <map>
#include <stack>
#include <queue>


int main(int argc, char** argv){
  std::ifstream in("../in.map");
  std::string code; in >>  code;
  IntComputer pc(code);


  std::string ascii_code = R"(NOT A J
NOT C T
AND H T
OR T J
NOT B T
AND A T
AND C T
OR T J
AND D J
RUN
)";

  for (auto &c: ascii_code)
    pc.mem() << c;
  pc.mem().output_cb_ = [](auto v) { std::cout << v << std::endl; };
  pc.run();

  return 0;
}