
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

v3.min = (a, b) => v3.new(Math.min(a[0], b[0]),
  Math.min(a[1], b[1]), Math.min(a[2], b[2]));

v3.max = (a, b) => v3.new(Math.max(a[0], b[0]),
  Math.max(a[1], b[1]), Math.max(a[2], b[2]));

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
  const r = Math.sqrt(Math.random());
  const t = Math.random() * 2 * Math.PI;
  return v3.new(r * Math.cos(t), r * Math.sin(t));
}

class Perlin {
  constructor(source) {
    this.source = source;
    this.ranvec = this._generatePerlin();
    this.permX = this._generatePerm();
    this.permY = this._generatePerm();
    this.permZ = this._generatePerm();
  }

  noise(p) {
    const u = p.x - Math.floor(p.x);
    const v = p.y - Math.floor(p.y);
    const w = p.z - Math.floor(p.z);
    const i = Math.floor(p.x);
    const j = Math.floor(p.y);
    const k = Math.floor(p.z);
    const mat = [];

    for (let dk = 0; dk < 2; ++dk) {
      for (let dj = 0; dj < 2; ++dj) {
        for (let di = 0; di < 2; ++di) {
          mat.push(
            this.ranvec[this.permX[(i + di) & 255] ^
              this.permY[(j + dj) & 255] ^
              this.permZ[(k + dk) & 255]]);
        }
      }
    }

    return this._interpTrilinear(mat, u, v, w);
  }

  turb(p, depth = 7) {
    let accum = 0;
    let temp = p;
    let weight = 1.0;
    for (let i = 0; i < depth; ++i) {
      accum += weight * this.noise(temp);
      weight *= 0.5;
      temp = v3.mul(2, temp);
    }
    return Math.abs(accum);
  }

  _interpTrilinear(mat, u, v, w) {
    const uu = u * u * (3 - 2 * u);
    const vv = v * v * (3 - 2 * v);
    const ww = w * w * (3 - 2 * w);
    let ret = 0;
    for (let i = 0; i < 2; ++i) {
      for (let j = 0; j < 2; ++j) {
        for (let k = 0; k < 2; ++k) {
          const weightv = v3.new(u - i, v - j, w - k);
          ret += (i * uu + (1 - i) * (1 - uu)) *
                 (j * vv + (1 - j) * (1 - vv)) *
                 (k * ww + (1 - k) * (1 - ww)) *
                 v3.dot(mat[i + j * 2 + k * 4], weightv);
        }
      }
    }
    return ret;
  }

  _generatePerlin() {
    const ret = [];
    for (let i = 0; i < 256; ++i) {
      ret.push(v3.unit(v3.new(
        -1 + 2 * this.source(),
        -1 + 2 * this.source(),
        -1 + 2 * this.source())));
    }
    return ret;
  }

  _permute(list) {
    for (let i = list.length - 1; i >= 0; --i) {
      const target = Math.floor(this.source() * (i + 1));
      const tmp = list[i];
      list[i] = list[target];
      list[target] = tmp;
    }
  }

  _generatePerm() {
    const ret = [];
    for (let i = 0; i < 256; ++i) ret.push(i);
    this._permute(ret);
    return ret;
  }
}

export {Perlin, v3, Ray, randomInUnitSphere, randomInUnitDisk};
