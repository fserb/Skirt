#include "core/Film.h"

#include <filesystem>
#include <fstream>

#include "core/skirt.h"

#include "3rdp/lodepng.h"
#define TINYEXR_IMPLEMENTATION
#include "3rdp/tinyexr.h"

#ifdef BIG_ENDIAN
constexpr float endianness = -1.0f;
#else
constexpr float endianness = 1.0f;
#endif

namespace skirt {

void Film::MergeTile(const FilmTile& tile) {
  for (int y = 0; y < tile.height; ++y) {
    for (int x = 0; x < tile.width; ++x) {
      int px = x + tile.x;
      int py = y + tile.y;
      data[px + py * width] = tile.data[x + y * tile.width];
    }
  }
}

// http://netpbm.sourceforge.net/doc/pfm.html
void Film::SaveImagePFM() {
  DVLOG(1) << "Saving file PFM: " << filename;
  std::ofstream file(filename);

  string header = StringPrintf("PF\n%d %d\n%f\n", width, height, endianness);
  file.write(header.c_str(), header.length());

  CHECK(sizeof(float) == 4);

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      const Vector3& v = data[x + y * width];
      file.write(reinterpret_cast<const char*>(&v.x), sizeof(float));
      file.write(reinterpret_cast<const char*>(&v.y), sizeof(float));
      file.write(reinterpret_cast<const char*>(&v.z), sizeof(float));
    }
  }

  file.close();
}

void Film::SaveImagePBM() {
  DVLOG(1) << "Saving file PBM: " << filename;
  std::ofstream file(filename);
  string header = StringPrintf("P3\n%d %d\n255\n", width, height);
  file.write(header.c_str(), header.length());

  for (int y = height - 1; y >= 0; --y) {
    for (int x = 0; x < width; ++x) {
      const Vector3& v = data[x + y * width];
      int r = int(255.99 * v.x);
      int g = int(255.99 * v.y);
      int b = int(255.99 * v.z);
      file << StringPrintf("%d %d %d\n", r, g, b);
    }
  }

  file.close();
}

void Film::SaveImagePNG() {
  DVLOG(1) << "Saving file PNG: " << filename;
  std::unique_ptr<unsigned char[]> image(
      new unsigned char[width * height * 4]());

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      const Vector3& v = data[x + y * width];
      int r = int(255.99 * v.x);
      int g = int(255.99 * v.y);
      int b = int(255.99 * v.z);
      const int p = (x + y * width) * 4;
      image[p] = r;
      image[p + 1] = g;
      image[p + 2] = b;
      image[p + 3] = 255;
    }
  }

  unsigned int error = lodepng::encode(filename, image.get(), width, height);
  if (error) {
    LOG(ERROR) << "PNG encoder error: " << lodepng_error_text(error);
  }
}

void Film::SaveImageEXR() {
  DVLOG(1) << "Saving file EXR: " << filename;

  EXRHeader header;
  InitEXRHeader(&header);

  EXRImage image;
  InitEXRImage(&image);

  image.num_channels = 3;

  std::vector<float> images[3];
  images[0].resize(width * height);
  images[1].resize(width * height);
  images[2].resize(width * height);

  for (int i = 0; i < width * height; i++) {
    const Vector3& v = data[i];
    images[0][i] = v.x;
    images[1][i] = v.y;
    images[2][i] = v.z;
  }

  float* image_ptr[3];
  image_ptr[0] = &(images[2].at(0));  // B
  image_ptr[1] = &(images[1].at(0));  // G
  image_ptr[2] = &(images[0].at(0));  // R

  image.images = (unsigned char**)image_ptr;
  image.width = width;
  image.height = height;

  header.num_channels = 3;
  header.channels =
      (EXRChannelInfo*)malloc(sizeof(EXRChannelInfo) * header.num_channels);
  // Must be (A)BGR order, since most of EXR viewers expect this channel order.
  strncpy(header.channels[0].name, "B", 255);
  header.channels[0].name[strlen("B")] = '\0';
  strncpy(header.channels[1].name, "G", 255);
  header.channels[1].name[strlen("G")] = '\0';
  strncpy(header.channels[2].name, "R", 255);
  header.channels[2].name[strlen("R")] = '\0';

  header.pixel_types = (int*)malloc(sizeof(int) * header.num_channels);
  header.requested_pixel_types =
      (int*)malloc(sizeof(int) * header.num_channels);
  for (int i = 0; i < header.num_channels; i++) {
    header.pixel_types[i] = TINYEXR_PIXELTYPE_FLOAT;
    header.requested_pixel_types[i] = TINYEXR_PIXELTYPE_HALF;
  }

  const char* err = nullptr;
  int ret = SaveEXRImageToFile(&image, &header, filename.c_str(), &err);
  if (ret != TINYEXR_SUCCESS) {
    LOG(ERROR) << "Save EXR err: " << err;
    FreeEXRErrorMessage(err);  // free's buffer for an error message
    return;
  }

  free(header.channels);
  free(header.pixel_types);
  free(header.requested_pixel_types);
}

void Film::SaveImage() {
  std::filesystem::path p(filename);
  string ext = p.extension();
  transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

  if (ext == ".pfm") return SaveImagePFM();
  if (ext == ".pbm") return SaveImagePBM();
  if (ext == ".png") return SaveImagePNG();
  if (ext == ".exr") return SaveImageEXR();

  LOG(ERROR) << "Couldn't save file: " << filename
             << " with type: " << p.extension();
}

}  // namespace skirt
