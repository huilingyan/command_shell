#ifndef PARSE_H
#define PARSE_H

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

#include "CheckBuiltInFunc.h"

std::string skip_white_space(std::string str);

std::string reverse(std::string & s);

void StrVecToCharArr(char **& arr, std::vector<std::string> & vec);

//when read in an command, split it for execve()
std::vector<std::string> parse(std::string & command);

std::string getFullPath(std::string & command, char * envList);

bool ExportVar(char * var,
               std::map<char *, char *> & VarMap,
               std::vector<std::string> & Env);

//substitute every var with value
void VarParse(std::string & command, std::map<char *, char *> & VarToVal);

#endif
