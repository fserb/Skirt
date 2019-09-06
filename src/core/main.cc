// main

#include <stdio.h>
#include "skirt.h"

void f() {
  DCHECK(1 == 0);
}

int main(int argc, char **argv) {
  google::InitGoogleLogging(argv[0]);
  printf("skirt\n");
  f();
  return 0;
}
