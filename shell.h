#ifndef _SHELL_
#define _SHELL_

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
#include <ios>
#include <limits>
#include <istream>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "CheckBuiltInFunc.h"
#include "parse.h"
#define LIM 100

class Shell {

 public:

  char * ECE551PATH; // env variables
  std::vector<std::string> Environ; // list of env variables
  std::map<char *, char *> VarToVal; // map of env vars and corresponding values
  char curr_dirt[PATH_MAX];

  std::string command;
  std::vector<std::string> cmdParse;
  char ** parsed_command;
  std::string path_org;

  pid_t child_pid, w;
  int wstatus;

  // constructor
  Shell() {
    ECE551PATH = getenv("PATH");
    char envVar[LIM];
    strcpy(envVar, "ECE551PATH=");
    strcat(envVar, ECE551PATH);
    Environ.push_back(envVar);   
  }

  /* Methods */

  /* 1. prior check before parsing */
  void show_prompt();
  void get_input();
  bool check_exit();
  bool check_empty_cmd();

  /* 2. command parsing */
  void subst_var();
  char ** parse_cmd();

  /* 3. execute commands */
  bool handle_cd();
  bool handle_set();
  bool handle_rev();
  bool handle_export();

  // void exec_built_in();
  void find_abs_path();

  /* 4. create new processes */
  void fork_and_exec();


  /* wrap-up method */
  int run();

};

void Shell::show_prompt() {
  std::cout << "ffosh:" << getcwd(curr_dirt, PATH_MAX) << " $ ";
}

/* a method which gets command from the "input" */
void Shell::get_input() {

  // std::string str;
  // std::getline(std::cin, str);
  std::getline(std::cin, command);
  // debug
  std::cout << "command is: " << command << " $ ";
  // command = str;


  // std::cin.clear();
  // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

/* a method which handles 'exit' cmd */
bool Shell::check_exit() {
  if ((!command.compare("exit")) || (std::cin.eof())) {  //if exit or end of file
    CheckExit(VarToVal);
    return true;
    // return EXIT_SUCCESS;
  }
  return false;
}

/* a method which handles empty cmd */
bool Shell::check_empty_cmd() {
  if ((command.find_first_not_of(" ") == std::string::npos) ||
      (command.empty())) {  // if no command
    // continue;
    return true;
  }
  return false;  
}

/* a method which substitutes vars with values */
void Shell::subst_var() {
  std::string::size_type pos = 0;
  while (pos < command.length()) {
    if ((pos = command.find("$", pos)) != std::string::npos) { // If found $
      std::map<char *, char *>::iterator it = VarToVal.begin();
      for (it = VarToVal.begin(); it != VarToVal.end(); ++it) { // Find the var in map
        std::string keyToFind = it->first;
        std::string keyToSubt = "$" + keyToFind;
        std::string ValToSubt = it->second;
        if (command.find(keyToSubt, pos) != std::string::npos) { // If found
          command.replace(pos, keyToSubt.length(), ValToSubt);  // Substitue var with value
          pos += ValToSubt.length();
          break;
        }
      }
      pos += 1;
      continue;
    }
    return;
  }
  return;
}

/* a method which parses the command */
char ** Shell::parse_cmd() {

  cmdParse.clear();
  // std::vector<std::string> str_split;
  int closed_quote = 0;
  int backslash = 0;

  //skip the leading white space
  command = skip_white_space(command);

  //parse the command out, set as argv[0]
  std::string first_command = command.substr(0, command.find(" "));
  cmdParse.push_back(first_command);
  command.erase(0, command.find(" "));

  std::string arguments = skip_white_space(command);

  if (cmdParse[0] == "set") {  // if the command is "set", parse specially
    if (arguments.find(" ") == std::string::npos) {  //if there's no value to set
      std::cerr << "No value. Invalid use of set!" << std::endl;
      exit(EXIT_FAILURE);
    }
    else {
      size_t pos = arguments.find(" ");
      std::string var = arguments.substr(0, pos);
      for (std::string::size_type i = 0; i < var.length(); i++) {
        if (!(isalnum(var[i]) || (var[i] == '_'))) {  // if not legal var name
          std::cerr << "Invalid variable name!" << std::endl;
          exit(EXIT_FAILURE);
        }
      }
      cmdParse.push_back(var);
      arguments = arguments.erase(0, pos);
      std::string value = skip_white_space(
          arguments);  // skip the leading space and rest of argument is set as value
      cmdParse.push_back(value);
    }
  }
  else {
    //parse the arguments regularly
    std::string::size_type i = 0;

    while (i != arguments.length()) {  // if not end of string
      if (arguments[i] == '\\') {      //if backslash
        if (backslash) {               // if backslash before
          backslash = 0;
          i++;
          continue;
        }
        else {  //if not backslash before
          backslash = 1;
          arguments.erase(i, 1);
          continue;
        }
      }

      else if (arguments[i] == '"') {  //if quotation mark
        if (backslash) {               // if backslash before
          backslash = 0;
          i++;
          continue;
        }
        else {                 // if not backslash before
          if (closed_quote) {  // if has unclosed quotation mark
            closed_quote = 0;
            arguments.erase(i, 1);
            continue;
          }
          else {  // if no unclosed quotation mark
            closed_quote = 1;
            arguments.erase(i, 1);
            continue;
          }
        }
      }

      else if (arguments[i] == ' ') {  // if white space
        if (backslash) {
          backslash = 0;
        }

        if (closed_quote) {  // if unclosed quotation mark
          i++;
          continue;
        }
        else {  // if no unclosed quotation mark
          std::string parts_of_argu = arguments.substr(0, i);  //take as argument
          cmdParse.push_back(parts_of_argu);                  // put argument into argv[]
          arguments.erase(0, i);                               //erase current argument
          arguments = skip_white_space(
              arguments);  // skip the leading white space before next argument
          i = 0;
          continue;
        }
      }

      else {  // if normal character
        if (backslash) {
          backslash = 0;
        }
        i++;
        continue;
      }
    }
    if (!arguments.empty()) {  // for the last argument
      cmdParse.push_back(arguments);
    }
    if (closed_quote) {
      std::cout << "Error: Unclosed quotation mark!" << std::endl;
    }
  }

  // DEBUG
  std::cout << "The command is now parsed to : " << std::endl;

  StrVecToCharArr(parsed_command, cmdParse);  //Need converting to char** for execve()
  path_org = parsed_command[0]; // get the original path

  return parsed_command;
}

/* a method which handles 'cd' command */
bool Shell::handle_cd() {
  if (path_org == "cd") {  // if the command is "cd"
    ChangeDirt(parsed_command);
    return true;
    // continue;
  }
  return false;
}

/* a method which handles 'set' command */
bool Shell::handle_set() {
  if (path_org == "set") {  //if the command is "set"
    SetVar(parsed_command, VarToVal);
    return true;
    // continue;
  }
  return false;
}

/* a method which handles 'rev' command */
bool Shell::handle_rev() {
  if (path_org == "rev") {  // if the command is "rev"
  RevVal(parsed_command, VarToVal);
  return true;
  // continue;
  }
  return false;
}

/* a method which handles 'export' command */
bool Shell::handle_export() {
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

  return true;
  // continue;
  }
  return false;
}

