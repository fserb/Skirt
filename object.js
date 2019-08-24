import {AABB} from "./raytracer.js";
import {v3, getSphereUV} from "./utils.js";

class Sphere {
  constructor(center, radius, material) {
    this.center = center;
    this.radius = radius;
    this.material = material;
  }

  hit(r, minT, maxT) {
    const oc = v3.sub(r.origin, this.center);
    const a = v3.dot(r.direction, r.direction);
    const b = v3.dot(oc, r.direction);
    const c = v3.dot(oc, oc) - this.radius * this.radius;
    const delta = b * b - a * c;
    if (delta > 0) {
      let t = (-b - Math.sqrt(delta)) / a;
      if (t < maxT && t > minT) {
        const p = r.pointAt(t);
        const normal = v3.div(v3.sub(p, this.center), this.radius);
        return { t: t, p: p,
          uv: getSphereUV(normal),
          normal: normal,
          material: this.material };
      }
      t = (-b + Math.sqrt(delta)) / a;
      if (t < maxT && t > minT) {
        const p = r.pointAt(t);
        const normal = v3.div(v3.sub(p, this.center), this.radius);
        return { t: t, p: p,
          uv: getSphereUV(normal),
          normal: normal,
          material: this.material };
      }
    }
    return null;
  }

  bbox() {
    return new AABB(v3.sub(this.center, this.radius),
      v3.add(this.center, this.radius));
  }
}

class RectXY {
  constructor(x0, y0, x1, y1, z, material) {
    this.x0 = x0; this.y0 = y0; this.x1 = x1; this.y1 = y1;
    this.z = z;
    this.material = material;
  }

  hit(r, minT, maxT) {
    const t = (this.z - r.origin.z) / r.direction.z;
    if (t < minT || t > maxT) return null;
    const x = r.origin.x + t * r.direction.x;
    const y = r.origin.y + t * r.direction.y;
    if (x < this.x0 || x > this.x1 || y < this.y0 || y > this.y1) {
      return null;
    }
    return {
      uv: [(x - this.x0) / (this.x1 - this.x0),
        (y - this.y0) / (this.y1 - this.y0)],
      t: t,
      material: this.material,
      p: r.pointAt(t),
      normal: v3.new(0, 0, 1) };
  }

  bbox() {
    return new AABB(v3.new(this.x0, this.y0, this.z - 0.0001),
      v3.new(this.x1, this.y1, this.z + 0.0001));
  }
}

export {RectXY, Sphere};
