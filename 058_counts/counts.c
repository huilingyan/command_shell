#include "counts.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

counts_t * createCounts(void) {
  //WRITE ME
  counts_t * c_array = malloc(sizeof(*c_array));
  if (c_array == NULL) {
    fprintf(stderr, "No Inputs!\n");
    exit(EXIT_FAILURE);
  }
  c_array->counts = NULL;
  c_array->n = 0;
  c_array->unknown = 0;

  return c_array;
}
void addCount(counts_t * c, const char * name) {
  //WRITE ME
  int name_exists = 1;
  if (c == NULL) {
    fprintf(stderr, "No inputs!\n");
    exit(EXIT_FAILURE);
  }
  if (name == NULL) {
    c->unknown++;
  }
  else {
    for (size_t i = 0; i < c->n; i++) {
      if (!strcmp(
              name,
              c->counts[i]->s)) {  //if the name already exists, simply plus count by 1
        name_exists = 0;
        c->counts[i]->count++;
        break;
      }
    }
    if (name_exists == 1) {  //if the name doesn't exist, allocate a memory on it
      c->counts = realloc(c->counts, (c->n + 1) * sizeof(*c->counts));
      c->counts[c->n] = malloc(sizeof(*c->counts[c->n]));
      c->counts[c->n]->s = strdup(name);
      c->counts[c->n]->count = 1;
      c->n++;
    }
  }
}
void printCounts(counts_t * c, FILE * outFile) {
  //WRITE ME
  for (size_t i = 0; i < c->n; i++) {
    fprintf(outFile, "%s : %lu\n", c->counts[i]->s, c->counts[i]->count);
  }
  if (c->unknown > 0) {
    fprintf(outFile, "<unknown> : %lu\n", c->unknown);
  }
}

void freeCounts(counts_t * c) {
  //WRITE ME
  for (size_t i = 0; i < c->n; i++) {
    free(c->counts[i]->s);
    free(c->counts[i]);
  }
  free(c->counts);
  free(c);
}
