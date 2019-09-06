// main

#include <stdio.h>

#include "skirt.h"

#include "vec3.h"

void f() {
  DCHECK(0 == 0);
  VLOG(google::INFO) << "Hello";
}

int main(int argc, char **argv) {
  FLAGS_logtostderr = 1;
  google::InitGoogleLogging(argv[0]);

  printf("skirt\n");
  f();
  return 0;
}
