//
// Created by yalavrinenko on 06.12.2019.
//
#include <iostream>
#include <fstream>
#include <iterator>
#include <map>
#include <algorithm>
#include <queue>
#include <numeric>
#include <set>

using namespace std::string_literals;

int depth(std::map<std::string, std::string> const &soi, std::string const &body){
  if (soi.count(body)){
    return 1 + depth(soi, soi.at(body));
  } else
    return 0;
}

int path(std::map<std::string, std::vector<std::string>> const &soi, std::string const &from, std::string const &to){
  std::queue<std::pair<std::string, int>> next;
  std::set<std::string> visited;
  next.push({from, 0});
  visited.insert(from);
  while (next.front().first != to){
    auto location = next.front(); next.pop();
    for (auto const &object : soi.at(location.first)){
      if (!visited.count(object)) {
        next.push({object, location.second + 1});
        visited.insert(object);
      }
    }
  }

  return next.front().second;
}

int main(int argc, char** argv){
  std::ifstream in("../in.map");
  std::map<std::string, std::vector<std::string>> soi;
  auto insert = [&soi](std::string const &s){
    auto cel_body = s.substr(0, s.find(')'));
    auto body = s.substr(cel_body.size() + 1);
    soi[body].emplace_back(cel_body);
    soi[cel_body].emplace_back(body);
  };
  std::for_each(std::istream_iterator<std::string>{in}, std::istream_iterator<std::string>{}, insert);

//  auto accum = [&soi](int sum, auto const &kv){
//    auto const& [k, v] = kv;
//    return sum + depth(soi, k);
//  };
//  auto count = std::accumulate(soi.begin(), soi.end(), 0, accum);

  std::cout << path(soi, "YOU"s, "SAN"s) - 2;
  return 0;
}