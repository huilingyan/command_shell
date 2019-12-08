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
#include "parse.h"
#define LIM 100

int main(int argc, char * argv[]) {
  std::string command;
  std::vector<std::string> cmdParse;
  char ** parsed_command;
  pid_t child_pid, w;

  int wstatus;
  char curr_dirt[PATH_MAX];
  std::map<char *, char *> VarToVal;
  std::vector<std::string> Environ;
  char * ECE551PATH = getenv("PATH");
  char envVar[LIM];

  strcpy(envVar, "ECE551PATH=");
  strcat(envVar, ECE551PATH);
  Environ.push_back(envVar);

  while (1) {
    std::cout << "ffosh:" << getcwd(curr_dirt, PATH_MAX) << " $ ";
    std::getline(std::cin, command);

    // checkExit
    if ((!command.compare("exit")) || (std::cin.eof())) {  //if exit or end of file
      CheckExit(VarToVal);
      return EXIT_SUCCESS;
    }

    // checkEmptyCmd
    if ((command.find_first_not_of(" ") == std::string::npos) ||
        (command.empty())) {  // if no command
      continue;
    }

    // CommandParse
    VarParse(command, VarToVal);
    cmdParse = parse(command);
    std::cout << "The command is now parsed to : " << std::endl;
    StrVecToCharArr(parsed_command, cmdParse);  //Need converting to char** for execve()

    std::string path_org = parsed_command[0];

    if (path_org == "cd") {  // if the command is "cd"
      ChangeDirt(parsed_command);
      continue;
    }
    if (path_org == "set") {  //if the command is "set"
      SetVar(parsed_command, VarToVal);
      continue;
    }

    if (path_org == "rev") {  // if the command is "rev"
      RevVal(parsed_command, VarToVal);
      continue;
    }

    if (path_org == "export") {  // if the command is "export"
      char * VarToExp = strdup(parsed_command[1]);
      if (!ExportVar(VarToExp, VarToVal, Environ)) {
        std::cerr << "No such variable to export!" << std::endl;
      }
      // print out for test
      std::cout << "Now envp[] contains:" << std::endl;
      for (std::string::size_type j = 0; j < Environ.size(); j++) {
        std::cout << Environ[j] << std::endl;
      }
      std::cout << "\n";
      free(VarToExp);
      continue;
    }

    if (path_org.find("/") ==
        std::string::npos) {  //if the command doesn't have "/", then search for the path

      std::string path_full = getFullPath(path_org, ECE551PATH);
      if (path_full == path_org) {
        std::cout << "Command " << path_org << " not found" << std::endl;
      }
      else {
        delete[] parsed_command[0];
        parsed_command[0] = new char[path_full.length()];
        strcpy(parsed_command[0], path_full.c_str());
        //parsed_command[0] = const_cast<char *>(path_full.c_str());
      }
      //strcpy(parsed_command[0], path_full.c_str());
    }

    char ** newEnv;
    StrVecToCharArr(newEnv, Environ);

    // create new process
    child_pid = fork();    //create the child process
    if (child_pid == 0) {  // Codes executed by child process
      if (execve(parsed_command[0], parsed_command, newEnv) == -1) {  //if execve() failed
        perror("execve");
        // Have to handle memory by ourselves
        for (std::string::size_type j = 0; j < cmdParse.size() + 1; j++) {
          delete[] parsed_command[j];
        }
        delete[] parsed_command;
        exit(EXIT_FAILURE);
      }
    }
    else {  // Code executed by parent process
      do {
        //free the memory for envp[]
        for (std::string::size_type j = 0; j < Environ.size() + 1; j++) {
          delete[] newEnv[j];
        }
        delete[] newEnv;
        //free the memory for parsed_cmd
        for (std::string::size_type j = 0; j < cmdParse.size() + 1; j++) {
          delete[] parsed_command[j];
        }
        delete[] parsed_command;

        w = waitpid(child_pid, &wstatus, WUNTRACED | WCONTINUED);
        if (w == -1) {
          perror("waitpid");
          exit(EXIT_FAILURE);
        }

        if (WIFEXITED(wstatus)) {
          if (!WEXITSTATUS(wstatus)) {
            std::cout << "Program was successful" << std::endl;
          }
          else {
            std::cout << "Program failed with code " << WEXITSTATUS(wstatus) << std::endl;
          }
        }
        else if (WIFSIGNALED(wstatus)) {
          std::cout << "Terminated by signal " << WTERMSIG(wstatus) << std::endl;
        }
      } while (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus));
    }
  }

  return EXIT_SUCCESS;
}
