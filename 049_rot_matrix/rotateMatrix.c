/*#include <ctype.h>
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
  for (;;) {
    ch = fgetc(f);
    switch (ch) {
      case '\n':
        if (i == COLS) {
          line[COLS] = '\0';
          return;
        }
        else {
          error("line is too short");
        }
        break;
      case EOF:
        if (i == COLS) {
          error("unexpected EOF");
        }
        else {
          error("line is too short");
        }
        break;
      default:
        if (i < COLS) {
          line[i++] = (char)ch;
        }
        else {
          error("line is too long");
        }
        break;
    }
  }
}

void print_rotated(char M[ROWS][COLS]) {
  int i, j;

  for (i = 0; i < ROWS; i++) {
    for (j = 0; j < COLS; j++) {
      putchar(M[COLS - 1 - j][i]);
    }
    putchar('\n');
  }
}

int main(int argc, char * argv[]) {
  char matrix[ROWS][COLS];
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
  for (i = 0; i < ROWS; i++) {
    read_line(matrix[i], f);
  }
  if ((ch = fgetc(f)) != EOF) {
    fprintf(stderr, "Invalid input: file is too long (read %d instead of EOF)\n", ch);
    exit(EXIT_FAILURE);
  }
  fclose(f);

  print_rotated(matrix);

  return 0;
}*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void error_check(char array[10][11], FILE * f) {
  int i = 0, j = 0;
  int ch;
  while ((ch = fgetc(f)) != EOF) {
    switch (ch) {
      case '\n': {
        const char * a = strchr(array[i], '\n');
        if ((a - array[i] != 10)) {
          fprintf(stderr, "Incorrect number of columns!\n");
          exit(EXIT_FAILURE);
        }
        i++;
        j = 0;
        break;
      }

        //case EOF: {
        //if (i != 10) {
        //fprintf(stderr, "Incorrect number of lines!\n");
        //exit(EXIT_FAILURE);
        //}
        //}
      default:
        array[i][j] = (char)ch;
        j++;
        break;
    }
    if (i != 10) {
      fprintf(stderr, "Incorrect number of lines!\n");
      exit(EXIT_FAILURE);
    }
  }
}
void rotate(char matrix[10][11]) {
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

  char matrix[10][11];

  error_check(matrix, f);
  rotate(matrix);
  if (fclose(f) != 0) {
    fprintf(stderr, "Failed to close the input file\n");
    return EXIT_FAILURE;
  }  //check if file can be closed
  return EXIT_SUCCESS;
}
