
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

  get len() {
    return Math.sqrt(this.sqlen);
  }

  get sqlen() {
    return this[0] * this[0] + this[1] * this[1] + this[2] * this[2];
  }
}

const v3 = {};

v3.new = (x, y, z) => new Vec3(x, y, z);

v3.add = (a, b) => {
  const [ax, ay, az] = a.length ? a : [a, a, a];
  const [bx, by, bz] = b.length ? b : [b, b, b];
  return new Vec3(ax + bx, ay + by, az + bz);
};

v3.sub = (a, b) => {
  const [ax, ay, az] = a.length ? a : [a, a, a];
  const [bx, by, bz] = b.length ? b : [b, b, b];
  return new Vec3(ax - bx, ay - by, az - bz);
};

v3.mul = (a, b) => {
  const [ax, ay, az] = a.length ? a : [a, a, a];
  const [bx, by, bz] = b.length ? b : [b, b, b];
  return new Vec3(ax * bx, ay * by, az * bz);
};

v3.div = (a, b) => {
  const [ax, ay, az] = a.length ? a : [a, a, a];
  const [bx, by, bz] = b.length ? b : [b, b, b];
  return new Vec3(ax / bx, ay / by, az / bz);
};

v3.unit = v => v3.div(v, v.len);

v3.dot = (a, b) => a[0] * b[0] + a[1] * b[1] + a[2] * b[2];

v3.cross = (a, b) => new Vec3(
  a[1] * b[2] - a[2] * b[1],
  a[2] * b[0] - a[0] * b[2],
  a[0] * b[1] - a[1] * b[0]);

v3.reflect = (v, n) => v3.sub(v, v3.mul(n, v3.dot(v, n) * 2));

v3.refract = (v, normal, nint) => {
  const uv = v3.unit(v);
  const dt = v3.dot(uv, normal);
  const discr = 1.0 - nint * nint * (1 - dt * dt);
  if (discr <= 0) return null;
  return v3.sub(
    v3.mul(
      v3.sub(
        uv,
        v3.mul(normal, dt)),
      nint),
    v3.mul(
      normal,
      Math.sqrt(discr)));
};

class Ray {
  constructor(a, b) {
    this.a = a;
    this.b = b;
  }
  get origin() { return this.a; }
  get direction() { return this.b; }

  pointAt(t) { return v3.add(this.a, v3.mul(this.b, t)); }
}

function randomInUnitSphere() {
  const p = v3.mul(
    v3.unit(v3.new(Math.random(), Math.random(), Math.random())),
    Math.cbrt(Math.random()));
  return v3.sub(v3.mul(p, 2), 1);
}

function randomInUnitDisk() {
  const p = v3.mul(
    v3.unit(v3.new(Math.random(), Math.random(), 0)),
    Math.sqrt(Math.random()));
  return v3.new(2 * p.x - 1, 2 * p.y - 1, 0);
}

export {v3, Ray, randomInUnitSphere, randomInUnitDisk};
