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

// hitable: hit(r, minT, maxT) -> {t, p, normal, material}
// material: scatter(r, {t, p, normal}, attenuation) -> scattered

function randomInUnitSphere() {
  let p;
  do {
    p = new Vec3(
      2 * Math.random() - 1,
      2 * Math.random() - 1,
      2 * Math.random() - 1);
  } while (p.sqlen >= 1.0);
  return p;
}

class Lambertian {
  constructor(albedo) {
    this.albedo = albedo;
  }

  scatter(ray, record) {
    const target = record.p.add(record.normal).add(randomInUnitSphere());
    return {
      scattered: new Ray(record.p, target.sub(record.p)),
      attenuation: this.albedo};
  }
}

class Metal {
  constructor(albedo, fuzz) {
    this.albedo = albedo;
    this.fuzz = Math.min(1, fuzz);
  }

  scatter(ray, record) {
    const reflected = ray.direction.unit().reflect(record.normal);
    const scattered = new Ray(record.p,
      reflected.add(randomInUnitSphere().muls(this.fuzz)));
    if (scattered.direction.dot(record.normal) > 0) {
      return { scattered: scattered, attenuation: this.albedo };
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
    const reflected = ray.direction.reflect(record.normal);
    let outNormal, niNt, cosine;
    if (ray.direction.dot(record.normal) > 0) {
      outNormal = record.normal.muls(-1);
      niNt = this.ri;
      cosine = this.ri * ray.direction.dot(record.normal) / ray.direction.len;
    } else {
      outNormal = record.normal;
      niNt = 1.0 / this.ri;
      cosine = - this.ri * ray.direction.dot(record.normal) / ray.direction.len;
    }
    const refracted = ray.direction.refract(outNormal, niNt);
    const reflectProb = refracted ? this.schlick(cosine, this.ri) : 1.0;

    if (Math.random() < reflectProb) {
      return { scattered: new Ray(record.p, reflected),
        attenuation: new Vec3(1, 1, 1) };
    }

    return { scattered: new Ray(record.p, refracted),
      attenuation: new Vec3(1, 1, 1) };
  }
}

class Sphere {
  constructor(center, radius, material) {
    this.center = center;
    this.radius = radius;
    this.material = material;
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
        return { t: t, p: p, normal: p.sub(this.center).divs(this.radius),
          material: this.material };
      }
      t = (-b + Math.sqrt(delta)) / a;
      if (t < maxT && t > minT) {
        const p = r.pointAt(t);
        return { t: t, p: p, normal: p.sub(this.center).divs(this.radius),
          material: this.material };
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

export {Sphere, HitList, Camera, Lambertian, Metal, Dielectric};
