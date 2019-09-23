#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

int fndmax(int array[], int n) {
  int max = 0;
  for (int i = 0; i < n; i++) {
    if (array[i] > array[max]) {
      max = i;
    }
  }

  return max;
}

int getkey(FILE * f) {
  int c;
  int max;
  int freqn[26] = {0};
  while ((c = fgetc(f)) != EOF) {
    if (isalpha(c)) {
      freqn[tolower(c) - 'a']++;
    }
  }
  max = fndmax(freqn, 26);
  return (max - 4 + 26) % 26;
}

int main(int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: encrypt key inputFileName\n");
    return EXIT_FAILURE;
  }

  FILE * f = fopen(argv[1], "r");
  if (f == NULL) {
    perror("Could not open file");
    return EXIT_FAILURE;
  }
  int key = getkey(f);
  printf("%d\n", key);
  if (fclose(f) != 0) {
    perror("Failed to close the input file!");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
