// main

#include <stdio.h>
#include <glog/logging.h>


#include "skirt.h"

void f() {
  DCHECK(0 == 0);
  VLOG(google::INFO) << "Hello";
}

int main(int argc, char **argv) {
  FLAGS_alsologtostderr = 1; // It will dump to console
  google::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);

  printf("skirt\n");
  f();
  return 0;
}
