#include <cstdio>
#include <cstdlib>
#include <iostream>

// This program will cause segmentation fault
// Which is for testing step1 - "terminated by signal"
int main(void) {
  int arr[5];
  for (int i = 0; i <= 10; i++) {
    arr[i] = i;
    std::cout << arr[i] << std::endl;
  }
  return EXIT_SUCCESS;
}
