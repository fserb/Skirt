#ifndef __RAY_H__
#define __RAY_H__

#include <vector>
#include <emscripten.h>

#include "vec3.h"

using namespace std;

class Material;

class Ray {
public:
  Ray() {}
  Ray(const vec3& origin, const vec3& direction)
    : origin(origin), direction(direction) {}

  vec3 pointAt(float t) const {
    return origin + t * direction;
  }

  vec3 origin, direction;
};

class AABB {
public:
  AABB() {}
  AABB(const vec3& a, const vec3& b) : min(a), max(b) {}

  bool hit(const Ray& r, float minT, float maxT) const {
    for (int a = 0; a < 3; ++a) {
      float invD = 1.0 / r.direction[a];
      float t0 = (min[a] - r.origin[a]) * invD;
      float t1 = (max[a] - r.origin[a]) * invD;
      if (invD < 0) swap(t0, t1);
      minT = t0 > minT ? t0 : minT;
      maxT = t1 < maxT ? t1 : maxT;
      if (maxT < minT) return false;
    }
    return true;
  }

  operator bool() const {
    return min[0] != max[0] || min[1] != max[1] || min[2] != max[2];
  }

  vec3 min, max;
};

struct Hit {
  float t;
  vec3 p;
  vec3 normal;
  vec3 uv;
  shared_ptr<Material> material;

  operator bool() const {
    return normal[0] != 0 || normal[1] != 0 || normal[2] != 0;
  }
};

Hit NoHit{0, vec3(0, 0, 0), vec3(0, 0, 0)};

class Hitable {
public:
  virtual Hit hit(const Ray& r, float minT, float maxT) const = 0;
  virtual const AABB bbox() const = 0;
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

class Camera {
public:
  Camera() {}
  Camera(vec3 lookfrom, vec3 lookat, vec3 up, float vfov, float aspect, float aperture, float focusDist) {
    lensRadius = aperture / 2;
    float theta = vfov * M_PI / 180.;
    float halfHeight = tan(theta / 2);
    float halfWidth = aspect * halfHeight;

    origin = lookfrom;
    w = unit(lookfrom - lookat);
    u = unit(cross(up, w));
    v = cross(w, u);

    lowerLeftCorner = origin - halfWidth * u  * focusDist
      - halfHeight * v  * focusDist - w * focusDist;
    horizontal = 2 * halfWidth * u * focusDist;
    vertical = 2 * halfHeight * v * focusDist;
  }

  Ray getRay(float s, float t) {
    vec3 rd = lensRadius * randomInUnitDisk();
    vec3 offset = u * rd.x() + v * rd.y();
    return Ray(origin + offset,
      lowerLeftCorner + s * horizontal + t * vertical - origin - offset);
  }

  vec3 origin;
  vec3 lowerLeftCorner;
  vec3 horizontal;
  vec3 vertical;
  vec3 u, v, w;
  float lensRadius;
};

#endif
