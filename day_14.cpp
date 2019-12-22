//
// Created by yalavrinenko on 14.12.2019.
//
#include <fstream>
#include <vector>
#include <iostream>
#include <array>
#include <tuple>
#include <map>
#include <algorithm>
#include <sstream>
#include <queue>
#include <cmath>

struct IngrItems{
  std::string name;
  long count{};

  IngrItems() = default;
  IngrItems(std::string name, long count): name{std::move(name)}, count(count){}

  bool operator< (IngrItems const &v) const {
    return name < v.name;
  }
};

using ChemList = std::map<IngrItems, std::vector<IngrItems>>;

std::vector<IngrItems> read_components(std::string const &s){
  std::istringstream iss{s};
  std::vector<IngrItems> v;
  std::string component; long count{};
  while (iss >> count >> component){
    v.emplace_back(component, count);
    iss.ignore(1);
  }
  return v;
}

ChemList read_recepts(){
  std::ifstream in("../in.map");
  ChemList chems;
  while (!in.eof()){
    std::string entry; std::getline(in, entry);
    entry.erase(std::remove(entry.begin(), entry.end(), ','), entry.end());
    auto equal_pos = entry.find("=>");
    auto sstr = entry.substr(equal_pos+2, entry.size() - equal_pos - 2);
    std::istringstream s{sstr};
    std::string component; long count{};
    s >> count >> component;
    chems[{component, count}] = read_components(entry.substr(0, equal_pos));

    std::sort(chems[{component, count}].begin(), chems[{component, count}].end(), [](auto &a, auto &b){ return a.count < b.count; });
  }

  return chems;
}

size_t ore_count(IngrItems const &ritem, ChemList const &recepts, std::map<std::string, long> &overflow){
  if (ritem.name == "ORE")
    return ritem.count;

  auto entry = recepts.find(ritem);
  long rexec = 0;
  auto prod = 0;
  while (prod < ritem.count){
    prod += entry->first.count;
    ++rexec;
  }
  auto ocount = 0;
  for (auto const &c : entry->second){
    auto required = c.count * rexec;
    while (overflow[c.name] >= c.count){
      required -= c.count;
      overflow[c.name] -= c.count;
    }
    ocount += ore_count({c.name, required}, recepts, overflow);
  }

  auto produced = prod;
  overflow[ritem.name] += produced - ritem.count;

  std::cout << ritem.name << " " << ritem.count << " " << produced << " " << ocount << std::endl;
  return ocount;
}

void required_map(IngrItems const &ritem, ChemList const &recepts, std::map<std::string, long> &overflow){
  if (ritem.name == "ORE")
    return;
  auto entry = recepts.find(ritem);
  for (auto const &c : entry->second){


  }
}

int main(int argc, char** argv){
  auto recepts = read_recepts();

  std::map<std::string, long> of;
  std::cout << ore_count({"FUEL", 1}, recepts, of);

  std::cout << std::endl;
  for (auto &[k, v] : of){
    std::cout << k << " " << v << std::endl;
  }

  return 0;
}