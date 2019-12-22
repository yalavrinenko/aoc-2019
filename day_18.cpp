//
// Created by yalavrinenko on 18.12.2019.
//
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <vector>

using Map = std::vector<std::string>;
using NeighKey = std::vector<std::tuple<int, int, int>>;
size_t min_l = -1;

NeighKey find_neight_keys(Map const &map, std::set<char> const &keys, int x,
                          int y) {
  NeighKey v;
  std::queue<std::tuple<int, int, int>> q;
  q.push({x, y, 0});

  std::set<std::pair<int, int>> visited;

  auto check = [&](auto i, auto j) {
    return !(map[i][j] == '#' || (std::isupper(map[i][j]) &&
                                  keys.count(std::tolower(map[i][j])) == 0));
  };

  while (!q.empty()) {
    auto [cx, cy, l] = q.front();
    visited.insert({cx, cy});
    q.pop();
    if (std::islower(map[cx][cy]) && keys.count(map[cx][cy]) == 0) {
      v.emplace_back(cx, cy, l);
      continue;
    }

    for (auto d : {-1, 1}) {
      if (check(cx + d, cy) && !visited.count({cx + d, cy}))
        q.push({cx + d, cy, l + 1});

      if (check(cx, cy + d) && !visited.count({cx, cy + d}))
        q.push({cx, cy + d, l + 1});
    }
  }

  return v;
}

void path(Map const &map, std::set<char> &keys, int rx, int ry, int length) {
  if (length >= min_l)
    return;

  auto neight = find_neight_keys(map, keys, rx, ry);
  if (neight.empty()) {
    min_l = (min_l < length) ? min_l : length;
    std::cout << length << std::endl;
    return;
  }

  for (auto &[x, y, l] : neight) {
    keys.insert(map[x][y]);
    length += l;

    path(map, keys, x, y, length);

    length -= l;
    keys.erase(map[x][y]);
  }
}

void path(Map const &map, int rx, int ry) {
  struct bfs_node{
    std::set<char> keys;
    int x, y;
    int length;

    bool operator < (bfs_node const &v) const {
      return length > v.length;
    }
  };

  std::priority_queue<bfs_node> q;
  q.push({{}, rx, ry, 0});
  while (!q.empty()){
    auto node = q.top(); q.pop();

    auto neight = find_neight_keys(map, node.keys, node.x, node.y);
    if (neight.empty()){
      std::cout << node.length << std::endl;
      return;
    }

    for (auto &[cx, cy, l] : neight){
      auto k = node.keys; k.insert(map[cx][cy]);
      q.push({k, cx, cy, node.length + l});
    }
  }
}

using AdjList = std::map<char, std::vector<std::pair<char, int>>>;

AdjList construct_graph(Map const &map, int rx, int ry) {
  AdjList list;
  struct bfs_node{
    char c;
    std::set<char> keys;
    int x, y;
    int length;
  };

  std::queue<bfs_node> q;
  q.push({'@', {}, rx, ry, 0});

  std::map<char, bool> queued;
  while (!q.empty()){
    auto node = q.front(); q.pop();

    auto neight = find_neight_keys(map, node.keys, node.x, node.y);

    for (auto &[cx, cy, l] : neight){
      //if (!queued[map[cx][cy]]) {
        auto k = node.keys;
        k.insert(map[cx][cy]);
        queued[map[cx][cy]] = true;
        q.push({map[cx][cy], k, cx, cy, node.length + l});
      //}
      list[node.c].push_back({map[cx][cy], l});
    }
  }

  return list;
}

int main(int argc, char **argv) {
  std::ifstream in("../in.map");
  Map map;
  std::copy(std::istream_iterator<std::string>{in},
            std::istream_iterator<std::string>{}, std::back_inserter(map));

  auto rx = 0, ry = 0;
  std::set<char> sets;
  for (auto i = 0; i < map.size(); ++i)
    for (auto j = 0; j < map[i].size(); ++j) {
      if (map[i][j] == '@') {
        rx = i, ry = j;
      }
      if (std::isalpha(map[i][j]) && std::islower(map[i][j]))
        sets.insert(map[i][j]);
    }

  auto const KEY_COUNT = sets.size();
  //std::set<char> keys;
  //path(map, keys, rx, ry, 0);
  //path(map, rx, ry);
  //std::cout << min_l << std::endl;
  auto list = construct_graph(map, rx, ry);

  for (auto &[k, v] : list){
    std::cout << k << ": ";
    for (auto &[c, l] : v){
      std::cout << "(" << c << "," << l << ") ";
    }
    std::cout << std::endl;
  }
  return 0;
}