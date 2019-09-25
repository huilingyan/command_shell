#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COLS 10
#define ROWS 10

void error(const char * msg) {
  fprintf(stderr, "Invalid input: %s\n", msg);
  exit(EXIT_FAILURE);
}

void read_line(char line[], FILE * f) {
  int i;
  int ch;

  i = 0;
  while (1) {
    ch = fgetc(f);
    switch (ch) {
      case '\n':
        if (i == 10) {
          line[10] = '\0';
          return;
        }
        else {
          error("line is too short");
        }
        break;
      case EOF:
        if (i == 10) {
          error("unexpected EOF");
        }
        else {
          error("line is too short");
        }
        break;
      default:
        if (i < 10) {
          line[i++] = (char)ch;
        }
        else {
          error("line is too long");
        }
        break;
    }
  }
}

void print_rotated(char matrix[10][10]) {
  int i, j;

  for (i = 0; i < 10; i++) {
    for (j = 0; j < 10; j++) {
      printf("%c", matrix[9 - j][i]);
    }
    printf("\n");
  }
}

int main(int argc, char * argv[]) {
  char matrix[10][10];
  FILE * f;
  int i, ch;

  if (argc != 2) {
    fprintf(stderr, "Usage: rotateMatrix input\n");
    exit(EXIT_FAILURE);
  }
  if ((f = fopen(argv[1], "r")) == 0) {
    fprintf(stderr, "Could not open %s\n", argv[1]);
    exit(EXIT_FAILURE);
  }
  for (i = 0; i < 10; i++) {
    read_line(matrix[i], f);
  }
  if ((ch = fgetc(f)) != EOF) {
    fprintf(stderr, "Invalid input: file is too long (read %d instead of EOF)\n", ch);
    exit(EXIT_FAILURE);
  }
  fclose(f);

  print_rotated(matrix);

  return 0;
}
