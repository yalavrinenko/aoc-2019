////
//// Created by yalavrinenko on 10.12.2019.
////
//#include <string>
//#include <iostream>
//#include <vector>
//#include <fstream>
//#include <cmath>
//#include <tuple>
//#include <algorithm>
//struct point{
//  int x;
//  int y;
//  int state = 0;
//
//  bool operator== (point const &p) const {
//    return x == p.x && y == p.y;
//  }
//};
//
//std::tuple<double, double> direction(int x, int y, int i, int j){
//  auto xi = i - x;
//  auto yi = j - y;
//
//  return {xi, yi};
//}
//
//double dist(int x1, int y1, int x2, int y2){
//  auto tx = x1 - x2;
//  auto ty = y1 - y2;
//  return std::sqrt(tx*tx + ty*ty);
//}
//
//size_t intersection_count(point const &p, point const &origin, std::vector<point> const &m){
//  auto [kx, ky] = direction(origin.x, origin.y, p.x, p.y);
//  auto d = dist(origin.x, origin.y, p.x, p.y);
//
//  auto sign  = [](auto x) { return (x < 0) ? -1 : 1; };
//
//  auto count = 0;
//  for (auto const &ip : m){
//    if (ip == p || ip == origin || ip.state != 0)
//      continue;
//    auto [tx, ty] = direction(origin.x, origin.y, ip.x, ip.y);
//
//    auto d1 = dist(origin.x, origin.y, ip.x, ip.y);
//    auto d2 = dist(p.x, p.y, ip.x, ip.y);
//
//    auto dot = tx * ky - ty * kx;
//    if (dot == 0 && std::abs(d1 + d2 - d) < 1e-4)
//      ++count;
//  }
//
//  return count;
//}
//
//size_t get_asteroid_numbers(std::vector<point> const &m, int x, int y){
//  auto count = 0;
//  for (auto const &p : m){
//    if (p.x == x && p.y == y)
//      continue;
//    else {
//      if (intersection_count(p, {x, y}, m) == 0)
//        ++count;
//    }
//  }
//  return count;
//}
//
//std::tuple<point, double> find_closest(std::vector<point> const &m, point o, double _angle){
//  double min_angle = -10;
//  point pmin;
//  for (auto &p : m){
//    if (p.state == 0 && intersection_count(p, o, m) == 0){
//      double x = p.y - o.y;
//      double y = o.x - p.x;
//      auto len = std::sqrt(x*x + y*y);
//      x /= len;
//      y /= len;
//      auto angle = std::atan2(y, x);
//      if (_angle >= angle && min_angle < angle){
//        min_angle = angle;
//        pmin = p;
//      }
//    }
//  }
//  return {pmin, min_angle};
//}
//
//int main(int argc, char** argv){
//  std::ifstream in("../in.map");
//  std::vector<point> m;
//
//  auto y = 0;
//
//  while (!in.eof()){
//    std::string s;
//    std::getline(in, s);
//    for (auto x = 0; x < s.size(); ++x) {
//      if (s[y] == '#')
//        m.emplace_back(point{x, y});
//    }
//    ++y;
//  }
//
//  double angle = M_PI / 2.0;
//  int count = 0;
//  while (true){
//    auto [p, angle_] = find_closest(m, {11, 13}, angle);
//    point p_ = p;
//    if (angle_ == -10 && count == m.size())
//      break;
//
//    if (angle_ == -10){
//      angle = M_PI;
//      continue;
//    }
//
//    auto it = std::find(m.begin(), m.end(), p_);
//    it->state = 1;
//    angle = angle_ - 1e-6;
//    if (angle < -M_PI)
//      angle = M_PI;
//
//    ++count;
//    std::cout << count << " " << it->x << " " << it->y << std::endl;
//  }
//
//  return 0;
//}

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <set>
#include <vector>

int gcd(int x, int y)
{
    return (y == 0) ? x : gcd(y, x % y);
}


struct ivec2 {
    int x, y;

