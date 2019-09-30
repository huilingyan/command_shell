#include "election.h"

#include <stdio.h>

#include "string.h"
//include any other headers you need here...

//The emacs automatically makes some WEIRD changes on format (if statements and eliminate the
//newlines i made for readability) when i save the file.
//None of my business!!!!!
void report_error(const char * s) {
  fprintf(stderr, "%s", s);
  exit(EXIT_FAILURE);
}
state_t parseLine(const char * line) {
  //STEP 1: write me
  state_t state;
  const char * sep = line;
  const char * c = line;
  //If the string is empty, error
  if (*line == '\0') {
    report_error("Empty line!\n");
  }

  //get the name of states
  sep = strchr(c, ':');  //find the location of ':', where it first appears
  if (sep == NULL) {
    report_error("No ':' found; improper format!\n");
  }
  if ((size_t)(sep - c) >
      MAX_STATE_NAME_LENGTH - 1) {  //If the size of state name exceeds the limit, error
    report_error("Too long state name!\n");
  }
  strncpy(state.name, c, (size_t)(sep - c));  //copy the state name into struct
  state.name[(size_t)(sep - c)] = '\0';       //add a null-terminator
  sep++;
  c = sep;  //Both c and sep point to the next character after first ':'

  //get the population
  state.population = 0;
  while (*sep >= '0' && *sep <= '9') {  //if reads a number
    state.population =
        state.population * 10 + (uint64_t)(*sep - '0');  //convert char to unsigned
    // 64bit int, and add from the most significant bit
    sep++;
  }
  if (*sep != ':') {  //if previous while loop exit without reading a ':'
    report_error("Invalid character appeared!\n");
  }
  if (sep == c) {
    report_error("No population!\n");
  }
  sep++;
  c = sep;  //Both c and sep point to the next character after ':'
  //get the electoral votes
  state.electoralVotes = 0;
  while (*sep >= '0' && *sep <= '9') {  //if reads a number
    state.electoralVotes = state.electoralVotes * 10 +
                           (unsigned)(*sep - '0');  //conver char to unsigned int,
    //add from the most significant bit
    sep++;
  }
  if (*sep != '\0') {  //if previous while loop exit without reading a '\0'
    report_error("Invalid inputs in electoral votes!\n");
  }
  if (sep == c) {
    report_error("No electoral votes!\n");
  }

  return state;
}

unsigned int countElectoralVotes(state_t * stateData,
                                 uint64_t * voteCounts,
                                 size_t nStates) {
  //STEP 2: write me
  unsigned count = 0;
  if (!stateData) {  //if no info for stateData
    report_error("Cannot get the information of the state!\n");
  }
  if (!voteCounts) {  //if no info for vote counts
    report_error("Cannot get the voteCounts!\n");
  }
  for (size_t i = 0; i < nStates; i++) {
    if (voteCounts[i] > stateData[i].population / 2) {
      count = count + stateData[i].electoralVotes;
    }
  }
  return count;
}

void printRecounts(state_t * stateData, uint64_t * voteCounts, size_t nStates) {
  //STEP 3: write me
  if (!stateData) {  //if no info for stateData
    report_error("Cannot get the information of the state!\n");
  }
  if (!voteCounts) {  //if no info for vote counts
    report_error("Cannot get the voteCounts!\n");
  }

  double pct;
  for (size_t i = 0; i < nStates; i++) {
    pct = voteCounts[i] / (double)stateData[i].population * 100;
    if (pct >= 49.500000 && pct <= 50.500000) {
      printf("%s requires a recount (Candidate A has %.2f%% of the vote)\n",
             stateData[i].name,
             pct);
    }
  }
}
size_t findMaxIndex(double * array, size_t n) {
  if (n <= 0) {
    report_error("n should be non-zero int!\n");
  }
  size_t maxidx = 0;
  for (size_t i = 1; i < n; i++) {
    if (array[i] > array[maxidx]) {
      maxidx = i;
    }
  }
  return maxidx;
}
void printLargestWin(state_t * stateData, uint64_t * voteCounts, size_t nStates) {
  //STEP 4: write me
  double pct[nStates];
  size_t MaxIdx;
  if (!stateData) {  //if no info for stateData
    report_error("Cannot get the information of the state!\n");
  }
  if (!voteCounts) {  //if no info for vote counts
    report_error("Cannot get the voteCounts!\n");
  }

  for (size_t i = 0; i < nStates; i++) {
    pct[i] = voteCounts[i] / (double)stateData[i].population * 100;
  }
  MaxIdx = findMaxIndex(pct, nStates);
  printf("Candidate A won %s with %.2f%% of the vote\n",
         stateData[MaxIdx].name,
         pct[MaxIdx]);
}
