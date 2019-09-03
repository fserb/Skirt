#ifndef __OBJECTS_H__
#define __OBJECTS_H__

#include <vector>

#include "vec3.h"
#include "raytracer.h"
#include "texture.h"

using namespace std;

class Sphere : public Hitable {
public:
  static shared_ptr<Sphere> create(vec3 center, float r, shared_ptr<Material> mat) {
    return shared_ptr<Sphere>(new Sphere(center, r, mat));
  }
  Sphere() {}
  Sphere(vec3 center, float r, shared_ptr<Material> m)
    : center(center), radius(r), material(m) { }
  virtual Hit hit(const Ray& r, float minT, float maxT) const {
    vec3 oc = r.origin - center;
    float a = dot(r.direction, r.direction);
    float b = dot(oc, r.direction);
    float c = dot(oc, oc) - radius * radius;
    float disc = b * b - a * c;
    if (disc > 0) {
      float tmp = (-b - sqrt(disc)) / a;
      if (tmp > minT && tmp < maxT) {
        vec3 rp = r.pointAt(tmp);
        return Hit{tmp, rp, (rp - center) / radius, material};
      }
      tmp = (-b + sqrt(disc)) / a;
      if (tmp > minT && tmp < maxT) {
        vec3 rp = r.pointAt(tmp);
        return Hit{tmp, rp, (rp - center) / radius, material};
      }
    }
    return NoHit;
  }

  vec3 center;
  float radius;
  shared_ptr<Material> material;
};

class HitList : public Hitable {
public:
  HitList() { }

  void add(shared_ptr<Sphere> a) {
    list.push_back(a);
  }

  virtual Hit hit(const Ray& r, float minT, float maxT) const {
    Hit hit = NoHit;
    float closest = maxT;
    for (const shared_ptr<Sphere>& obj : list) {
      Hit h = obj->hit(r, minT, closest);
      if (h != NoHit) {
        hit = h;
        closest = h.t;
      }
    }

    return hit;
  }

  vector<shared_ptr<Sphere>> list;
  int size;
};

#endif // __OBJECTS_H__
