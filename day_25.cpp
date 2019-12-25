//
// Created by yalavrinenko on 25.12.2019.
//
#include "intcomputer.hpp"
#include <array>
#include <fstream>
#include <future>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <stack>
#include <thread>

using namespace std::string_literals;

struct permutation{
  std::string items[7] = {"food ration"s,
                         "weather machine"s,
                         "antenna"s,
                         "jam"s,
                         "semiconductor"s,
                         "planetoid"s,
                         "monolith"s};

  void make_combination(){
    std::set<std::string> used{items[0]};
    make_combination(used);
  }

  int count() const {
    return combnations_.size();
  }

  std::set<std::string> const& next() const {
    return combnations_[next_++];
  }

private:

  void make_combination(std::set<std::string> &used){
    this->combnations_.emplace_back(used);

    for (auto i = 0; i < 7; ++i){
      if (used.count(items[i]) == 0){
        used.insert(items[i]);
        make_combination(used);
        used.erase(items[i]);
      }
    }
  }

  std::vector<std::set<std::string>> combnations_;
  std::stack<std::string> selected_;
  mutable int next_ = 0;
};

int main(int argc, char** argv){
  std::ifstream in("../in.map");
  std::string code; in >>  code;
  IntComputer pc(code);

  std::ifstream exist_command("../cmake-build-release/track");
  std::queue<std::string> comm;
  while (!exist_command.eof()){
    std::string s;
    std::getline(exist_command, s);
    comm.push(s);
  }

  permutation p;
  p.make_combination();

  pc.mem().input_cb_ = [&pc, &comm, &p](){
    std::string command;
    if (comm.empty()) {
      //std::getline(std::cin, command);
      command = "1";
      if (command == "1"){
        auto next = p.next();
        for (auto &s : next){
          comm.push("take " + s);
        }

        comm.push("east");

        for (auto &s : next){
          comm.push("drop " + s);
        }

        command = comm.front(); comm.pop();
      }
    }
    else {
      command = comm.front(); comm.pop();
    }
    for (auto i = 1; i <command.size(); ++i)
      pc.mem() << command[i];
    pc.mem() << '\n';

    return command[0];
  };
  pc.mem().output_cb_ = [](auto v) { std::cout << (char)v; };
  pc.run();

  return 0;
}