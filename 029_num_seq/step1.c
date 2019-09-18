// This file is for Step 1.
// You should do
//  Step 1 (A): write seq1
//  Step 1 (B): write main to test seq1
//  Step 1 (C): write printSeq1Range
//  Step 1 (D): add test cases to main to test printSeq1Range
//
// Be sure to #include any header files you need!

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int seq1(int x) {
  return 4 * x - 3;
}

void printSeq1Range(int low, int high) {
  if (low >= high) {
    printf("\n");
  }
  else {
    int i = low;
    while (i < high - 1) {
      printf("%d, ", seq1(i));
      i++;
    }
    printf("%d\n", seq1(high - 1));
  }
}

void test_seq1(int x) {
  int y = seq1(x);
  printf("seq1(%d) = %d\n", x, y);
}

void test_printSeq1Range(int low, int high) {
  printf("printSeq1Range(%d, %d)\n", low, high);
  printSeq1Range(low, high);
}

int main() {
  test_seq1(0);
  test_seq1(1);
  test_seq1(-1);
  test_seq1(-4);
  test_seq1(10);
  test_seq1(-32768);
  test_seq1(32767);

  test_printSeq1Range(0, 0);
  test_printSeq1Range(1, 1);
  test_printSeq1Range(0, 4);
  test_printSeq1Range(-2, 6);
  test_printSeq1Range(7, 2);
  test_printSeq1Range(-6, -2);
  test_printSeq1Range(-6, -7);
}
