#include <cstdio>
#include <cstdlib>

#include "function.h"

int binarySearchForZero(Function<int, int> * f, int low, int high) {
  /*if (f->invoke(high - 1) < 0) {
    return high - 1;
  }
  else if (f->invoke(low) > 0) {
    return low;
    }*/
  if (low + 1 == high) {
    return low;
  }
  while (low < high - 1) {
    int mid = (low + high) / 2;
    int srch_pt = f->invoke(mid);
    if (srch_pt == 0) {
      return mid;
    }
    else if (srch_pt > 0) {
      high = mid;
    }
    else {
      low = mid;
    }
  }
  return low;
}