/* a method which handles built-in commands */
// void Shell::exec_built_in() {
//   // path_org = parsed_command[0];

//   if (path_org == "cd") {  // if the command is "cd"
//     ChangeDirt(parsed_command);
//     continue;
//   }
//   if (path_org == "set") {  //if the command is "set"
//     SetVar(parsed_command, VarToVal);
//     continue;
//   }

//   if (path_org == "rev") {  // if the command is "rev"
//     RevVal(parsed_command, VarToVal);
//     continue;
//   }

//   if (path_org == "export") {  // if the command is "export"
//     char * VarToExp = strdup(parsed_command[1]);
//     if (!ExportVar(VarToExp, VarToVal, Environ)) {
//       std::cerr << "No such variable to export!" << std::endl;
//     }
//     // print out for test
//     std::cout << "Now envp[] contains:" << std::endl;
//     for (std::string::size_type j = 0; j < Environ.size(); j++) {
//       std::cout << Environ[j] << std::endl;
//     }
//     std::cout << "\n";
//     free(VarToExp);
//     continue;
//   }
// }

/* a method which finds the absolute path */
void Shell::find_abs_path() {
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
}

/* a method which creates new process */
void Shell::fork_and_exec() {
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

int Shell::run() {

  while (true) {
    show_prompt();
    get_input();
    if (check_exit()) { // if cmd is exit
      return EXIT_SUCCESS;
    }
    if (check_empty_cmd()) { // if is empty cmd 
      continue;
    }

    subst_var();
    parse_cmd();

    if (handle_cd()) { // if the command is "cd"
      continue;
    }
    if (handle_set()) { // if the command is "set"
      continue;
    }
    if (handle_rev()) { // if the command is "rev"
      continue;
    }
    if (handle_export()) { // if the command is "export"
      continue;
    }

    find_abs_path();

    fork_and_exec();
  } 

}

#endif