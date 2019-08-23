
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
      this[2] * o[0] - this[0] * o[2],
      this[0] * o[1] - this[1] * o[0]);
  }

  get len() {
    return Math.sqrt(this.sqlen);
  }

  get sqlen() {
    return this[0] * this[0] + this[1] * this[1] + this[2] * this[2];
  }

  unit() { return this.divs(this.len); }

  reflect(normal) {
    return this.sub(normal.muls(this.dot(normal) * 2));
  }

  refract(normal, niNt) {
    const uv = this.unit();
    const dt = uv.dot(normal);
    const discriminant = 1.0 - niNt * niNt * (1 - dt * dt);
    if (discriminant <= 0) return null;
    return uv.sub(normal.muls(dt)).muls(niNt)
      .sub(normal.muls(Math.sqrt(discriminant)));
  }
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

function randomInUnitDisk() {
  let p;
  do {
    p = new Vec3(
      2 * Math.random() - 1,
      2 * Math.random() - 1,
      0);
  } while (p.sqlen >= 1.0);
  return p;
}

export {Vec3, Ray, randomInUnitSphere, randomInUnitDisk};
