#include "shapes/Sphere.h"

#include "core/skirt.h"

#include "core/AABB.h"

namespace skirt {

const AABB Sphere::Bound() const {
  return AABB(Vector3(-radius, -radius, -radius),
              Vector3(radius, radius, radius));
}

optional<Hit> Sphere::Intersect(const Ray& r) const {
  Vector3 ro = r.origin - Vector3(0, 0, -1);
  float a = Dot(r.direction, r.direction);
  float b = Dot(ro, r.direction);
  float c = Dot(ro, ro) - radius * radius;
  float disc = b * b - a * c;
  if (disc > 0) {
    float t = (-b - sqrt(disc)) / a;
    Vector3 rp;
    if (t > r.minT && t < r.maxT) {
      rp = r.pointAt(t);
      Vector3 normal = (rp - Vector3(0, 0, -1)) / radius;
      return Hit(t, rp, normal);
    }
    t = (-b + sqrt(disc)) / a;
    if (t > r.minT && t < r.maxT) {
      Vector3 rp = r.pointAt(t);
      Vector3 normal = (rp - Vector3(0, 0, -1)) / radius;
      return Hit(t, rp, normal);
    }
  }
  return nullopt;
}

float Sphere::Area() const {
  return 4 * PI * radius * radius;
}

}  // namespace skirt
