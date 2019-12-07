#ifndef CMDSHELL_H
#define CMDSHELL_H

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
#define LIM 100

#include "parse.h"

class CmdShell {
  std::string command;
  std::vector<std::string> cmdParse;
  char ** parsed_command;
};
#endif
