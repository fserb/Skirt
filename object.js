import {AABB, HitList, Ray} from "./raytracer.js";
import {Isotropic} from "./texture.js";
import {v3, Mat4, getSphereUV} from "./utils.js";

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

class ConstantMedium {
  constructor(boundary, density, texture) {
    this.boundary = boundary;
    this.density = density;
    this.phaseFunction = new Isotropic(texture);
  }

  hit(r, minT, maxT) {
    const rec1 = this.boundary.hit(r, -Infinity, Infinity);
    if (!rec1) return null;
    const rec2 = this.boundary.hit(r, rec1.t + 0.0001, Infinity);
    if (!rec2) return null;

    rec1.t = Math.max(minT, rec1.t);
    rec2.t = Math.min(maxT, rec2.t);
    if (rec1.t >= rec2.t) return null;
    rec1.t = Math.max(0, rec1.t);

    const distInsideBoundary = (rec2.t - rec1.t) * r.direction.len;
    const hitDistance = -(1 / this.density) * Math.log(Math.random());
    if (hitDistance >= distInsideBoundary) return null;

    const t = rec1.t + hitDistance / r.direction.len;
    return {
      t: t,
      p: r.pointAt(t),
      normal: v3.new(1, 0, 0),
      material: this.phaseFunction
    };
  }

  bbox() {
    return this.boundary.bbox();
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

class Box {
  constructor(p0, p1, material) {
    this.pmin = p0;
    this.pmax = p1;

    this.list = new HitList();
    this.list.push(new RectXY(p0.x, p0.y, p1.x, p1.y, p1.z, material));
    this.list.push(new FlipNormal(
      new RectXY(p0.x, p0.y, p1.x, p1.y, p0.z, material)));

    this.list.push(new RectXZ(p0.x, p0.z, p1.x, p1.z, p1.y, material));
    this.list.push(new FlipNormal(
      new RectXZ(p0.x, p0.z, p1.x, p1.z, p0.y, material)));

    this.list.push(new RectYZ(p0.y, p0.z, p1.y, p1.z, p1.x, material));
    this.list.push(new FlipNormal(
      new RectYZ(p0.y, p0.z, p1.y, p1.z, p0.x, material)));
  }

  hit(r, minT, maxT) {
    return this.list.hit(r, minT, maxT);
  }

  bbox() {
    return new AABB(this.pmin, this.pmax);
  }
}

class Transform {
  constructor(translate, rotate, scale, obj) {
    this.transform = new Mat4();
    this.transform.translate(translate);
    this.transform.rotateX(rotate.x);
    this.transform.rotateY(rotate.y);
    this.transform.rotateZ(rotate.z);
    this.transform.scale(scale);
    this.invTransform = new Mat4();
    this.invTransform.scale(v3.div(1, scale));
    this.invTransform.rotateZ(-rotate.z);
    this.invTransform.rotateY(-rotate.y);
    this.invTransform.rotateX(-rotate.x);
    this.invTransform.translate(v3.mul(-1, translate));
    this.obj = obj;

    // calculate bbox
    const orig = obj.bbox();

    const points = [
      v3.new(orig.min.x, orig.min.y, orig.min.z),
      v3.new(orig.min.x, orig.min.y, orig.max.z),
      v3.new(orig.min.x, orig.max.y, orig.min.z),
      v3.new(orig.min.x, orig.max.y, orig.max.z),
      v3.new(orig.max.x, orig.min.y, orig.min.z),
      v3.new(orig.max.x, orig.min.y, orig.max.z),
      v3.new(orig.max.x, orig.max.y, orig.min.z),
      v3.new(orig.max.x, orig.max.y, orig.max.z)];

    const min = v3.new(Infinity, Infinity, Infinity);
    const max = v3.new(-Infinity, -Infinity, -Infinity);
    for (let i = 0; i < 8; ++i) {
      const p = v3.transform(points[i], this.transform);
      for (let j = 0; j < 3; ++j) {
        if (p[j] < min[j]) min[j] = p[j];
        if (p[j] > max[j]) max[j] = p[j];
      }
    }

    this.bbox_ = new AABB(min, max);
  }

  bbox() {
    return this.bbox_;
  }

  hit(r, minT, maxT) {
    const tr = new Ray(v3.transform(r.origin, this.invTransform),
      v3.transformNormal(r.direction, this.invTransform));
    const h = this.obj.hit(tr, minT, maxT);
    if (!h) return null;
    h.p = v3.transform(h.p, this.transform);
    h.normal = v3.unit(v3.transformNormal(h.normal, this.transform));
    return h;
  }
}

export {FlipNormal, RectXY, RectXZ, RectYZ, Box, Sphere,
  ConstantMedium, Transform};
