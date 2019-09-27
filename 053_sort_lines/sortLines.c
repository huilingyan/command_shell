#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//This function is used to figure out the ordering
//of the strings in qsort.  You do not need
//to modify it.

int stringOrder(const void * vp1, const void * vp2) {
  const char * const * p1 = vp1;
  const char * const * p2 = vp2;
  return strcmp(*p1, *p2);
}
//This function will sort and print data (whose length is count).
void sortData(char ** data, size_t count) {
  qsort(data, count, sizeof(char *), stringOrder);
}
void sort_lines(FILE * f) {
  size_t sz = 0;
  ssize_t len = 0;
  char * line = NULL;
  int i = 0;
  char ** sort_array = NULL;
  while ((len = getline(&line, &sz, f)) >= 0) {
    sort_array = realloc(sort_array, (i + 1) * sizeof(*sort_array));
    sort_array[i] = line;
    line = NULL;
    i++;
  }
  //free(line);
  sortData(sort_array, i);
  for (size_t j = 0; j < i; j++) {
    printf("%s\n", sort_array[j]);
    free(sort_array[j]);
  }
  free(sort_array);
  if (fclose(f) != 0) {
    fprintf(stderr, "failed to close the input file!\n");
    exit(EXIT_FAILURE);
  }
}

int main(int argc, char ** argv) {
  //WRITE YOUR CODE HERE!
  if (argc == 1) {
    sort_lines(stdin);
  }
  else {
    for (int i = 1; i < argc; i++) {
      FILE * f = fopen(argv[i], "r");
      if (f == NULL) {
        fprintf(stderr, "Cannot open the file!\n");
        exit(EXIT_FAILURE);
      }
      sort_lines(f);
    }
  }

  return EXIT_SUCCESS;
}
