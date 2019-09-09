#include "test.h"

#include "core/skirt.h"
#include "loader/Loader.h"

using namespace skirt;

class LoaderTest : public ::testing::Test {
 public:
  void LoadScene(const string& scene) {
    LoadSceneString(scene);
    state = GetLoaderStateForTesting();
  }

  unique_ptr<skirt::LoaderState> state;
};

TEST_F(LoaderTest, LookAt) {
  LoadScene(R"""(
LookAt:
  from: [1, 2, 3]
  to: [4, 5, 6]
)""");

  EXPECT_EQ(state->lookAtFrom, Vector3(1, 2, 3));
  EXPECT_EQ(state->lookAtTo, Vector3(4, 5, 6));
}
