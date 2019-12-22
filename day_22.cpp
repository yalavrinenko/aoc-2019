//
// Created by yalavrinenko on 22.12.2019.
//
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <tuple>
#include <algorithm>
#include <numeric>

enum class ActionTypes{
  new_stack,
  cut,
  increment
};

struct action{
  enum ActionTypes action;
  int value;
};

std::vector<action> read_actions(std::ifstream &in){
  std::vector<action> act;
  while (!in.eof()){
    std::string c1;
    in >> c1;
    if (c1 == "cut"){
      int slice; in >> slice;
      act.emplace_back(action{ActionTypes ::cut, slice});
    } else {
      std::string c2; in >> c2;
      if (c2 == "into")
        act.emplace_back(action{ActionTypes ::new_stack});
      else {
        int shift; in >> c2; in >> shift;
        act.emplace_back(action{ActionTypes ::increment, shift});
      }
    }

    std::getline(in, c1);
  }
  return act;
}

void apply_action_list(std::vector<action> const &list, std::vector<int> &stack){
  auto tmp = stack;
  for (auto &act : list){
    if (act.action == ActionTypes::new_stack)
      std::reverse(stack.begin(), stack.end());
    if (act.action == ActionTypes::cut){
      if (act.value > 0){
        std::copy(stack.begin() + act.value, stack.end(), tmp.begin());
        std::copy(stack.begin(), stack.begin() + act.value, tmp.begin() + tmp.size() - act.value);
      } else {
        auto N = std::abs(act.value);
        std::copy(stack.begin(), stack.begin() + stack.size() - N, tmp.begin() + N);
        std::copy(stack.end() - N, stack.end(), tmp.begin());
      }
      stack = tmp;
    }
    if (act.action == ActionTypes::increment){
      int j = 0;
      int i = 0;
      while (j < stack.size()){
        tmp[i] = stack[j];
        i = (i + act.value) % stack.size();
        ++j;
      }
      stack = tmp;
    }

//    for (auto &v: stack){
//      std::cout << v << " ";
//    }
//    std::cout << std::endl;
  }
}

int main(int argc, char ** argv){
  std::ifstream in("../in.map");
  auto act = read_actions(in);

  auto N = 10007;
  std::vector<int> stack(N);
  std::iota(stack.begin(), stack.end(), 0);

  apply_action_list(act, stack);

  std::cout << std::distance(std::find(stack.begin(), stack.end(), 2019), stack.begin()) << std::endl;
  return 0;
}