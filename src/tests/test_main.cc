
#include <cstdlib>

#include <glog/logging.h>
#include <gtest/gtest.h>
#include <rapidcheck.h>

#include "core/StringPrintf.h"

using namespace skirt;

int main(int argc, char** argv) {
  FLAGS_logtostderr = 1;
  FLAGS_v = 3;
  google::InitGoogleLogging(argv[0]);
  setenv("RC_PARAMS",
         StringPrintf("max_success=1000 %s", getenv("RC_PARAMS")).c_str(),
         1);
  rc::detail::configuration();
  FLAGS_logtostderr = 1;
  FLAGS_v = 3;
  ::testing::InitGoogleTest(&argc, argv);

  // rc::detail::configFromString("max_success=1000");

  return RUN_ALL_TESTS();
  return 0;
}
