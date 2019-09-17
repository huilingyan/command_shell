#include <stdio.h>
#include <stdlib.h>

size_t max(size_t x, size_t y) {
  if (x >= y) {
    return x;
  }
  else
    return y;
}

size_t maxSeq(int * array, size_t n) {
  size_t result = 0;
  size_t i = 0, j = 0;
  if (n == 0) {
    return 0;
  }
  for (i = 0; i < n - 1; i++) {
    if (array[i + 1] <= array[i]) {
      if ((i - j) > result) {
        result = i - j + 1;
      }
      j = i + 1;
    }
  }
  size_t result_2 = n - j;

  return max(result, result_2);
}
