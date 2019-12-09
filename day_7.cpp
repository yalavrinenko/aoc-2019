//
// Created by yalavrinenko on 07.12.2019.
//

#include "intcomputer.hpp"
#include <iostream>
#include <fstream>
#include <iterator>
#include <numeric>

using namespace std::string_literals ;

int main(int argc,char ** argv){
  std::ifstream in("../in.map");
  std::string code;
  in >> code;

  std::array<int, 5> phases{5, 6, 7, 8, 9};

  auto max_signal = -1;

  do {
    std::array<IntComputer, 5> ampl{IntComputer{code}, IntComputer{code},
                                    IntComputer{code}, IntComputer{code},
                                    IntComputer{code}};
    std::array<long, 5> have_out{-1, -1, -1, -1, -1};
    for (auto i = 0; i < 5; ++i){
      ampl[i].mem().output_cb_ = [&have_out, i](auto v){ have_out[i] = v; };
      ampl[i].mem().input(phases[i]);
    }

    auto input = 0;

    bool is_stop = false;
    auto ampl_index = 0;
    while (!is_stop){
      ampl[ampl_index].mem().input(input);
      auto ret_code = 0;
      do{
        ret_code = ampl[ampl_index].next_instruction();
      } while (ret_code != -1 && have_out[ampl_index] == -1);

      if (ret_code == -1)
        is_stop = true;
      else {
        input = have_out[ampl_index];
        have_out[ampl_index] = -1;
        ampl_index = (ampl_index + 1) % 5;
      }
    }

    auto s = std::accumulate(phases.begin(), phases.end(),""s, [](auto s, auto v){
      return s + std::to_string(v);
    });
    if (max_signal < input){
      max_signal = input;
    }
  }while (std::next_permutation(phases.begin(), phases.end()));

  std::cout << max_signal << std::endl;

  return 0;
}
