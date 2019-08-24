import {v3, Ray, Perlin,
  randomInUnitSphere} from "./utils.js";

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

class NoiseTexture {
  constructor(source, scale) {
    this.perlin = new Perlin(source);
    this.scale = scale;
  }

  value(u, v, p) {
    // return v3.mul(v3.new(1, 1, 1), 0.5 * (1 + this.perlin.noise(v3.mul(this.scale, p))));
    // return v3.mul(v3.new(1, 1, 1), this.perlin.turb(v3.mul(this.scale, p)));
    return v3.mul(v3.new(1, 1, 1),
      0.5 * (1 + Math.sin(this.scale * p.z + 10 * this.perlin.turb(p))));
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

export {Lambertian, Metal, Dielectric,
  ConstantTexture, CheckerTexture, NoiseTexture};
