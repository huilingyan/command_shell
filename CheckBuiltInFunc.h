#ifndef CHECKBUILTINFUNC_H
#define CHECKBUILTINFUNC_H

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
#include <string>
#include <utility>
#include <vector>

#include "parse.h"

void CheckExit(std::map<char *, char *> & VarToVal);

void ChangeDirt(char **& parsed_command);

void SetVar(char **& parsed_command, std::map<char *, char *> & VarToVal);

void RevVal(char **& parsed_command, std::map<char *, char *> & VarToVal);

#endif
