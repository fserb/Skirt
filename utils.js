
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

v3.transform = (v, mat) => {
  const w = (mat[3] * v.x + mat[7] * v.y + mat[11] * v.z + mat[15]) || 1.0;
  return v3.new(
    (mat[0] * v.x + mat[4] * v.y + mat[8] * v.z + mat[12]) / w,
    (mat[1] * v.x + mat[5] * v.y + mat[9] * v.z + mat[13]) / w,
    (mat[2] * v.x + mat[6] * v.y + mat[10] * v.z + mat[14]) / w);
};

v3.transformNormal = (n, mat) => {
  return v3.new(
    mat[0] * n.x + mat[4] * n.y + mat[8] * n.z,
    mat[1] * n.x + mat[5] * n.y + mat[9] * n.z,
    mat[2] * n.x + mat[6] * n.y + mat[10] * n.z);
};

class Mat4 extends Float32Array {
  constructor() {
    super([
      1, 0, 0, 0,
      0, 1, 0, 0,
      0, 0, 1, 0,
      0, 0, 0, 1]);
  }

  scale(v) {
    this[0] *= v.x;
    this[1] *= v.x;
    this[2] *= v.x;
    this[3] *= v.x;
    this[4] *= v.y;
    this[5] *= v.y;
    this[6] *= v.y;
    this[7] *= v.y;
    this[8] *= v.z;
    this[9] *= v.z;
    this[10] *= v.z;
    this[11] *= v.z;
    return this;
  }

  translate(v) {
    this[12] = this[0] * v.x + this[4] * v.y + this[8] * v.z + this[12];
    this[13] = this[1] * v.x + this[5] * v.y + this[9] * v.z + this[13];
    this[14] = this[2] * v.x + this[6] * v.y + this[10] * v.z + this[14];
    this[15] = this[3] * v.x + this[7] * v.y + this[11] * v.z + this[15];
    return this;
  }

  rotate(v, rad) {
    v = v3.unit(v);
    const s = Math.sin(rad);
    const c = Math.cos(rad);
    const t = 1 - c;

    const a00 = this[0], a01 = this[1], a02 = this[2], a03 = this[3],
      a10 = this[4], a11 = this[5], a12 = this[6], a13 = this[7],
      a20 = this[8], a21 = this[9], a22 = this[10], a23 = this[11];

    const b00 = v.x * v.x * t + c, b01 = v.y * v.x * t + v.z * s,
      b02 = v.z * v.x * t - v.y * s, b10 = v.x * v.y * t - v.z * s,
      b11 = v.y * v.y * t + c, b12 = v.z * v.y * t + v.x * s,
      b20 = v.x * v.z * t + v.y * s, b21 = v.y * v.z * t - v.x * s,
      b22 = v.z * v.z * t + c;

    this[0] = a00 * b00 + a10 * b01 + a20 * b02;
    this[1] = a01 * b00 + a11 * b01 + a21 * b02;
    this[2] = a02 * b00 + a12 * b01 + a22 * b02;
    this[3] = a03 * b00 + a13 * b01 + a23 * b02;
    this[4] = a00 * b10 + a10 * b11 + a20 * b12;
    this[5] = a01 * b10 + a11 * b11 + a21 * b12;
    this[6] = a02 * b10 + a12 * b11 + a22 * b12;
    this[7] = a03 * b10 + a13 * b11 + a23 * b12;
    this[8] = a00 * b20 + a10 * b21 + a20 * b22;
    this[9] = a01 * b20 + a11 * b21 + a21 * b22;
    this[10] = a02 * b20 + a12 * b21 + a22 * b22;
    this[11] = a03 * b20 + a13 * b21 + a23 * b22;
    return this;
  }

  rotateX(rad) {
    const s = Math.sin(rad),
      c = Math.cos(rad),
      a10 = this[4],
      a11 = this[5],
      a12 = this[6],
      a13 = this[7],
      a20 = this[8],
      a21 = this[9],
      a22 = this[10],
      a23 = this[11];
    this[4] = a10 * c + a20 * s;
    this[5] = a11 * c + a21 * s;
    this[6] = a12 * c + a22 * s;
    this[7] = a13 * c + a23 * s;
    this[8] = a20 * c - a10 * s;
    this[9] = a21 * c - a11 * s;
    this[10] = a22 * c - a12 * s;
    this[11] = a23 * c - a13 * s;
    return this;
  }

  rotateY(rad) {
    const s = Math.sin(rad),
      c = Math.cos(rad),
      a00 = this[0],
      a01 = this[1],
      a02 = this[2],
      a03 = this[3],
      a20 = this[8],
      a21 = this[9],
      a22 = this[10],
      a23 = this[11];
    this[0] = a00 * c - a20 * s;
    this[1] = a01 * c - a21 * s;
    this[2] = a02 * c - a22 * s;
    this[3] = a03 * c - a23 * s;
    this[8] = a00 * s + a20 * c;
    this[9] = a01 * s + a21 * c;
    this[10] = a02 * s + a22 * c;
    this[11] = a03 * s + a23 * c;
    return this;
  }

