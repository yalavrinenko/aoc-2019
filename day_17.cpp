//
// Created by yalavrinenko on 17.12.2019.
//
#include "intcomputer.hpp"
#include <iostream>
#include <array>
#include <fstream>
#include <map>
#include <stack>
#include <queue>

using namespace std::string_literals;

bool is_cross(std::vector<std::string> const &map, int i, int j){
  if (i <= 0 || j <= 0 || i >= map.size()-1 || j >= map.front().size()-1)
    return false;

  int is_cross = 0;

  for (auto d : {-1, 1}){
    if (map[i+d][j] == '#')
      ++is_cross;
    if (map[i][j+d] == '#')
      ++is_cross;
  }

  return is_cross == 4 && map[i][j] == '#';
}

std::tuple<bool, std::pair<int, int>, char> is_left(std::vector<std::string> const &map, int i, int j){
  if (map[i][j] == '^') {
    if (map[i][j - 1] == '#'){
      return {true, {0, -1}, '<'};
    } else {
      return {false, {0, 1}, '>'};
    }
  }

  if (map[i][j] == 'v') {
    if (map[i][j + 1] == '#'){
      return {true, {0, 1}, '>'};
    } else {
      return {false, {0, -1}, '<'};
    }
  }

  if (map[i][j] == '<') {
    if (map[i + 1][j] == '#'){
      return {true, {1, 0}, 'v'};
    } else {
      return {false, {-1, 0}, '^'};
    }
  }

  if (map[i][j] == '>') {
    if (map[i - 1][j] == '#'){
      return {true, {-1, 0}, '^'};
    } else {
      return {false, {1, 0}, 'v'};
    }
  }
  return {false, {0, 0},'o'};
}

void move_step(std::vector<std::string> &map, int rx, int ry){
  auto [il, d, nd] = is_left(map, rx, ry);
  if (il)
    std::cout << "L,";
  else
    std::cout << "R,";

  auto count = 0;
  while (map[rx][ry] != '.' && rx < map.size() && ry < map[rx].size()){
    rx += d.first;
    ry += d.second;
    ++count;
  }

  std::cout << count - 1 << ",";
  if (count != 1){
    rx -= d.first;
    ry -= d.second;
    map[rx][ry] = nd;
    move_step(map, rx, ry);
  }
}

int main(int argc, char** argv){
  std::ifstream in("../in.map");
  std::string code; in >>  code;
  IntComputer robot(code);

  std::string in_prog = R"(A,B,A,A,B,C,B,C,C,B
L,12,R,8,L,6,R,8,L,6
R,8,L,12,L,12,R,8
L,6,R,6,L,12
n
)";

  for (auto &c : in_prog)
    robot.mem().input(c);

  char prev_char=' ';
  robot.mem().output_cb_ = [&prev_char](auto v) {
    std::cout << (char)v;
  };

  robot.run();

//  std::vector<std::string> map;
//  std::string line{};
//  while (robot.mem().output_count()){
//    char c = robot.mem().output();
//    if (c == '\n'){
//      map.emplace_back(line);
//      line.clear();
//    } else
//      line += c;
//  }

//  auto empty_s = std::string(map[0].size(), '.');
//  map.insert(map.begin(), empty_s);
//  map.back() = empty_s;
//
//  for (auto &s : map){
//    s = '.' + s + '.';
//  }
//
//  auto sum = 0;
//  int rx, ry;
//  for (auto i = 0; i < map.size(); ++i, std::cout << std::endl)
//    for (auto j = 0; j < map[i].size(); ++j){
//      std::cout << map[i][j];
//      if (is_cross(map, i, j)) {
//        sum += i * j;
//      }
//      if (map[i][j] == '^'){
//        rx = i; ry = j;
//      }
//    }

  //move_step(map, rx, ry);
  return 0;
}