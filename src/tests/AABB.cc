#include "test.h"

#include "core/AABB.h"

#include "core/skirt.h"

using namespace skirt;

TEST(AABB, PointDistance) {
  {
    AABB b(Vector3(0, 0, 0), Vector3(1, 1, 1));

    // Points inside the bounding box or on faces
    EXPECT_EQ(0., Distance(b, Vector3(.5, .5, .5)));
    EXPECT_EQ(0., Distance(b, Vector3(0, 1, 1)));
    EXPECT_EQ(0., Distance(b, Vector3(.25, .8, 1)));
    EXPECT_EQ(0., Distance(b, Vector3(0, .25, .8)));
    EXPECT_EQ(0., Distance(b, Vector3(.7, 0, .8)));

    // Aligned with the plane of one of the faces
    EXPECT_EQ(5., Distance(b, Vector3(6, 1, 1)));
    EXPECT_EQ(10., Distance(b, Vector3(0, -10, 1)));

    // 2 of the dimensions inside the box's extent
    EXPECT_EQ(2., Distance(b, Vector3(0.5, 0.5, 3)));
    EXPECT_EQ(3., Distance(b, Vector3(0.5, 0.5, -3)));
    EXPECT_EQ(2., Distance(b, Vector3(0.5, 3, 0.5)));
    EXPECT_EQ(3., Distance(b, Vector3(0.5, -3, 0.5)));
    EXPECT_EQ(2., Distance(b, Vector3(3, 0.5, 0.5)));
    EXPECT_EQ(3., Distance(b, Vector3(-3, 0.5, 0.5)));

    // General points
    EXPECT_EQ(3 * 3 + 7 * 7 + 10 * 10, DistanceSq(b, Vector3(4, 8, -10)));
    EXPECT_EQ(6 * 6 + 10 * 10 + 7 * 7, DistanceSq(b, Vector3(-6, -10, 8)));
  }

  {
    // A few with a more irregular box, just to be sure
    AABB b(Vector3(-1, -3, 5), Vector3(2, -2, 18));
    EXPECT_EQ(0., Distance(b, Vector3(-.99, -2, 5)));
    EXPECT_EQ(2 * 2 + 6 * 6 + 4 * 4, DistanceSq(b, Vector3(-3, -9, 22)));
  }
}

TEST(AABB, Union) {
  AABB a(Vector3(-10, -10, 5), Vector3(0, 20, 10));
  AABB b;  // degenerate
  AABB c = Union(a, b);
  EXPECT_EQ(a, c);

  EXPECT_EQ(b, Union(b, b));

  AABB d(Vector3(-15, 10, 30));
  AABB e = Union(a, d);
  EXPECT_EQ(AABB(Vector3(-15, -10, 5), Vector3(0, 20, 30)), e);
}