  rotateZ(rad) {
    const s = Math.sin(rad),
      c = Math.cos(rad),
      a00 = this[0],
      a01 = this[1],
      a02 = this[2],
      a03 = this[3],
      a10 = this[4],
      a11 = this[5],
      a12 = this[6],
      a13 = this[7];

    this[0] = a00 * c + a10 * s;
    this[1] = a01 * c + a11 * s;
    this[2] = a02 * c + a12 * s;
    this[3] = a03 * c + a13 * s;
    this[4] = a10 * c - a00 * s;
    this[5] = a11 * c - a01 * s;
    this[6] = a12 * c - a02 * s;
    this[7] = a13 * c - a03 * s;
    return this;
  }

  invert() {
    const n11 = this[0], n21 = this[1], n31 = this[2], n41 = this[3],
      n12 = this[4], n22 = this[5], n32 = this[6], n42 = this[7],
      n13 = this[8], n23 = this[9], n33 = this[10], n43 = this[11],
      n14 = this[12], n24 = this[13], n34 = this[14], n44 = this[15],

      t11 = n23 * n34 * n42 - n24 * n33 * n42 + n24 * n32 * n43 -
            n22 * n34 * n43 - n23 * n32 * n44 + n22 * n33 * n44,
      t12 = n14 * n33 * n42 - n13 * n34 * n42 - n14 * n32 * n43 +
            n12 * n34 * n43 + n13 * n32 * n44 - n12 * n33 * n44,
      t13 = n13 * n24 * n42 - n14 * n23 * n42 + n14 * n22 * n43 -
            n12 * n24 * n43 - n13 * n22 * n44 + n12 * n23 * n44,
      t14 = n14 * n23 * n32 - n13 * n24 * n32 - n14 * n22 * n33 +
            n12 * n24 * n33 + n13 * n22 * n34 - n12 * n23 * n34;

    const det = n11 * t11 + n21 * t12 + n31 * t13 + n41 * t14;

    if (det === 0) {
      // eslint-disable-next-line no-console
      console.error("mix.mat4.invert: can't invert matrix, determinant is 0");
      return null;
    }

    const detInv = 1 / det;
    this[0] = t11 * detInv;
    this[1] = (n24 * n33 * n41 - n23 * n34 * n41 - n24 * n31 * n43 +
               n21 * n34 * n43 + n23 * n31 * n44 - n21 * n33 * n44) * detInv;
    this[2] = (n22 * n34 * n41 - n24 * n32 * n41 + n24 * n31 * n42 -
               n21 * n34 * n42 - n22 * n31 * n44 + n21 * n32 * n44) * detInv;
    this[3] = (n23 * n32 * n41 - n22 * n33 * n41 - n23 * n31 * n42 +
               n21 * n33 * n42 + n22 * n31 * n43 - n21 * n32 * n43) * detInv;

    this[4] = t12 * detInv;
    this[5] = (n13 * n34 * n41 - n14 * n33 * n41 + n14 * n31 * n43 -
               n11 * n34 * n43 - n13 * n31 * n44 + n11 * n33 * n44) * detInv;
    this[6] = (n14 * n32 * n41 - n12 * n34 * n41 - n14 * n31 * n42 +
               n11 * n34 * n42 + n12 * n31 * n44 - n11 * n32 * n44) * detInv;
    this[7] = (n12 * n33 * n41 - n13 * n32 * n41 + n13 * n31 * n42 -
               n11 * n33 * n42 - n12 * n31 * n43 + n11 * n32 * n43) * detInv;

    this[8] = t13 * detInv;
    this[9] = (n14 * n23 * n41 - n13 * n24 * n41 - n14 * n21 * n43 +
               n11 * n24 * n43 + n13 * n21 * n44 - n11 * n23 * n44) * detInv;
    this[10] = (n12 * n24 * n41 - n14 * n22 * n41 + n14 * n21 * n42 -
                n11 * n24 * n42 - n12 * n21 * n44 + n11 * n22 * n44) * detInv;
    this[11] = (n13 * n22 * n41 - n12 * n23 * n41 - n13 * n21 * n42 +
                n11 * n23 * n42 + n12 * n21 * n43 - n11 * n22 * n43) * detInv;

    this[12] = t14 * detInv;
    this[13] = (n13 * n24 * n31 - n14 * n23 * n31 + n14 * n21 * n33 -
                n11 * n24 * n33 - n13 * n21 * n34 + n11 * n23 * n34) * detInv;
    this[14] = (n14 * n22 * n31 - n12 * n24 * n31 - n14 * n21 * n32 +
                n11 * n24 * n32 + n12 * n21 * n34 - n11 * n22 * n34) * detInv;
    this[15] = (n12 * n23 * n31 - n13 * n22 * n31 + n13 * n21 * n32 -
                n11 * n23 * n32 - n12 * n21 * n33 + n11 * n22 * n33) * detInv;

    return this;
  }
}

function getSphereUV(normal) {
  const phi = Math.atan2(normal.z, normal.x);
  const theta = Math.asin(normal.y);
  return [1 - (phi + Math.PI) / (2 * Math.PI),
    (theta + Math.PI / 2) / Math.PI];
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

export {Perlin, Mat4, v3, randomInUnitSphere, randomInUnitDisk, getSphereUV};
