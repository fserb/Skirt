#pragma once

#include "core/skirt.h"

namespace skirt {

#ifdef DEBUG
#define ONDEBUG if (true)
#else
#define ONDEBUG if (false)
#endif

class EFloat {
 public:
  EFloat() : EFloat(0, 0.0f) {}
  EFloat(float v, float err = 0.0f) : v(v) {
    if (err == 0.) {
      low = high = v;
    } else {
      low = FloatDown(v - err);
      high = FloatUp(v + err);
    }
    ONDEBUG precise = v;
    ONDEBUG Check();
  }
  EFloat(const EFloat& o) {
    ONDEBUG o.Check();
    v = o.v;
    low = o.low;
    high = o.high;
    ONDEBUG precise = o.precise;
  }

  INLINE void Check() const {
    if (!std::isinf(low) && !std::isnan(low) && !std::isinf(high) &&
        !std::isnan(high))
      CHECK_LE(low, high);

    ONDEBUG {
      if (!std::isinf(v) && !std::isnan(v)) {
        CHECK_LE(LowerBound(), precise);
        CHECK_LE(precise, UpperBound());
      }
    }
  }

  INLINE explicit operator float() const {
    return v;
  }
  INLINE explicit operator double() const {
    return v;
  }
  INLINE float GetAbsoluteError() const {
    return high - low;
  }
  INLINE float UpperBound() const {
    return high;
  }
  INLINE float LowerBound() const {
    return low;
  }
#ifdef DEBUG
  INLINE float GetRelativeError() const {
    return abs((precise - v) / precise);
  }
  long double PreciseValue() const {
    return precise;
  }
#endif

  EFloat operator+(EFloat o) const {
    EFloat r;
    r.v = v + o.v;
    ONDEBUG r.precise = precise + o.precise;
    r.low = FloatDown(low + o.low);
    r.high = FloatUp(high + o.high);
    ONDEBUG r.Check();
    return r;
  }

  EFloat operator-(EFloat o) const {
    EFloat r;
    r.v = v - o.v;
    ONDEBUG r.precise = precise - o.precise;
    r.low = FloatDown(low - o.low);
    r.high = FloatUp(high - o.high);
    ONDEBUG r.Check();
    return r;
  }

  EFloat operator*(EFloat o) const {
    EFloat r;
    r.v = v * o.v;
    ONDEBUG r.precise = precise * o.precise;

    float p[4] = {low * o.low, high * o.low, low * o.high, high * o.high};

    r.low = FloatDown(min(min(p[0], p[1]), min(p[2], p[3])));
    r.low = FloatUp(max(max(p[0], p[1]), max(p[2], p[3])));
    ONDEBUG r.Check();
    return r;
  }

  EFloat operator/(EFloat o) const {
    EFloat r;
    r.v = v / o.v;
    ONDEBUG r.precise = precise / o.precise;

    if (o.low < 0 && o.high > 0) {
      r.low = -Infinity;
      r.high = Infinity;
    } else {
      float p[4] = {low / o.low, high / o.low, low / o.high, high / o.high};

      r.low = FloatDown(min(min(p[0], p[1]), min(p[2], p[3])));
      r.low = FloatUp(max(max(p[0], p[1]), max(p[2], p[3])));
    }
    ONDEBUG r.Check();
    return r;
  }

  EFloat operator-() const {
    EFloat r;
    r.v = -v;
    ONDEBUG r.precise = -precise;
    r.low = -high;
    r.high = -low;
    ONDEBUG r.Check();
    return r;
  }

  INLINE bool operator==(EFloat o) const {
    return v == o.v;
  }

  INLINE EFloat& operator=(const EFloat& o) {
    ONDEBUG o.Check();
    if (&o != this) {
      v = o.v;
      low = o.low;
      high = o.high;
      ONDEBUG precise = o.precise;
    }
    return *this;
  }

  friend std::ostream& operator<<(std::ostream& os, const EFloat& v) {
    os << StringPrintf("EFloat %f (%a) - [%f, %f]", v.v, v.v, v.low, v.high);
    ONDEBUG os << StringPrintf(", p=%.30Lf", v.precise);
    return os;
  }

  friend EFloat sqrt(EFloat o);
  friend EFloat abs(EFloat o);

 private:
  float v, low, high;
#ifdef DEBUG
  long double precise;
#endif
};

INLINE EFloat operator*(float f, EFloat fe) {
  return EFloat(f) * fe;
}

INLINE EFloat operator/(float f, EFloat fe) {
  return EFloat(f) / fe;
}

INLINE EFloat operator+(float f, EFloat fe) {
  return EFloat(f) + fe;
}

INLINE EFloat operator-(float f, EFloat fe) {
  return EFloat(f) - fe;
}

INLINE EFloat sqrt(EFloat fe) {
  EFloat r;
  r.v = std::sqrt(fe.v);
  ONDEBUG r.precise = std::sqrt(fe.precise);
  r.low = FloatDown(std::sqrt(fe.low));
  r.high = FloatUp(std::sqrt(fe.high));
  ONDEBUG r.Check();
  return r;
}

INLINE EFloat abs(EFloat fe) {
  if (fe.low >= 0) {
    return fe;
  } else if (fe.high <= 0) {
    EFloat r;
    r.v = -fe.v;
    ONDEBUG r.precise = -fe.precise;
    r.low = -fe.high;
    r.high = -fe.low;
    ONDEBUG r.Check();
    return r;
  } else {
    EFloat r;
    r.v = std::abs(fe.v);
    ONDEBUG r.precise = std::abs(fe.precise);
    r.low = 0;
    r.high = std::max(-fe.low, fe.high);
    ONDEBUG r.Check();
    return r;
  }
}

}  // namespace skirt
