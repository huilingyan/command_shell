// This file is for Step 2.
// You should do
//  Step 2 (A): write seq2
//  Step 2 (B): write main to test seq2
//  Step 2 (C): write sumSeq2
//  Step 2 (D): add test cases to main to test sumSeq2
//
// Be sure to #include any header files you need!

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int seq2(int x) {  //calculate the sequence
  if ((x - 2) % 4 == 0) {
    return 2 * x;
  }
  else {
    return x * x + 3 * x + 1;
  }
}

int sumSeq2(int low, int high) {  //calculate the sum of number within the given sequence
  if (low >=
      high) {  // if low is higher than high, it's 0 since there're no numbers in sequence
    return 0;
  }
  else {
    int sum = 0;
    for (int i = low; i < high; i++) {
      sum += seq2(i);
    }
    return sum;
  }
}

void test_seq2(int x) {
  int y = seq2(x);
  printf("seq2(%d) = %d\n", x, y);
}

void test_sumSeq2(int low, int high) {
  int z = sumSeq2(low, high);
  printf("sumSeq2(%d, %d) = %d\n", low, high, z);
}

int main(void) {
  test_seq2(0);
  test_seq2(1);
  test_seq2(2);
  test_seq2(-4);
  test_seq2(6);
  test_seq2(10);
  test_seq2(-1);
  test_seq2(14);

  test_sumSeq2(0, 0);
  test_sumSeq2(0, 2);
  test_sumSeq2(3, 6);
  test_sumSeq2(9, 7);
  test_sumSeq2(0, -2);
  test_sumSeq2(-3, -1);

  return 0;
}
