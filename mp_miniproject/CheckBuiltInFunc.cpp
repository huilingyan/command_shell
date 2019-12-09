#include "CheckBuiltInFunc.h"

#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <fstream>
#include <iostream>
#include <istream>
#include <map>
#include <utility>
#include <vector>

#include "parse.h"
void CheckExit(std::map<char *, char *> & VarToVal) {
  std::map<char *, char *>::iterator it = VarToVal.begin();
  for (it = VarToVal.begin(); it != VarToVal.end(); ++it) { // Free the memory for map before exit the program
    free(it->first);
    free(it->second);
  }
  return;
}

void ChangeDirt(char **& parsed_command) {
  if (chdir(parsed_command[1]) == -1) {  //if chdir() failed
    std::cerr << "No such file or directory!" << std::endl;
  }
  return;
}

void SetVar(char **& parsed_command, std::map<char *, char *> & VarToVal) {
  char * key = strdup(parsed_command[1]);
  char * value = strdup(parsed_command[2]);
  VarToVal.insert(std::pair<char *, char *>(key, value)); // Push key-value pair into map

  // print out the elements of map for test
  std::map<char *, char *>::iterator it = VarToVal.begin();
  std::cout << "Now the VarToVal map contains: " << std::endl;
  for (it = VarToVal.begin(); it != VarToVal.end(); ++it) {
    std::cout << it->first << " => " << it->second << std::endl;
  }
  std::cout << "\n";
  return;
}

void RevVal(char **& parsed_command, std::map<char *, char *> & VarToVal) {
  std::string var_to_rev = parsed_command[1]; 
  std::map<char *, char *>::iterator it = VarToVal.begin();
  for (it = VarToVal.begin(); it != VarToVal.end(); ++it) { // Iterate through the map to find the var
    std::string keyToCompare = it->first;
    std::string ValueToRev = it->second;
    if (keyToCompare == var_to_rev) { // If found
      ValueToRev = reverse(ValueToRev); // Reverse value
      strcpy(it->second, ValueToRev.c_str()); // Change string into char*
      break;
    }
  }
  if (it == VarToVal.end()) {
    std::cerr << "No such variable!" << std::endl;
  }
  else {
    std::cout << "After reverse, the value is:" << it->second << std::endl;
  }
  return;
}
