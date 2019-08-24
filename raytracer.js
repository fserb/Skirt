import {v3, Ray,
  randomInUnitSphere, randomInUnitDisk} from "./utils.js";

class AABB {
  constructor(a, b) { this.min = a; this.max = b; }

  hit(r, tmin, tmax) {
    for (let a = 0; a < 3; ++a) {
      const invD = 1.0 / r.direction[a];
      let t0 = (this.min[a] - r.origin[a]) * invD;
      let t1 = (this.max[a] - r.origin[a]) * invD;
      if (invD < 0) { const tmp = t0; t0 = t1; t1 = tmp; }
      tmin = t0 > tmin ? t0 : tmin;
      tmax = t1 < tmax ? t1 : tmax;
      if (tmax <= tmin) return false;
    }
    return true;
  }
}

class Camera {
  constructor(lookfrom, lookat, vup, vfov, aspect, aperture, focusDist) {
    this.lensRadius = aperture / 2.0;
    const theta = vfov * Math.PI / 180;
    const halfHeight = Math.tan(theta / 2);
    const halfWidth = aspect * halfHeight;

    this.origin = lookfrom;
    this.w = v3.unit(v3.sub(lookfrom, lookat));
    this.u = v3.unit(v3.cross(vup, this.w));
    this.v = v3.cross(this.w, this.u);

    this.lowerLeftCorner =
      v3.sub(this.origin,
        v3.add(v3.add(
          v3.mul(this.u, halfWidth * focusDist),
          v3.mul(this.v, halfHeight * focusDist)),
        v3.mul(this.w, focusDist)));

    this.horizontal = v3.mul(this.u, 2 * halfWidth * focusDist);
    this.vertical = v3.mul(this.v, 2 * halfHeight * focusDist);
  }

  getRay(u, v) {
    const rd = v3.mul(randomInUnitDisk(), this.lensRadius);
    const offset = v3.add(v3.mul(this.u, rd.x), v3.mul(this.v, rd.y));

    return new Ray(v3.add(this.origin, offset),
      v3.sub(
        v3.add(this.lowerLeftCorner,
          v3.add(
            v3.mul(this.horizontal, u),
            v3.mul(this.vertical, v))),
        v3.add(this.origin, offset)));
  }
}

// texture: value(u, v, p) -> vec3

class ConstantTexture {
  constructor(r, g, b) { this.color = v3.new(r, g, b); }
  value(_u, _v, _p) {
    return this.color;
  }
}

class CheckerTexture {
  constructor(a, b) { this.even = a; this.odd = b; }
  value(u, v, p) {
    const sines = Math.sin(10 * p.x) * Math.sin(10 * p.y) * Math.sin(10 * p.z);
    if (sines < 0) {
      return this.odd.value(u, v, p);
    } else {
      return this.even.value(u, v, p);
    }
  }
}

// material: scatter(r, {t, p, normal}, attenuation) -> scattered

class Lambertian {
  constructor(albedo) {
    this.albedo = albedo;
  }

  scatter(ray, record) {
    const target = v3.add(
      v3.add(record.p, record.normal),
      randomInUnitSphere());
    return {
      scattered: new Ray(record.p, v3.sub(target, record.p)),
      attenuation: this.albedo.value(0, 0, record.p) };
  }
}

class Metal {
  constructor(albedo, fuzz) {
    this.albedo = albedo;
    this.fuzz = Math.min(1, fuzz);
  }

  scatter(ray, record) {
    const reflected = v3.reflect(v3.unit(ray.direction), record.normal);

    const scattered = new Ray(record.p,
      v3.add(reflected, v3.mul(randomInUnitSphere(), this.fuzz)));

    if (v3.dot(scattered.direction, record.normal) > 0) {
      return { scattered: scattered,
        attenuation: this.albedo.value(0, 0, record.p) };
    } else {
      return null;
    }
  }
}

class Dielectric {
  constructor(ri) {
    this.ri = ri;
  }

  schlick(cosine, refIdx) {
    let r0 = (1 - refIdx) / (1 + refIdx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * Math.pow((1 - cosine), 5);
  }

  scatter(ray, record) {
    const reflected = v3.reflect(ray.direction, record.normal);
    let outNormal, niNt, cosine;
    if (v3.dot(ray.direction, record.normal) > 0) {
      outNormal = v3.mul(-1, record.normal);
      niNt = this.ri;
      cosine = this.ri *
        v3.dot(ray.direction, record.normal) / ray.direction.len;
    } else {
      outNormal = record.normal;
      niNt = 1.0 / this.ri;
      cosine = - this.ri *
        v3.dot(ray.direction, record.normal) / ray.direction.len;
    }
    const refracted = v3.refract(ray.direction, outNormal, niNt);
    const reflectProb = refracted ? this.schlick(cosine, this.ri) : 1.0;

    if (Math.random() < reflectProb) {
      return { scattered: new Ray(record.p, reflected),
        attenuation: v3.new(1, 1, 1) };
    }

    return { scattered: new Ray(record.p, refracted),
      attenuation: v3.new(1, 1, 1) };
  }
}

// hitable:
//   hit(r, minT, maxT) -> {t, p, normal, material}
//   bbox() -> AABB

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
        return { t: t, p: p,
          normal: v3.div(v3.sub(p, this.center), this.radius),
          material: this.material };
      }
      t = (-b + Math.sqrt(delta)) / a;
      if (t < maxT && t > minT) {
        const p = r.pointAt(t);
        return { t: t, p: p,
          normal: v3.div(v3.sub(p, this.center), this.radius),
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

  bbox() {
    if (this.length == 0) return null;
    let ret = null;
    for (let i = 0; i < this.length; ++i) {
      const a = this[i].bbox();
      if (a == null) continue;
      if (ret == null) { ret = a; continue; }
      ret = new AABB(v3.min(a.min, ret.min), v3.max(a.max, ret.max));
    }
    return ret;
  }
}

class BVH {
  constructor(objs) {
    const axis = Math.floor(3 * Math.random());
    objs.sort((a, b) => {
      const bba = a.bbox();
      const bbb = b.bbox();
      return bba.min[axis] - bbb.min[axis];
    });
    if (objs.length == 1) {
      this.left = this.right = objs[0];
    } else if (objs.length == 2) {
      this.left = objs[0];
      this.right = objs[1];
    } else {
      this.left = new BVH(objs.slice(0, objs.length / 2));
      this.right = new BVH(objs.slice(objs.length / 2));
    }

    const bbleft = this.left.bbox();
    const bbright = this.right.bbox();

    this.box = new AABB(v3.min(bbleft.min, bbright.min),
      v3.max(bbleft.max, bbright.max));
  }

  hit(r, tmin, tmax) {
    if (this.box.hit(r, tmin, tmax)) {
      const leftRec = this.left.hit(r, tmin, tmax);
      const rightRec = this.right.hit(r, tmin, tmax);
      if (leftRec && rightRec) {
        return (leftRec.t < rightRec.t) ? leftRec : rightRec;
      } else if (leftRec) {
        return leftRec;
      } else if (rightRec) {
        return rightRec;
      }
    }
    return null;
  }

  bbox() {
    return this.box;
  }
}

export {BVH, Sphere, HitList, Camera, Lambertian, Metal, Dielectric,
  ConstantTexture, CheckerTexture};
