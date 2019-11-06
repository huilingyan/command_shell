#include <cstdio>
#include <cstdlib>

#include "function.h"

int binarySearchForZero(Function<int, int> * f, int low, int high) {
  if (f->invoke(high - 1) <= 0) {
    return high - 1;
  }
  else if (f->invoke(low) >= 0) {
    return low;
  }
  else {
    while (low < high) {
      int mid = (low + high) / 2;
      if (f->invoke(mid) == 0) {
        return mid;
      }
      else if (f->invoke(mid) > 0) {
        high = mid - 1;
      }
      else {
        low = mid + 1;
      }
    }
    return low;
  }
}
