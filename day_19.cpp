//
// Created by yalavrinenko on 19.12.2019.
//
#include "intcomputer.hpp"
#include <iostream>
#include <array>
#include <fstream>
#include <map>
#include <stack>
#include <queue>

int main(int argc, char** argv) {
  std::ifstream in("../in.map");
  std::string code;
  in >> code;

//  auto i = 0, j = 0, v = 0;
//  robot.mem().input_cb_ = [&i, &j, &v] () {
//    auto ret = 0;
//    if (v % 2 == 0){
//      ret = i;
//      i = (i + 1) % 50;
//    } else {
//      ret = j;
//      j = (j + 1) % 50;
//    }
//    v++;
//    return ret;
//  };
//
//  auto out_v = 0;
//  robot.mem().output_cb_ = [&out_v](auto v){
//    std::cout << ((v == 1) ? '#' : '.');
//    ++out_v;
//    if (out_v % 50 == 0)
//      std::cout << std::endl;
//  };

//  std::queue<std::pair<int, int>> q;

//auto count = 0;
//
//  for (auto i = 0; i < 50; ++i, std::cout << std::endl)
//    for (auto j = 0; j < 50; ++j){
//      IntComputer robot(code);
//      robot.mem() << j << i;
//      robot.run();
//      long c = 0;
//      robot.mem() >> c;
//      std::cout << ((c==1) ? '#' : '.');
//      count += c;
//    }
//
//  std::cout << count << std::endl;
//
//return 0;

  auto get_limits = [&code](auto line){
    auto begin = 0, end = 0;
    for (auto j = 0; j < 3000; ++j) {
      IntComputer robot(code);
      robot.mem() << j << line;
      robot.run();
      long c = 0;
      robot.mem() >> c;
      if (c == 1 && begin == 0){
        begin = j;
      }
      if (c == 1)
        end = j + 1;
    }
    return std::make_pair(begin, end);
  };

  auto l = 800, r = 830;

  auto shift = 100;

  while (l < r){
    auto c = l;//(l + r) / 2;

    auto [heal_l, head_r] = get_limits(c);
    auto [tail_l, tail_r] = get_limits(c + shift - 1);

    auto diff = head_r - tail_l;

    std::cout << c <<  " " << c + shift << " " << diff << std::endl;

    if (diff == shift){
      std::cout << heal_l << " " << head_r << std::endl;
      std::cout << tail_l << " " << tail_r << std::endl;
      std::cout << "X=" << head_r - shift << " Y=" << c << " ANS= " << (head_r - shift)*10000 + c << std::endl;
      break;
    }

    ++l;
    continue;
    if (diff < shift)
      l = c;
    else
      r = c;
  }

//  auto sum = 0;
//  for (auto i  = 0; i < 50; ++i){
//    auto [tail_l, tail_r] = get_limits(i);
//    if (tail_l != -1)
//      sum += tail_r - tail_l;
////    auto diff = head_r - tail_l;
////
////    std::cout << i <<  " " << diff << std::endl;
//  }
//
//  std::cout << sum << std::endl;

  return 0;
}