//
// Created by yalavrinenko on 08.12.2019.
//

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <iterator>

int main(int argc, char** argv){
  std::ifstream in("../in.map");
  std::string img; in >> img;

  auto height = 6;
  auto width = 25;
  auto const lsize = width * height;

  std::vector<std::string> layers(img.size() / lsize, std::string(lsize, ' '));

  for (auto lid = 0; lid < img.size() / lsize; ++lid){
    std::copy(img.begin() + lid * lsize, img.begin() + (lid + 1) * lsize, layers[lid].begin());
  }

  auto get_pixel = [&layers](auto pixel_position){
    for (auto &layer : layers){
      if (layer[pixel_position] == '0')
        return ' ';
      if (layer[pixel_position] == '1')
        return '*';
    }
    return ' ';
  };

  std::string decoded(lsize, ' ');
  for (auto i = 0; i < lsize; ++i)
    decoded[i] = get_pixel(i);

  for (auto i = 0; i < height; ++i){
    std::copy(decoded.begin() + i*width, decoded.begin() + (i + 1) * width, std::ostream_iterator<char>(std::cout, ""));
    std::cout << "\n";
  }
}