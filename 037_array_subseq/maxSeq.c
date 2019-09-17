#include <stdio.h>
#include <stdlib.h>

size_t maxSeq(int * array, size_t n) {
  size_t result = 0;
  size_t i = 0, j = 0;
  if (n <= 0) {
    return 0;
  }
  for (i = 0; i <= n - 1; i++) {
    if ((i == n - 1) || (array[i + 1] <= array[i])) {
      if ((i - j) > result) {
        result = i - j + 1;
      }
      j = i + 1;
    }
  }

  return result;
}
