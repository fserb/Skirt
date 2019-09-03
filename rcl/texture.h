#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <memory>

#include "utils.h"
#include "vec3.h"
#include "raytracer.h"

class Texture {
public:
  virtual vec3 value(float u, float v, const vec3& p) const = 0;
};

class ConstantTexture : public Texture {
public:
  ConstantTexture(float r, float g, float b) : color(r, g, b) { }

  virtual vec3 value(float u, float v, const vec3& p) const {
    return color;
  }
  vec3 color;
};

class CheckerTexture : public Texture {
public:
  CheckerTexture(shared_ptr<Texture> t0, shared_ptr<Texture> t1)
    : even(t0), odd(t1) { }

  virtual vec3 value(float u, float v, const vec3& p) const {
    float sines = sin(10 * p.x())*sin(10 * p.y())*sin(10 * p.z());
    if (sines < 0) {
      return odd->value(u, v, p);
    } else {
      return even->value(u, v, p);
    }
  }

  shared_ptr<Texture> even, odd;
};



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
    return Scatter{Ray(hit.p, target - hit.p), albedo->value(0, 0, hit.p)};
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
      return Scatter{scattered, albedo->value(0, 0, hit.p)};
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
