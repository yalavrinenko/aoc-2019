//
// Created by yalavrinenko on 24.12.2019.
//
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <string>
#include <tuple>
#include <vector>

int main(int argc, char ** argv){
  std::ifstream in("../in.map");

  std::array<std::array<char, 5>, 5> cl[2];

  std::array<std::array<size_t, 5>, 5> hashes{};

  auto print = [&cl](auto k) {
    for (auto i = 0; i < 5; ++i, std::cout << std::endl)
      for (auto j = 0; j < 5; ++j)
        std::cout << cl[k][i][j];
  };

  for (auto i = 0; i < 5; ++i)
    for (auto j = 0; j < 5; ++j) {
      in >> cl[0][i][j];
      hashes[i][j] = std::pow(2, i * 5 + j);
    }

  auto neight = [&cl](auto x, auto y, auto k){
    auto count = 0;

    auto inc = [&](auto i, auto j){
      if (i < 0 || j < 0 || i >= 5 || j >= 5 || (i == x && j == y))
        return;;
      if (cl[k][i][j] == '#')
        ++count;
    };

    for (auto d : {-1, 1}){
      inc(x + d, y);
      inc (x , y +d);
    }
    return count;
  };

  auto evolve = [&cl, &neight](auto k){
    auto nid = (k + 1) % 2;
    cl[nid] = cl[k];
    for (auto i = 0; i < 5; ++i){
      for (auto j = 0; j < 5; ++j){
        auto n = neight(i, j, k);
        if (cl[k][i][j] == '#' && n != 1)
          cl[nid][i][j] = '.';
        if (cl[k][i][j] == '.' && (n == 1 || n == 2))
          cl[nid][i][j] = '#';
      }
    }
  };

  auto k = 0;
  auto n = 5;

  auto count_hash = [&cl, &hashes](auto k){
    size_t h = 0;
    for (auto i = 0; i < 5; ++i)
      for (auto j = 0; j < 5; ++j){
        if (cl[k][i][j] == '#')
          h += hashes[i][j];
      }
    return h;
  };

  std::map<size_t, int> hh;

  while (true){
    evolve(k);
    k = (k + 1) % 2;
    auto h = count_hash(k);
    if (hh[h] == 1){
      print(k);
      std::cout << h << std::endl;
      break;
    } else {
      hh[h] = 1;
    }
//    std::cout << std::endl;
//    print(k);
//    --n;
  }
  return 0;
}