// This file is for Step 3.
// You should do
//  Step 3 (A): write seq3
//  Step 3 (B): write main to test seq3
//  Step 3 (C): write countEvenInSeq3Range
//  Step 3 (D): add test cases to main to test countEvenInSeq3Range
//
// Be sure to #include any header files you need!
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int seq3(int x, int y) {  //calculate the 2D sequence
  return x * (y - 3) + 2 * y;
}

int countEvenInSeq3Range(int xLow,
                         int xHi,
                         int yLow,
                         int yHi) {  //count the number of even in given range
  if ((xLow >= xHi) ||
      (yLow >=
       yHi)) {  //if either xlow is larger than xhigh or ylow is larger than yhigh, return 0 since there's no number
    return 0;
  }
  else {
    int sumofEven = 0;
    for (int i = xLow; i < xHi; i++) {
      for (int j = yLow; j < yHi; j++) {
        if (seq3(i, j) % 2 == 0) {  //when the request is met, count up for 1
          sumofEven++;
        }
      }
    }
    return sumofEven;
  }
}

void test_seq3(int x, int y) {
  int z = seq3(x, y);
  printf("seq3(%d, %d) = %d\n", x, y, z);
}

void test_countEvenInSeq3Range(int xLow, int xHi, int yLow, int yHi) {
  int z = countEvenInSeq3Range(xLow, xHi, yLow, yHi);
  printf("countEvenInSeq3Range(%d, %d, %d, %d) = %d\n", xLow, xHi, yLow, yHi, z);
}

int main(void) {
  test_seq3(0, 0);
  test_seq3(0, 1);
  test_seq3(0, -1);
  test_seq3(2, 2);
  test_seq3(-4, -5);

  test_countEvenInSeq3Range(0, 0, 0, 0);
  test_countEvenInSeq3Range(1, 2, 1, 1);
  test_countEvenInSeq3Range(1, 1, 2, 4);
  test_countEvenInSeq3Range(-5, -3, -5, -2);
  test_countEvenInSeq3Range(-1, -2, 1, 3);
  test_countEvenInSeq3Range(0, 2, 0, 3);
  test_countEvenInSeq3Range(2, 3, -1, 3);
  test_countEvenInSeq3Range(2, 3, -1, 4);
  test_countEvenInSeq3Range(0, 1, -2147483648, 1);

  return 0;
}
