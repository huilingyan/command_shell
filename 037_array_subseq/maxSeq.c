#include <stdio.h>
#include <stdlib.h>

size_t maxSeq(int * array, size_t n) {
  size_t result = 0;
  size_t i = 1, j = 0;
  if (n == 0) {
    return 0;
  }
  for (i = 1; i < n; i++) {
    if (array[i] <= array[i - 1]) {
      if ((i - j) > result) {
        result = i - j + 1;
      }
      j = i;
    }
  }

  return result;
}
