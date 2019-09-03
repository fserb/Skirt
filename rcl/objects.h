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

  virtual const AABB bbox() const {
    return AABB(center - radius, center + radius);
  }

  vec3 center;
  float radius;
  shared_ptr<Material> material;
};

class HitList : public Hitable {
public:
  HitList() { }
  HitList(const HitList& l) {
    for (const shared_ptr<Hitable>& obj : l.list) {
      add(obj);
    }
  }

  void add(shared_ptr<Hitable> a) {
    list.push_back(a);
  }

  virtual Hit hit(const Ray& r, float minT, float maxT) const {
    Hit hit = NoHit;
    float closest = maxT;
    for (const shared_ptr<Hitable>& obj : list) {
      Hit h = obj->hit(r, minT, closest);
      if (h != NoHit) {
        hit = h;
        closest = h.t;
      }
    }

    return hit;
  }

  virtual const AABB bbox() const {
    if (list.size() == 0) return AABB();
    AABB ret;
    for (const shared_ptr<Hitable>& obj : list) {
      AABB b = obj->bbox();
      if (!b) continue;
      if (!ret) { ret = b; continue; }
      ret = AABB(min(b.min, ret.min), max(b.max, ret.max));
    }

    return ret;
  }

  vector<shared_ptr<Hitable>> list;
  int size;
};

class BVH : public Hitable {
public:
  BVH() {}
  BVH(vector<shared_ptr<Hitable>> list) {
    int axis = floor(3 * frand());
    sort(list.begin(), list.end(),
      [axis](shared_ptr<Hitable> a, shared_ptr<Hitable> b) {
        AABB bba = a->bbox();
        AABB bbb = b->bbox();
        return bba.min[axis] < bbb.min[axis];
      });
    if (list.size() == 1) {
      left = right = list[0];
    } else if (list.size() == 2) {
      left = list[0];
      right = list[1];
    } else {
      left = shared_ptr<Hitable>(new BVH(
        vector<shared_ptr<Hitable>>(list.begin(), list.begin() + list.size() / 2)));
      right = shared_ptr<Hitable>(new BVH(
        vector<shared_ptr<Hitable>>(list.begin() + list.size() / 2, list.end())));
    }

    AABB bbl = left->bbox();
    AABB bbr = right->bbox();

    box = AABB(min(bbl.min, bbr.min), max(bbl.max, bbr.max));
  }

  virtual Hit hit(const Ray& r, float minT, float maxT) const {
    if (box.hit(r, minT, maxT)) {
      Hit leftHit = left->hit(r, minT, maxT);
      Hit rightHit = right->hit(r, minT, maxT);
      if (leftHit && rightHit) {
        return leftHit.t < rightHit.t ? leftHit : rightHit;
      } else if (leftHit) {
        return leftHit;
      } else if (rightHit) {
        return rightHit;
      }
    }
    return NoHit;
  }

  virtual const AABB bbox() const {
    return box;
  }

  shared_ptr<Hitable> left;
  shared_ptr<Hitable> right;
  AABB box;
};

#endif // __OBJECTS_H__
