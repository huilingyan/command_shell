#include <stdio.h>
#include <stdlib.h>

unsigned power(unsigned x, unsigned y);

void run_check(unsigned x, unsigned y, unsigned expected_ans) {
  if (power(x, y) != expected_ans) {
    printf("There's a bug in your test case(%u, %u)!\n", x, y);
    exit(EXIT_FAILURE);
  }
}

int main() {
  run_check(0, 0, 1);
  run_check(1, 0, 1);
  run_check(0, 1, 0);
  run_check(2, 0, 1);
  run_check(4, 4, 256);
  run_check(123456, 1, 123456);

  return EXIT_SUCCESS;
}
