#include "kv.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

kvpair_t * split_line(char * line) {
  char * sep = line;
  char * c = line;
  kvpair_t * pair = malloc(sizeof(*pair));
  sep = strchr(c, '=');
  if (sep == NULL) {
    fprintf(stderr, "invalid input!\n");
    exit(EXIT_FAILURE);
  }
  //pair->key = realloc(pair->key, sizeof(pair->key));
  //strncpy(pair->key, c, (sep - c));
  pair->key = strndup(c, sep - c);
  pair->key[sep - c] = '\0';
  c = ++sep;
  sep = strchr(c, '\n');
  //pair->value = realloc(pair->value, sizeof(pair->value));
  //strncpy(pair->value, c, (sep - c));
  pair->value = strndup(c, sep - c);
  pair->value[sep - c] = '\0';
  return pair;
}
kvarray_t * readKVs(const char * fname) {
  //WRITE ME
  FILE * f = fopen(fname, "r");
  if (f == NULL) {
    fprintf(stderr, "No input file!\n");
    exit(EXIT_FAILURE);
  }
  kvarray_t * kvarray = malloc(sizeof(*kvarray));
  kvarray->n = 0;
  kvarray->kv = malloc(sizeof(*kvarray->kv));
  char * line = NULL;
  size_t sz = 0;
  while (getline(&line, &sz, f) >= 0) {
    kvpair_t * pairs = split_line(line);
    kvarray->kv[kvarray->n] = pairs;
    kvarray->n++;
    kvarray->kv = realloc(kvarray->kv, (kvarray->n + 1) * sizeof(*kvarray->kv));
  }
  free(line);
  if (fclose(f) != 0) {
    fprintf(stderr, "Cannot close the file!\n");
    exit(EXIT_FAILURE);
  }
  return kvarray;
}

void freeKVs(kvarray_t * pairs) {
  //WRITE ME
  for (size_t i = 0; i < pairs->n; i++) {
    free(pairs->kv[i]->key);
    free(pairs->kv[i]->value);
    free(pairs->kv[i]);
  }
  free(pairs->kv);
  free(pairs);
}

void printKVs(kvarray_t * pairs) {
  //WRITE ME
  for (size_t i = 0; i < pairs->n; i++) {
    printf("key = '%s' value = '%s'\n", pairs->kv[i]->key, pairs->kv[i]->value);
  }
}

char * lookupValue(kvarray_t * pairs, const char * key) {
  //WRITE ME
  for (size_t i = 0; i < pairs->n; i++) {
    if (!strcmp(pairs->kv[i]->key, key)) {
      return pairs->kv[i]->key;
    }
  }
  return NULL;
}
