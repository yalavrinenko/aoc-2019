//
// Created by yalavrinenko on 12.12.2019.
//

#include <fstream>
#include <vector>
#include <iostream>
#include <array>
#include <tuple>
#include <unordered_map>

struct vec3d{
    int x = 0, y = 0, z = 0;

    vec3d& operator+=(vec3d const &v){
      x += v.x; y += v.y; z += v.z;
      return *this;
    }
};

using mdsystem = std::array<vec3d, 4>;

void interact(std::array<vec3d, 4> const &r, std::array<vec3d, 4> &a){
  for (auto &va : a)
    va = {0, 0, 0};
  for (auto i = 0; i < 3; ++i)
    for (auto j = i + 1; j < 4; ++j){
      auto dx = 0; //(r[i].x > r[j].x) ? -1 : ((r[i].x == r[j].x) ? 0 : 1);
      auto dy = 0; //(r[i].y > r[j].y) ? -1 : ((r[i].y == r[j].y) ? 0 : 1);
      auto dz = 0; //(r[i].z > r[j].z) ? -1 : ((r[i].z == r[j].z) ? 0 : 1);

      a[i].x += dx; a[i].y += dy; a[i].z += dz;
      a[j].x += -dx; a[j].y += -dy; a[j].z += -dz;
    }
}

void propagate(mdsystem &r, mdsystem &v, mdsystem const &a){
  for (auto i = 0; i < 4; ++i){
    v[i] += a[i];
    r[i] += v[i];
  }
}

std::tuple<int, int> energy(mdsystem &r, mdsystem &v){
  int total = 0;
  int kinetic = 0;
  for (auto i = 0; i < 4; ++i){
    auto &vr = r[i];
    auto &vv = v[i];
    auto pot = std::abs(vr.x) + std::abs(vr.y) + std::abs(vr.z);
    auto kin = std::abs(vv.x) + std::abs(vv.y) + std::abs(vv.z);
    total += pot * kin;
    kinetic += kin;
  }


  return {kinetic, total};
}

int main(int argc, char** argv){
    std::array<vec3d, 4> r, v, a;
    std::ifstream in("../in.map");
    for (auto &rv : r){
      in >> rv.x >> rv.y >> rv.z;
    }

    auto ts = 0;

    bool find_state = false;
    do{
      interact(r, a);
      propagate(r, v, a);
      auto [t, k] = energy(r, v);
      ts++;
      if (ts % 100000 == 0)
        std::cout << ts << " " << k << std::endl;
      find_state = k == 0;
    } while (!find_state);

    std::cout << ts*2 << std::endl;

    return 0;
}
