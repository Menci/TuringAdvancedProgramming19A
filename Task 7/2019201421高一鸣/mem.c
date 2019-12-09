#include "mem.h"

#include <stdio.h>
#include <stdlib.h>
void *myMalloc(size_t size) {
  cntnode++;
  return malloc(size);
}
void myFree(void *size) {
  cntnode--;
  free(size);
}