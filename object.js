import {AABB} from "./raytracer.js";
import {v3, getSphereUV} from "./utils.js";

class FlipNormal {
  constructor(obj) { this.obj = obj; }

  hit(r, minT, maxT) {
    const h = this.obj.hit(r, minT, maxT);
    if (h) {
      h.normal = v3.mul(-1, h.normal);
    }
    return h;
  }

  bbox() {
    return this.obj.bbox();
  }
}

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

class RectXZ {
  constructor(x0, z0, x1, z1, y, material) {
    this.x0 = x0; this.z0 = z0; this.x1 = x1; this.z1 = z1;
    this.y = y;
    this.material = material;
  }

  hit(r, minT, maxT) {
    const t = (this.y - r.origin.y) / r.direction.y;
    if (t < minT || t > maxT) return null;
    const x = r.origin.x + t * r.direction.x;
    const z = r.origin.z + t * r.direction.z;
    if (x < this.x0 || x > this.x1 || z < this.z0 || z > this.z1) {
      return null;
    }
    return {
      uv: [(x - this.x0) / (this.x1 - this.x0),
        (z - this.z0) / (this.z1 - this.z0)],
      t: t,
      material: this.material,
      p: r.pointAt(t),
      normal: v3.new(0, 1, 0) };
  }

  bbox() {
    return new AABB(v3.new(this.x0, this.y - 0.0001, this.z0),
      v3.new(this.x1, this.y + 0.0001, this.z1));
  }
}

class RectYZ {
  constructor(y0, z0, y1, z1, x, material) {
    this.y0 = y0; this.z0 = z0; this.y1 = y1; this.z1 = z1;
    this.x = x;
    this.material = material;
  }

  hit(r, minT, maxT) {
    const t = (this.x - r.origin.x) / r.direction.x;
    if (t < minT || t > maxT) return null;
    const y = r.origin.y + t * r.direction.y;
    const z = r.origin.z + t * r.direction.z;
    if (y < this.y0 || y > this.y1 || z < this.z0 || z > this.z1) {
      return null;
    }
    return {
      uv: [(y - this.y0) / (this.y1 - this.y0),
        (z - this.z0) / (this.z1 - this.z0)],
      t: t,
      material: this.material,
      p: r.pointAt(t),
      normal: v3.new(1, 0, 0) };
  }

  bbox() {
    return new AABB(v3.new(this.x - 0.0001, this.y0, this.z0),
      v3.new(this.x + 0.0001, this.y1, this.z1));
  }
}

export {FlipNormal, RectXY, RectXZ, RectYZ, Sphere};
