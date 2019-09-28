#include "election.h"

#include <stdio.h>

#include "string.h"
//include any other headers you need here...

state_t parseLine(const char * line) {
  //STEP 1: write me
  state_t state;
  const char * sep = line;
  const char * c = line;
  //If the string is empty, error
  if (line == NULL) {
    fprintf(stderr, "Empty line!");
    exit(EXIT_FAILURE);
  }
  //get the name of states
  sep = strchr(c, ':');  //find the location of ':', where it first appears
  if (sep == NULL) {
    fprintf(stderr, "No ':' found; improper format!");
    exit(EXIT_FAILURE);
  }
  if ((size_t)(sep - c) >
      MAX_STATE_NAME_LENGTH - 1) {  //If the size of state name exceeds the limit, error
    fprintf(stderr, "Too long state name!\n");
    exit(EXIT_FAILURE);
  }
  strncpy(state.name, c, (size_t)(sep - c));  //copy the state name into struct
  state.name[(size_t)(sep - c)] = '\0';       //add a null-terminator
  c = ++sep;  //Both c and sep point to the next character after first ':'

  //get the population
  state.population = 0;
  while ('0' <= *sep && *sep <= '9') {  //if reads a number
    state.population =
        state.population * 10 + (uint64_t)(*sep - '0');  //convert char to unsigned
    // 64bit int, and add from the least significant bit
    sep++;
  }
  if (*sep != ':') {  //if previous while loop exit without reading a ':'
    fprintf(stderr, "Invalid character appeared!");
    exit(EXIT_FAILURE);
  }
  c = ++sep;  //Both c and sep point to the next character after ':'
  //get the electoral votes
  state.electoralVotes = 0;
  while ('0' <= *sep && *sep <= '9') {  //if reads a number
    state.electoralVotes = state.electoralVotes * 10 +
                           (unsigned)(*sep - '0');  //conver char to unsigned int,
    //add from the least significant bit
    sep++;
  }
  if (*sep != '\0') {  //if previous while loop exit without reading a '\0'
    fprintf(stderr, "Invalid character appeared!");
    exit(EXIT_FAILURE);
  }

  return state;
}

unsigned int countElectoralVotes(state_t * stateData,
                                 uint64_t * voteCounts,
                                 size_t nStates) {
  //STEP 2: write me
  return 0;
}

void printRecounts(state_t * stateData, uint64_t * voteCounts, size_t nStates) {
  //STEP 3: write me
}

void printLargestWin(state_t * stateData, uint64_t * voteCounts, size_t nStates) {
  //STEP 4: write me
}
