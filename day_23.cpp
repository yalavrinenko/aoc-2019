//
// Created by yalavrinenko on 23.12.2019.
//
#include "intcomputer.hpp"
#include <array>
#include <fstream>
#include <future>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <stack>
#include <thread>

struct message{
  long dir;
  long x, y;
};

int main(int argc, char** argv) {
  std::ifstream in("../in.map");
  std::string code;
  in >> code;
  constexpr size_t N = 50;
  std::vector<IntComputer> net(N, IntComputer(code));
  std::array<std::queue<message>, N> message_queue;

  std::vector<std::future<void>> run_q;

  message NAT_Message{-1, 0, 0};
  std::mutex nat_mutex;

  std::array<bool, N> idle;
  idle.fill(false);

  auto last_derived = -1;

  auto is_idle = [&idle](){ return std::accumulate(idle.begin(), idle.end(), 0) == 50; };

  auto callback_factory = [is_idle, &last_derived, &net, &message_queue, &nat_mutex, &NAT_Message, &idle](size_t i){
    return [is_idle, &last_derived, &net, &message_queue, i, &nat_mutex, &NAT_Message, &idle](){

      net[i].mem().input_cb_ = [is_idle, &last_derived, &message_queue, &net, i, &idle, &NAT_Message, &nat_mutex]() {
        std::lock_guard<std::mutex> lock(nat_mutex);
        if (message_queue[i].empty()) {
          if (is_idle() && i == 0 && NAT_Message.dir != -1) {
            NAT_Message.dir = -1;
            idle.fill(false);
            net[i].mem() << NAT_Message.y;
            if (last_derived == NAT_Message.y) {
              std::cout << last_derived << std::endl;
              std::terminate();
            }
            last_derived = NAT_Message.y;
            return NAT_Message.x;
          }
          idle[i] = true;
          return -1l;
        }
        else {
          idle[i] = false;
          auto v = message_queue[i].front();
          message_queue[i].pop();
          net[i].mem() << v.y;
          return v.x;
        }
      };

      net[i].mem().output_cb_ = [&message_queue, &net, i, &nat_mutex, &NAT_Message](auto v){
        if (net[i].mem().output_count() == 3){
          long dir, x, y;
          net[i].mem() >> dir >> x >> y;
          if (dir == 255) {
            std::lock_guard<std::mutex> lock(nat_mutex);
            std::cout << "NAT: " << i << " " << x << " " << y << std::endl;
            NAT_Message = message{static_cast<long>(i), x, y};
          } else
            message_queue[dir].push({dir, x, y});
        }
      };

      net[i].mem() << i;
      net[i].run();
    };
  };

  run_q.reserve(N);
  for (auto i = 0; i < N; ++i){
    run_q.emplace_back(std::async(std::launch::async, callback_factory(i)));
  }

  for (auto i = 0; i < N; ++i)
    run_q[i].get();
  return 0;
}