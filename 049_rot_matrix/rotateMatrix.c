#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void error_check(char array[10][12], FILE * f) {
  int i = 0;
  int ch;
  while ((ch = fgetc(f)) != EOF) {
    while (fgets(array[i], 12, f) != NULL) {
      const char * a = strchr(array[i], '\0');
      if ((a - array[i]) != 11) {
        fprintf(stderr, "Incorrect number of columns!\n");
      }
    }
    i++;
  }
  if (i != 10) {
    fprintf(stderr, "Incorrect number of lines!\n");
  }
}
void rotate(char matrix[10][12]) {
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      fprintf(stdout, "%c", matrix[j][9 - i]);
    }
    printf("\n");
  }
}

int main(int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: rotate matrix inputFileName\n");
    return EXIT_FAILURE;
  }  //check if the input file is a single line comment

  FILE * f = fopen(argv[1], "r");
  if (f == NULL) {
    fprintf(stderr, "Cannot open the file!\n");
    return EXIT_FAILURE;
  }  //if filename does not exist

  char matrix[10][12];

  error_check(matrix, f);
  rotate(matrix);
  if (fclose(f) != 0) {
    fprintf(stderr, "Failed to close the input file\n");
    return EXIT_FAILURE;
  }  //check if file can be closed
  return EXIT_SUCCESS;
}