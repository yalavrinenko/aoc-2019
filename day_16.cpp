//
// Created by yalavrinenko on 16.12.2019.
//
#include <array>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <numeric>

int phase_v(int repeat, int i){
  static const std::array<char, 4> v{0, 1, 0, -1};
  auto base = repeat * 4;
  return v[int((i % base) / repeat)];
}

std::vector<char> FFT(std::vector<char> const &v){
  std::vector<char> out(v.size());

  for (auto i = 0; i < v.size(); ++i){
    auto j = 0;
    out[i] = std::abs(std::accumulate(v.begin(), v.end(), 0, [i, &j](auto sum, auto v){
      return sum + v * phase_v(i + 1, ++j);
    })) % 10;
  }

  return out;
}

std::vector<char> FFTv2(std::vector<char> const &v, size_t offset){
  std::vector<char> out(v.size(), {0});

  auto size = v.size();
  out[v.size() - 1] = v[v.size() - 1];
  for (long i = v.size() - 2; i > v.size() / 2; --i){
    out[i] += out[i + 1] + v[i];
    out[i] = std::abs(out[i]) % 10;
  }

  return out;
}

std::vector<char> repeat(std::vector<char> const &v, int N){
  std::vector<char> out_v(v.size() * N);
  for (auto i = 0; i < N; ++i){
    std::copy(v.begin(), v.end(), out_v.begin() + i*v.size());
  }
  return out_v;
}

int main(int argc, char ** argv){
  std::ifstream in("../in.map");
  std::vector<char> input_signal;
  std::copy(std::istream_iterator<char>(in), std::istream_iterator<char>{}, std::back_inserter(input_signal));
  std::transform(input_signal.begin(), input_signal.end(), input_signal.begin(), [](auto v) { return v - '0'; });


  input_signal = repeat(input_signal, 10000);

  auto N = 100;
  auto offset = 5979633;
  if (offset < input_signal.size() / 2)
    throw "Out of range";

  while (N > 0){
    input_signal = FFTv2(input_signal, offset);
    --N;

    std:copy_n(input_signal.begin() + offset, 8, std::ostream_iterator<int>(std::cout, ""));
    std::cout << std::endl;
  }


  return 0;
}