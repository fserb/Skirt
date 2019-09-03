#ifndef __OBJECTS_H__
#define __OBJECTS_H__

#include <limits>

#include "vec3.h"
#include "mat4.h"
#include "raytracer.h"
#include "texture.h"

using namespace std;

class FlipNormals : public Hitable {
public:
  FlipNormals(shared_ptr<Hitable> obj) : obj(obj) { }

  virtual Hit hit(const Ray& r, float minT, float maxT) const {
    Hit h = obj->hit(r, minT, maxT);
    if (h) {
      h.normal = -h.normal;
    }
    return h;
  }

  virtual const AABB bbox() const {
    return obj->bbox();
  }

  shared_ptr<Hitable> obj;
};

class RectXY : public Hitable {
public:
  RectXY(float x0, float x1, float y0, float y1, float z, shared_ptr<Material> m)
    : x0(x0), x1(x1), y0(y0), y1(y1), z(z), material(m) { }

  virtual Hit hit(const Ray& r, float minT, float maxT) const {
    float t = (z - r.origin.z()) / r.direction.z();
    if (t < minT || t > maxT) return NoHit;
    float x = r.origin.x() + t * r.direction.x();
    float y = r.origin.y() + t * r.direction.y();
    if (x < x0 || x > x1 || y < y0 || y > y1) return NoHit;
    return Hit{t, r.pointAt(t), vec3(0, 0, 1),
      vec3((x - x0) / (x1 - x0), (y - y0) / (y1 - y0), 0),
      material};
  }

  virtual const AABB bbox() const {
    return AABB(vec3(x0, y0, z - 0.0001), vec3(x1, y1, z + 0.0001));
  }

  float x0, x1, y0, y1, z;
  shared_ptr<Material> material;
};

class RectXZ : public Hitable {
public:
  RectXZ(float x0, float x1, float z0, float z1, float y, shared_ptr<Material> m)
    : x0(x0), x1(x1), z0(z0), z1(z1), y(y), material(m) { }

  virtual Hit hit(const Ray& r, float minT, float maxT) const {
    float t = (y - r.origin.y()) / r.direction.y();
    if (t < minT || t > maxT) return NoHit;
    float x = r.origin.x() + t * r.direction.x();
    float z = r.origin.z() + t * r.direction.z();
    if (x < x0 || x > x1 || z < z0 || z > z1) return NoHit;
    return Hit{t, r.pointAt(t), vec3(0, 1, 0),
      vec3((x - x0) / (x1 - x0), (z - z0) / (z1 - z0), 0),
      material};
  }

  virtual const AABB bbox() const {
    return AABB(vec3(x0, y - 0.0001, z0), vec3(x1, y + 0.0001, z1));
  }

  float x0, x1, z0, z1, y;
  shared_ptr<Material> material;
};

class RectYZ : public Hitable {
public:
  RectYZ(float y0, float y1, float z0, float z1, float x, shared_ptr<Material> m)
    : y0(y0), y1(y1), z0(z0), z1(z1), x(x), material(m) { }

  virtual Hit hit(const Ray& r, float minT, float maxT) const {
    float t = (x - r.origin.x()) / r.direction.x();
    if (t < minT || t > maxT) return NoHit;
    float y = r.origin.y() + t * r.direction.y();
    float z = r.origin.z() + t * r.direction.z();
    if (y < y0 || y > y1 || z < z0 || z > z1) return NoHit;
    return Hit{t, r.pointAt(t), vec3(1, 0, 0),
      vec3((y - y0) / (y1 - y0), (z - z0) / (z1 - z0), 0),
      material};
  }

  virtual const AABB bbox() const {
    return AABB(vec3(x - 0.0001, y0, z0), vec3(x + 0.0001, y1, z1));
  }

  float y0, y1, z0, z1, x;
  shared_ptr<Material> material;
};

class Box : public Hitable {
public:
  Box(vec3 p0, vec3 p1, shared_ptr<Material> m) : pmin(p0), pmax(p1), material(m) {
    list = make_shared<HitList>();
    list->add(make_shared<RectXY>(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), material));
    list->add(make_shared<FlipNormals>(
      make_shared<RectXY>(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), material)));

    list->add(make_shared<RectXZ>(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), material));
    list->add(make_shared<FlipNormals>(
      make_shared<RectXZ>(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), material)));

    list->add(make_shared<RectYZ>(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), material));
    list->add(make_shared<FlipNormals>(
      make_shared<RectYZ>(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), material)));
  }

  virtual Hit hit(const Ray& r, float minT, float maxT) const {
    return list->hit(r, minT, maxT);
  }

  virtual const AABB bbox() const {
    return AABB(pmin, pmax);
  }

  vec3 pmin, pmax;
  shared_ptr<HitList> list;
  shared_ptr<Material> material;
};

class Sphere : public Hitable {
public:
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
      float t = (-b - sqrt(disc)) / a;
      if (t > minT && t < maxT) {
        vec3 rp = r.pointAt(t);
        vec3 normal = (rp - center) / radius;
        return Hit{t, rp, normal, getSphereUV(normal), material};
      }
      t = (-b + sqrt(disc)) / a;
      if (t > minT && t < maxT) {
        vec3 rp = r.pointAt(t);
        vec3 normal = (rp - center) / radius;
        return Hit{t, rp, normal, getSphereUV(normal), material};
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

class Transform : public Hitable {
public:
  Transform(const mat4& mat, shared_ptr<Hitable> obj) : mat(mat), obj(obj) {
    invMat.copyFrom(mat).invert();

    const AABB& orig = obj->bbox();

    vec3 points[8] = {
      vec3(orig.min.x(), orig.min.y(), orig.min.z()),
      vec3(orig.min.x(), orig.min.y(), orig.max.z()),
      vec3(orig.min.x(), orig.max.y(), orig.min.z()),
      vec3(orig.min.x(), orig.max.y(), orig.max.z()),
      vec3(orig.max.x(), orig.min.y(), orig.min.z()),
      vec3(orig.max.x(), orig.min.y(), orig.max.z()),
      vec3(orig.max.x(), orig.max.y(), orig.min.z()),
      vec3(orig.max.x(), orig.max.y(), orig.max.z())};

    aabb.min = vec3(numeric_limits<float>::max(), numeric_limits<float>::max(), numeric_limits<float>::max());
    aabb.max = vec3(-numeric_limits<float>::max(), -numeric_limits<float>::max(), -numeric_limits<float>::max());

    for (int i = 0; i < 8; ++i) {
      vec3 p = transform(points[i], mat);
      for (int j = 0; j < 3; ++j) {
        if (p[j] < aabb.min[j]) aabb.min[j] = p[j];
        if (p[j] > aabb.max[j]) aabb.max[j] = p[j];
      }
    }
  }

  virtual Hit hit(const Ray& r, float minT, float maxT) const {
    Ray tr(transform(r.origin, invMat), transformNormal(r.direction, invMat));
    Hit h = obj->hit(tr, minT, maxT);
    if (!h) return NoHit;
    h.p = transform(h.p, mat);
    h.normal = unit(transformNormal(h.normal, mat));
    return h;
  }

  virtual const AABB bbox() const {
    return aabb;
  }

  mat4 mat;
  mat4 invMat;
  shared_ptr<Hitable> obj;
  AABB aabb;
};

#endif // __OBJECTS_H__
