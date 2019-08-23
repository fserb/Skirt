import {Vec3, Ray} from "./utils.js";

class Camera {
  constructor() {
    this.lowerLeftCorner = new Vec3(-2, -1.125, -1);
    this.horizontal = new Vec3(4, 0, 0);
    this.vertical = new Vec3(0, 2.25, 0);
    this.origin = new Vec3(0, 0, 0);
  }

  getRay(u, v) {
    return new Ray(this.origin,
      this.lowerLeftCorner.add(this.horizontal.muls(u))
        .add(this.vertical.muls(v)));
  }
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

export {Sphere, HitList, Camera};
