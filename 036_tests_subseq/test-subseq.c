#include <stdio.h>
#include <stdlib.h>

size_t maxSeq(int * array, size_t n);

void check_seq(int array[], size_t n, size_t expected_ans) {
  size_t ans = maxSeq(array, n);
  if (ans != expected_ans) {
    printf("An error found!\n");
    exit(EXIT_FAILURE);
  }
}
int array0[] = {}, array1[] = {1}, array2[] = {1, 1}, array3[] = {1, 2, 3, 4, 5, 5},
    array4[] = {1, 2, 3, 4, 5, 7, 2, 4, 6, 9, 10}, array5[] = {-3, -2, -1, 1},
    array6[] = {-5, 4, -3, 2, -1, 0, 1, 2, 3, 4}, array7[] = {-5, -4, -3};

int main() {
  check_seq(array0, 0, 0);
  check_seq(array1, 1, 1);
  check_seq(array2, 2, 1);
  check_seq(array3, 6, 5);
  check_seq(array4, 11, 6);
  check_seq(array5, 4, 4);
  check_seq(array6, 10, 6);
  check_seq(array7, 3, 3);
  return EXIT_SUCCESS;
}
