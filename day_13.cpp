//
// Created by yalavrinenko on 13.12.2019.
//
#include "intcomputer.hpp"
#include <iostream>
#include <array>
#include <fstream>
#include <thread>
#include <chrono>

using namespace std::string_literals;

struct ball{
  int x = 0, y = 0;
  int vx = 0, vy = 0;

  bool is_first = true;

  void set_position(int _x, int _y){
    if (!is_first){
      vx = _x - x;
      vy = _y - y;
    }
    is_first = false;
    x = _x; y = _y;
  }
};

struct pad{
  int x, y;
};

int main(int argc, char** argv) {
  std::ifstream in("../in.map");
  std::string s;
  in >> s;
  IntComputer robot(s);

  bool is_halted = false;

  auto count = 0;
  robot.mem()[0] = 2;

  std::array<std::array<int, 41>, 25> a{};
  bool have_map = false;
  auto score = 0;

  ball ball;
  pad pad{};

  auto collision_point = [&](){
    auto dh = pad.y - ball.y;
    auto t = (ball.vy == 0) ? 0 : dh / ball.vy;
    return ball.x;// + t * ball.vx;
  };

  robot.mem().input_cb_ = [&](){
    auto cx = collision_point();
    if (pad.x < cx)
      return 1;
    else if (pad.x > cx)
      return -1;
    else
      return 0;
  };

  do{
    while (robot.mem().output_count() < 3){
      is_halted = robot.next_instruction() == -1;
      if (is_halted)
        break;
    }

    if (!is_halted){
      auto x = robot.mem().output();
      auto y = robot.mem().output();
      auto id = robot.mem().output();

      if (x != -1) {
        a[y][x] = id;
        if (id == 4) {
          ball.set_position(x, y);
        }

        if (id == 3){
          pad.x = x;
          pad.y = y;
        }
      }

      if (x == -1 || have_map){
        std::system("clear");
        std::cout << score << std::endl;
        have_map = true;
        for (auto &al : a){
          for (auto &v : al){
            char c;
            switch (v){
            case 0: c = ' ';
              break;
            case 1: c = '|';
              break;
            case 2: c = '#';
              break;
            case 3: c = '_';
              break;
            case 4: c = 'O';
              break;
            }
            std::cout << c;
          }
          std::cout << "\n";
        }
        std::cout.flush();
        std::this_thread::sleep_for(std::chrono::duration<double>(0.01));

        if (x == -1)
          score = id;
      }
    }
  }while (!is_halted);

  std::cout << score << std::endl;

  return 0;
}