    ivec2(int k) : x(k), y(k) {}
    ivec2(int xx, int yy) : x(xx), y(yy) {}
};
ivec2 operator + (ivec2 a, ivec2 b) { return ivec2{ a.x + b.x, a.y + b.y }; }
ivec2 operator - (ivec2 a, ivec2 b) { return ivec2{ a.x - b.x, a.y - b.y }; }
ivec2 operator / (ivec2 a, int k) { return ivec2{ a.x / k, a.y / k }; }
bool operator == (ivec2 a, ivec2 b) { return a.x == b.x && a.y == b.y; }
bool operator <  (ivec2 a, ivec2 b) { return (a.x != b.x) ? (a.x < b.x) : (a.y < b.y); }
int length(ivec2 a) { return gcd(std::abs(a.x), std::abs(a.y)); }
ivec2 normalize(ivec2 a) { return a / length(a); }


double to_angle(ivec2 a)
{
    double angle = std::atan2(double(a.x), double(-a.y));
    return (angle >= 0.0) ? angle : (2.0 * M_PI + angle);
}


bool load(const char* filename, std::vector<ivec2>& asteroids, int& w)
{
    FILE* f = fopen(filename, "r");
    if (f == nullptr) {
        return false;
    }

    char line[256];
    int y = 0;
    while (fgets(line, sizeof(line), f) != nullptr) {
        int x = 0;
        while (line[x] != '\n' && line[x] != '\0') {
            if (line[x] == '#') {
                asteroids.push_back(ivec2(x, y));
            }
            x++;
        }
        w = x;
        ++y;
    }
    fclose(f);

    return true;
}


bool polar_order(ivec2 a, ivec2 b)
{
    if (a == b) {
        return false;
    }
    int a_dist = length(a);
    int b_dist = length(b);
    ivec2 a_dir = a / a_dist;
    ivec2 b_dir = b / b_dist;
    if (a_dir == b_dir) {
        return a_dist < b_dist;
    }
    else {
        return to_angle(a_dir) < to_angle(b_dir);
    }
}


size_t best_asteroid(const std::vector<ivec2>& asteroids)
{
    const size_t n = asteroids.size();
    size_t maxVisible = 0;
    size_t bestI = 0;

    std::set<ivec2> visible;
    for (size_t i = 0; i < n; i++) {
        visible.clear();
        for (size_t j = 0; j < n; j++) {
            if (i == j) {
                continue;
            }
            visible.insert(normalize(asteroids[j] - asteroids[i]));
        }

        if (visible.size() > maxVisible) {
            maxVisible = visible.size();
            bestI = i;
        }
    }

    return bestI;
}


int main(int argc, char** argv)
{
    std::vector<ivec2> asteroids;
    int w;
    if (!load("../in.map", asteroids, w)) {
        fprintf(stderr, "Couldn't load %s\n", argv[1]);
        return 1;
    }

    const size_t n = asteroids.size() - 1;
    size_t bestI = best_asteroid(asteroids);
    printf("base is at {%d,%d} (asteroid %u)\n", asteroids[bestI].x, asteroids[bestI].y, uint32_t(bestI));

    std::vector<ivec2> directions;
    directions.reserve(n);
    for (size_t i = 0; i <= n; i++) {
        if (i == bestI) {
            continue;
        }
        directions.push_back(asteroids[i] - asteroids[bestI]);
    }
    std::sort(directions.begin(), directions.end(), polar_order);

    std::vector<bool> removed(directions.size(), false);
    size_t i = 0, num_removed = 0;
    while (num_removed < 200) {
        while (i < n && removed[i]) {
            ++i;
        }
        removed[i] = true;
        ++num_removed;
        ivec2 pos = directions[i] + asteroids[bestI];
        if (num_removed == 200) {
            printf("The 200th asteroid to be vaporized is at %d,%d\n", pos.x, pos.y);
            break;
        }

        size_t j = i + 1;
        while (j < n && (removed[j] || normalize(directions[j]) == normalize(directions[i]))) {
            ++j;
        }
        i = j % n;
    }

    return 0;
}