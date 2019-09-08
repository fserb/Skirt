#pragma once

#include <vector>

#include "core/skirt.h"

namespace skirt {

class FilmTile {
 public:
  FilmTile(int x, int y, int width, int height)
      : x(x), y(y), width(width), height(height) {
    data.resize(width * height);
  }

  INLINE void WritePixel(int x, int y, const Vector3& rgb) {
    data[x + y * width] = rgb;
  }

  int x, y;
  int width, height;
  std::vector<Vector3> data;
};

class Film {
 public:
  Film(int width, int height, string filename)
      : width(width), height(height), filename(filename) {
    data.resize(width * height);
  }

  void MergeTile(const FilmTile& tile);

  void SaveImage();

  std::vector<Vector3> data;
  int width, height;
  string filename;

 private:
  void SaveImagePFM();
  void SaveImagePBM();
  void SaveImagePNG();
  void SaveImageEXR();
};

}  // namespace skirt
