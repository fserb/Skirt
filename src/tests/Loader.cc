#include "test.h"

#include "core/skirt.h"
#include "loader/Loader.h"

using namespace skirt;

class LoaderTest : public ::testing::Test {
 public:
  void LoadScene(const string& data) {
    scene = LoadSceneString(data);
    desc = scene->desc.get();
  }

  unique_ptr<skirt::Scene> scene;
  skirt::Description* desc;
};

TEST_F(LoaderTest, LookAt) {
  LoadScene(R"""(
LookAt:
  from: [1, 2, 3]
  to: [4, 5, 6]
)""");

  EXPECT_EQ(desc->lookAtFrom, Vector3(1, 2, 3));
  EXPECT_EQ(desc->lookAtTo, Vector3(4, 5, 6));
}

TEST_F(LoaderTest, Camera) {
  LoadScene(R"""(
Camera.perspective:
  fov: 20.0
  aperture: 2.0
  focusDistance: 5.196152422706632
)""");

  EXPECT_EQ(desc->cameraType, "perspective");
  EXPECT_FLOAT_EQ(desc->cameraFOV, 20.0f);
  EXPECT_FLOAT_EQ(desc->cameraAperture, 2.0f);
  EXPECT_FLOAT_EQ(desc->cameraFocusDistance, 5.196152422706632);
}

TEST_F(LoaderTest, Integrator) {
  LoadScene(R"""(
Integrator.sampler:
)""");

  EXPECT_EQ(desc->integratorType, "sampler");
}

TEST_F(LoaderTest, Film) {
  LoadScene(R"""(
Film.image:
  filename: "out.pmf"
  resolution: [123, 456]
)""");

  EXPECT_EQ(desc->filmType, "image");
  EXPECT_EQ(desc->filmFilename, "out.pmf");
  EXPECT_EQ(desc->width, 123);
  EXPECT_EQ(desc->height, 456);
}
