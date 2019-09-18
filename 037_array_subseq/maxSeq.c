#include <stdio.h>
#include <stdlib.h>

size_t maxSeq(int * array, size_t n) {
  size_t result = 1, len = 1;
  size_t i = 1;
  if (n == 0) {
    return 0;
  }
  for (i = 1; i < n; i++) {
    if (array[i] > array[i - 1]) {
      len++;
    }
    else {
      if (len > result) {
        result = len;
      }
      len = 1;
    }
  }
  if (len > result) {
    result = len;
  }

  return result;
}
