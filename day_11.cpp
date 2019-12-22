//
// Created by yalavrinenko on 11.12.2019.
//
#include "intcomputer.hpp"
#include <iostream>
#include <fstream>
#include <iterator>
#include <numeric>

int main(int argc, char** argv){
  std::ifstream in("../in.map");
  std::string s; in >> s;
  IntComputer robot(s);

  std::map<std::pair<int, int>, int> map;

  auto i = 0, j = 0;
  map[{i, j}] = 1;

  std::pair<int, int> dir{0, 1};
  bool is_halted = false;

  auto left = [](auto &dir, int &i, int &j){
    auto dx = -dir.second;
    auto dy = dir.first;

    dir.first = dx; dir.second = dy;
    i += dx; j += dy;
  };

  auto right = [](auto &dir, int &i, int &j){
    auto dx = dir.second;
    auto dy = -dir.first;

    dir.first = dx; dir.second = dy;
    i += dx; j += dy;
  };

  do{
    robot.mem().input(map[{i, j}]);

    while (robot.mem().output_count() < 2){
      is_halted = robot.next_instruction() == -1;
      if (is_halted)
        break;
    }

    if (!is_halted){
      auto color = robot.mem().output();
      auto angle = robot.mem().output();

      map[{i, j}] = color;

      if (angle == 0){
        left(dir, i, j);
      } else {
        right(dir, i, j);
      }
    }
  }while (!is_halted);

  for (auto &[k, v] : map){
    if (v == 1)
      std::cout << k.first << " " << k.second << std::endl;
  }



  return 0;
}