class Vec3 extends Array {
  constructor(x, y, z) {
    super(3);
    this[0] = x;
    this[1] = y;
    this[2] = z;
  }

  get x() { return this[0]; }
  get y() { return this[1]; }
  get z() { return this[2]; }

  get r() { return this[0]; }
  get g() { return this[1]; }
  get b() { return this[2]; }

  add(o) { return new Vec3(this[0] + o[0], this[1] + o[1], this[2] + o[2]); }
  sub(o) { return new Vec3(this[0] - o[0], this[1] - o[1], this[2] - o[2]); }
  mul(o) { return new Vec3(this[0] * o[0], this[1] * o[1], this[2] * o[2]); }
  div(o) { return new Vec3(this[0] / o[0], this[1] / o[1], this[2] / o[2]); }

  muls(t) { return new Vec3(this[0] * t, this[1] * t, this[2] * t); }
  divs(t) { return new Vec3(this[0] / t, this[1] / t, this[2] / t); }

  dot(o) { return this[0] * o[0] + this[1] * o[1] + this[2] * o[2]; }
  cross(o) {
    return new Vec3(this[1] * o[2] - this[2] * o[1],
      -(this[0] * o[2] - this[2] * o[0]),
      this[0] * o[1] - this[1] * o[0]);
  }

  get len() {
    return Math.sqrt(this.sqlen);
  }

  get sqlen() {
    return this[0] * this[0] + this[1] * this[1] + this[2] * this[2];
  }

  unit() { return this.divs(this.len); }
}

class Ray {
  constructor(a, b) {
    this.a = a;
    this.b = b;
  }
  get origin() { return this.a; }
  get direction() { return this.b; }

  pointAt(t) { return this.a.add(this.b.muls(t)); }
}

class Sphere {
  constructor(center, radius) {
    this.center = center;
    this.radius = radius;
  }

  hit(r, minT, maxT) {
    const oc = r.origin.sub(this.center);
    const a = r.direction.dot(r.direction);
    const b = oc.dot(r.direction);
    const c = oc.dot(oc) - this.radius * this.radius;
    const delta = b * b - a * c;
    if (delta > 0) {
      let t = (-b - Math.sqrt(delta)) / a;
      if (t < maxT && t > minT) {
        const p = r.pointAt(t);
        return { t: t, p: p, normal: p.sub(this.center).divs(this.radius) };
      }
      t = (-b + Math.sqrt(delta)) / a;
      if (t < maxT && t > minT) {
        const p = r.pointAt(t);
        return { t: t, p: p, normal: p.sub(this.center).divs(this.radius) };
      }
    }
    return null;
  }
}

class HitList extends Array {
  hit(r, minT, maxT) {
    let bestHit = null;
    let closest = maxT;
    for (let i = 0; i < this.length; ++i) {
      const h = this[i].hit(r, minT, closest);
      if (h) {
        bestHit = h;
        closest = h.t;
      }
    }
    return bestHit;
  }
}

export {Vec3, Ray, Sphere, HitList};
