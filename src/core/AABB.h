#ifndef __CORE_AABB_H__
#define __CORE_AABB_H__

#include "skirt.h"

namespace skirt {

class AABB {
 public:
  AABB()
      : minp(MaxFloat, MaxFloat, MaxFloat),
        maxp(-MaxFloat, -MaxFloat, -MaxFloat) {}
  AABB(const Vector3& p1, const Vector3& p2)
      : minp(min(p1.x, p2.x), min(p1.y, p2.y), min(p1.z, p2.z)),
        maxp(max(p1.x, p2.x), max(p1.y, p2.y), max(p1.z, p2.z)) {
    DCHECK(!HasNaNs());
  }
  bool HasNaNs() {
    return minp.HasNaNs() || maxp.HasNaNs();
  }

  INLINE Vector3& operator[](int i) {
    DCHECK(i == 0 || i == 1);
    if (i == 0) return minp;
    return maxp;
  }

  INLINE const Vector3& operator[](int i) const {
    DCHECK(i == 0 || i == 1);
    if (i == 0) return minp;
    return maxp;
  }

  INLINE Vector3 Corner(int corner) const {
    DCHECK(corner >= 0 && corner < 8);
    return Vector3((*this)[(corner & 1)].x,
                   (*this)[(corner & 2) ? 1 : 0].y,
                   (*this)[(corner & 4) ? 1 : 0].z);
  }

  INLINE Vector3 Diagonal() const {
    return maxp - minp;
  }

  INLINE float SurfaceArea() const {
    const Vector3& d = Diagonal();
    return 2 * (d.x * d.y + d.x * d.z + d.y * d.z);
  }

  INLINE float Volume() const {
    const Vector3& d = Diagonal();
    return d.x * d.y * d.z;
  }

  INLINE int LongestDimension() const {
    const Vector3& d = Diagonal();
    if (d.x > d.y && d.x > d.z) return 0;
    if (d.y > d.z) return 1;
    return 2;
  }

  INLINE Vector3 Offset(const Vector3& p) const {
    Vector3 ret = p - minp;
    if (maxp.x > minp.x) ret.x /= maxp.x - minp.x;
    if (maxp.y > minp.y) ret.y /= maxp.y - minp.y;
    if (maxp.z > minp.z) ret.z /= maxp.z - minp.z;
    return ret;
  }

  INLINE tuple<Vector3, float> BoundingSphere() const;

  Vector3 minp, maxp;
};

INLINE AABB Union(const AABB& b, const Vector3& p) {
  return AABB(
      Vector3(min(b.minp.x, p.x), min(b.minp.y, p.y), min(b.minp.z, p.z)),
      Vector3(max(b.maxp.x, p.x), max(b.maxp.y, p.y), max(b.maxp.z, p.z)));
}

INLINE AABB Union(const AABB& a, const AABB& b) {
  return AABB(Vector3(min(a.minp.x, b.minp.x),
                      min(a.minp.y, b.minp.y),
                      min(a.minp.z, b.minp.z)),
              Vector3(max(a.maxp.x, b.maxp.x),
                      max(a.maxp.y, b.maxp.y),
                      max(a.maxp.z, b.maxp.z)));
}

INLINE AABB Intersect(const AABB& a, const AABB& b) {
  return AABB(Vector3(max(a.minp.x, b.minp.x),
                      max(a.minp.y, b.minp.y),
                      max(a.minp.z, b.minp.z)),
              Vector3(min(a.maxp.x, b.maxp.x),
                      min(a.maxp.y, b.maxp.y),
                      min(a.maxp.z, b.maxp.z)));
}

INLINE bool Overlaps(const AABB& a, const AABB& b) {
  return ((a.maxp.x >= b.minp.x && a.minp.x <= b.maxp.x) &&
          (a.maxp.y >= b.minp.y && a.minp.y <= b.maxp.y) &&
          (a.maxp.z >= b.minp.z && a.minp.z <= b.maxp.z));
}

INLINE bool Inside(const AABB& b, const Vector3& p) {
  return ((p.x >= b.minp.x && p.x <= b.maxp.x) &&
          (p.y >= b.minp.y && p.y <= b.maxp.y) &&
          (p.z >= b.minp.z && p.z <= b.maxp.z));
}

INLINE AABB Expand(const AABB& b, float delta) {
  return AABB(b.minp - Vector3(delta, delta, delta),
              b.maxp + Vector3(delta, delta, delta));
}

INLINE tuple<Vector3, float> AABB::BoundingSphere() const {
  Vector3 center = (minp + maxp) / 2;
  float radius = Inside(*this, center) ? Distance(center, maxp) : 0;
  return make_tuple(center, radius);
}

}  // namespace skirt

#endif  // __CORE_AABB_H__
