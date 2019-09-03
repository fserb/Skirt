#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <memory>

#include "utils.h"
#include "vec3.h"
#include "raytracer.h"
#include "perlin.h"

class Texture {
public:
  virtual vec3 value(vec3 uv, const vec3& p) const = 0;
};

class ConstantTexture : public Texture {
public:
  ConstantTexture(float r, float g, float b) : color(r, g, b) { }

  virtual vec3 value(vec3 uv, const vec3& p) const {
    return color;
  }
  vec3 color;
};

class CheckerTexture : public Texture {
public:
  CheckerTexture(shared_ptr<Texture> t0, shared_ptr<Texture> t1)
    : even(t0), odd(t1) { }

  virtual vec3 value(vec3 uv, const vec3& p) const {
    float sines = sin(10 * p.x())*sin(10 * p.y())*sin(10 * p.z());
    if (sines < 0) {
      return odd->value(uv, p);
    } else {
      return even->value(uv, p);
    }
  }

  shared_ptr<Texture> even, odd;
};

class NoiseTexture : public Texture {
public:
  NoiseTexture(float scale) : scale(scale) {}
  virtual vec3 value(vec3 uv, const vec3& p) const {
    return vec3(1,1,1)*0.5*(1 + sin(scale*p.z() + 5*noise.turb(scale*p))) ;
  }

  Perlin noise;
  float scale;
};

EM_JS(void, LoadImage, (const char* name, int size, void* ptr), {
  self.LoadImage(name, size, ptr);
});

class ImageTexture : public Texture {
public:
  ImageTexture(string name) : data(nullptr) {
    LoadImage(name.c_str(), name.length(), this);
  }

  virtual vec3 value(vec3 uv, const vec3& p) const {
    if (!data) return vec3(0, 1, 0);
    int i = uv[0] * width;
    int j = (1 - uv[1]) * height - 0.001;
    if (i < 0) i = 0;
    if (j < 0) j = 0;
    if (i >= width) i = width - 1;
    if (j >= height) j = height - 1;
    int ptr = 4 * (i + j * width);
    return vec3(
      int(data[ptr]) / 255.0,
      int(data[ptr + 1]) / 255.0,
      int(data[ptr + 2]) / 255.0);
  }

  unsigned char* data;
  int width;
  int height;
};

extern "C" {

EMSCRIPTEN_KEEPALIVE
void UploadImage(void* ptr, unsigned char* data, int width, int height) {
  ImageTexture* it = static_cast<ImageTexture*>(ptr);
  it->width = width;
  it->height = height;
  it->data = data;
}

} // extern

struct Scatter {
  Ray scattered;
  vec3 attenuation;

  operator bool() const {
    return scattered.direction[0] != 0 || scattered.direction[1] != 0 || scattered.direction[2] != 0;
  }
};

Scatter NoScatter{Ray{vec3(0, 0, 0), vec3(0, 0, 0)}, vec3(0, 0, 0)};

class Material {
public:
  virtual Scatter scatter(const Ray& ray, const Hit& hit) const = 0;
};

class Lambertian : public Material {
public:
  Lambertian(const shared_ptr<Texture> a) : albedo(a) { }

  virtual Scatter scatter(const Ray& ray, const Hit& hit) const {
    vec3 target = hit.p + hit.normal + randomInUnitSphere();
    return Scatter{Ray(hit.p, target - hit.p), albedo->value(hit.uv, hit.p)};
  }

  shared_ptr<Texture> albedo;
};

class Metal : public Material {
public:
  Metal(const shared_ptr<Texture> a, float f) : albedo(a), fuzz(f) { }

  virtual Scatter scatter(const Ray& ray, const Hit& hit) const {
    vec3 reflected = reflect(unit(ray.direction), hit.normal);
    Ray scattered(hit.p, reflected + fuzz * randomInUnitSphere());
    if (dot(scattered.direction, hit.normal) > 0) {
      return Scatter{scattered, albedo->value(hit.uv, hit.p)};
    }
    return NoScatter;
  }

  const shared_ptr<Texture> albedo;
  float fuzz;
};

class Dielectric : public Material {
public:
  Dielectric(float ri) : refidx(ri) { }

  float schlick(float cosine, float refIdx) const {
    float r0 = (1 - refIdx) / (1 + refIdx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
  }


  virtual Scatter scatter(const Ray& ray, const Hit& hit) const {
    vec3 reflected = reflect(ray.direction, hit.normal);
    float nint;
    vec3 outnormal;
    float cosine;
    if (dot(ray.direction, hit.normal) > 0) {
      outnormal = -hit.normal;
      nint = refidx;
      cosine = refidx * dot(ray.direction, hit.normal) / ray.direction.length();
    } else {
      outnormal = hit.normal;
      nint = 1. / refidx;
      cosine = -refidx * dot(ray.direction, hit.normal) / ray.direction.length();
    }

    vec3 refracted = refract(ray.direction, outnormal, nint);
    float reflectProb = refracted ? schlick(cosine, refidx) : 1.0;

    if (frand() < reflectProb) {
      return Scatter{Ray(hit.p, reflected), vec3(1, 1, 1)};
    } else {
      return Scatter{Ray(hit.p, refracted), vec3(1, 1, 1)};
    }
  }

  float refidx;
};

#endif
