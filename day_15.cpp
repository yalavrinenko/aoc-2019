//
// Created by yalavrinenko on 15.12.2019.
//
#include "intcomputer.hpp"
#include <iostream>
#include <array>
#include <fstream>
#include <map>
#include <stack>
#include <queue>

using namespace std::string_literals;

class Navigation{
public:
  Navigation() {
    map_[{0, 0}] = 1;
  }

  auto input_function() {
    return [this](){
      return next_movement() + 1;
    };
  }

  auto output_function() {
    return [this](long value) {
      object_ = value;
      std::cout << x_ << " " << y_ << " " << value << std::endl;
      map_[{x_, y_}] = value;
      if (object_ == 0) {
        adjust_backward_xy();
      } else {
        if (!no_log_)
          mhistory_.push(direction_);
        else
          no_log_ = false;
      }
    };
  }

  int dfs(std::pair<long, long> const &p) {
    std::queue<std::pair<long, std::pair<long, long>>> q;
    q.push({0, p});

    while (!q.empty()){
      auto current = q.front();
      if (map_[current.second] == 2) {
        std::cout << current.second.first << ";" << current.second.second << std::endl;
        return current.first;
      }

      q.pop();

      auto [cx, cy] = current.second;

      for (auto dx : {-1, 1}){
        if (map_[{cx + dx, cy}] > 0)
          q.push({current.first + 1, {cx + dx, cy}});
        if (map_[{cx, cy + dx}] > 0)
          q.push({current.first + 1, {cx, cy + dx}});
      }

      map_[{cx, cy}] = 0;
    }

    return -1;
  }

  int spread_oxy(std::pair<long, long> const &p) {
    std::queue<std::pair<long, std::pair<long, long>>> q;
    q.push({0, p});

    int max_depth = 0;

    while (!q.empty()){
      auto current = q.front();
      max_depth = current.first;
      q.pop();

      auto [cx, cy] = current.second;

      for (auto dx : {-1, 1}){
        if (map_[{cx + dx, cy}] > 0)
          q.push({current.first + 1, {cx + dx, cy}});
        if (map_[{cx, cy + dx}] > 0)
          q.push({current.first + 1, {cx, cy + dx}});
      }

      map_[{cx, cy}] = 0;
    }

    return max_depth;
  }


protected:
  long next_movement() {
    if (object_ != 1){
      direction_ = (direction_ + 1) % 4;
    } else
      direction_ = 0;

    adjust_forward_xy();
    auto count_dir = 0;
    while (map_.count({x_, y_}) && count_dir < 4){
      adjust_backward_xy();
      direction_ = (direction_ + 1) % 4;
      adjust_forward_xy();
      ++count_dir;
    }

    if (count_dir == 4){
      if (mhistory_.empty())
        return -1;
      auto prev_dir = mhistory_.top();
      mhistory_.pop();

      adjust_backward_xy();
      direction_ = reverse(prev_dir);
      adjust_forward_xy();
      no_log_ = true;
    }

    return direction_;
  }

  void adjust_forward_xy(){
    switch (direction_){
    case 0:
      y_ += 1;
      break;
    case 1:
      y_ -= 1;
      break;
    case 2:
      x_ -= 1;
      break;
    case 3:
      x_ += 1;
      break;
    }
  }

  void adjust_backward_xy(){
    switch (direction_){
    case 0:
      y_ -= 1;
      break;
    case 1:
      y_ += 1;
      break;
    case 2:
      x_ += 1;
      break;
    case 3:
      x_ -= 1;
      break;
    }
  }

  static int reverse(int dir){
    if (dir == 0) return 1;
    if (dir == 1) return 0;
    if (dir == 2)
      return  3;
    if (dir == 3)
      return 2;
    return -1;
  }

  long direction_ = 0;
  long x_{0}, y_{0};
  long object_={1};
  std::map<std::pair<long, long>, int> map_;
  std::stack<long> mhistory_;
  bool no_log_ = false;
};

int main(int argc, char** argv){
  std::ifstream in("../in.map");
  std::string code; in >>  code;
  IntComputer robot(code);

  Navigation nav;
  robot.mem().input_cb_ = nav.input_function();
  robot.mem().output_cb_ = nav.output_function();

  robot.run();

  //std::cout << nav.dfs({0, 0}) << std::endl;
  std::cout << nav.spread_oxy({-12, 16}) << std::endl;
  return 0;
}