#include <limits.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <istream>
#include <map>
#include <string>
#include <utility>
#include <vector>
#define LIM 100

std::string skip_white_space(std::string str) {
  if (str.find_first_not_of(" ") != std::string::npos) {
    str.erase(0, str.find_first_not_of(" "));
  }
  return str;
}
//when read in an command, split it for execve()
char ** parse(std::string & command) {
  std::vector<std::string> str_split;
  int closed_quote = 0;
  int backslash = 0;

  //skip the leading white space
  command = skip_white_space(command);
  //parse the command out, set as argv[0]
  if (command.find(" ") != std::string::npos) {
    std::string first_command = command.substr(0, command.find(" "));
    str_split.push_back(first_command);
    command.erase(0, command.find(" "));
  }
  std::string arguments = skip_white_space(command);

  //parse the arguments
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
        str_split.push_back(parts_of_argu);                  // put argument into argv[]
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
    str_split.push_back(arguments);
  }
  if (closed_quote) {
    std::cout << "Error: Unclosed quotation mark!" << std::endl;
  }

  char ** cmd_parse = new char *[str_split.size() + 1];
  const char * cmd_ccharType;
  char * cmd_char;
  for (std::string::size_type j = 0; j < str_split.size(); j++) {
    cmd_ccharType = str_split[j].c_str();
    cmd_char = const_cast<char *>(cmd_ccharType);
    cmd_parse[j] = cmd_char;
    std::cout << cmd_parse[j] << std::endl;
  }
  cmd_parse[str_split.size()] = NULL;  // the last element of argv[] must be NULL

  return cmd_parse;
}

std::string getFullPath(std::string & command, char * envList) {
  struct stat sb;
  std::string fullPath;
  std::vector<std::string> str_split;
  std::string envList_trans = envList;
  while (envList_trans.find(":") != std::string::npos) {
    std::string parts = envList_trans.substr(0, envList_trans.find(":"));
    str_split.push_back(parts);
    envList_trans.erase(0, envList_trans.find(":") + 1);
  }
  str_split.push_back(envList_trans);
  for (std::vector<std::string>::size_type i = 0; i != str_split.size(); ++i) {
    std::string Path = str_split[i] + "/" + command;
    const char * Pathname = Path.c_str();
    if (stat(Pathname, &sb) == 0) {
      fullPath = Pathname;
      return fullPath;
    }
  }
  return "No path found";
}

int main(int argc, char * argv[]) {
  std::string command;
  char ** parsed_command;
  pid_t child_pid, w;
  int wstatus;
  char curr_dirt[PATH_MAX];
  std::map<char *, char *> VarToVal;
  //std::vector<std::pair<char *, char *> > VarToVal;

  while (1) {
    std::cout << "ffosh:" << getcwd(curr_dirt, PATH_MAX) << " $ ";
    std::getline(std::cin, command);
    command = skip_white_space(command);
    if ((!command.compare("exit")) || (std::cin.eof())) {  //if exit or end of file
      std::map<char *, char *>::iterator it = VarToVal.begin();
      for (it = VarToVal.begin(); it != VarToVal.end(); ++it) {
        free(it->first);
        free(it->second);
      }
      return EXIT_SUCCESS;
    }

    char * ECE551PATH = getenv("PATH");
    //setenv("ECE551PATH", path, 1);
    //char * newPath = getenv("ECE551PATH");

    //print out for test
    //std::cout << ECE551PATH << std::endl;

    char envVar[LIM];
    strcpy(envVar, "ECE551PATH=");
    char * newEnv[] = {strcat(envVar, ECE551PATH), NULL};

    parsed_command = parse(command);

    std::string path_org = parsed_command[0];

    if (path_org == "cd") {                  // if the command is "cd"
      if (chdir(parsed_command[1]) == -1) {  //if chdir() failed
        perror("chdir");
        exit(EXIT_FAILURE);
      }
    }
    else if (path_org == "set") {  //if the command is "set"
      char * key = strdup(parsed_command[1]);
      char * value = strdup(parsed_command[2]);
      //std::pair<char *, char *> kvPairs(parsed_command[1], parsed_command[2]);
      //VarToVal.insert(
      //std::map<char *, char *>::value_type(parsed_command[1], parsed_command[2]));
      VarToVal.insert(std::pair<char *, char *>(key, value));
      //VarToVal.push_back(std::make_pair(parsed_command[1], parsed_command[2]));

      // print out the elements of map for test
      std::map<char *, char *>::iterator it = VarToVal.begin();
      std::cout << "VarToVal contains: " << std::endl;
      for (it = VarToVal.begin(); it != VarToVal.end(); ++it) {
        std::cout << it->first << " => " << it->second << std::endl;
      }
      //free(key);
      //free(value);
    }
    else {
      if (path_org.find("/") ==
          std::string::
              npos) {  //if the command doesn't have "/", then search for the path

        std::string path_full = getFullPath(path_org, ECE551PATH);
        if (path_full == "No path found") {
          std::cout << "Command " << path_org << " not found" << std::endl;
        }
        parsed_command[0] = const_cast<char *>(path_full.c_str());
      }

      // create new process
      child_pid = fork();    //create the child process
      if (child_pid == 0) {  // Codes executed by child process
        if (execve(parsed_command[0], parsed_command, newEnv) ==
            -1) {  //if execve() failed
          perror("execve");
          exit(EXIT_FAILURE);
        }
      }
      else {  // Code executed by parent process
        delete[] parsed_command;
        do {
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
              std::cout << "Program failed with code " << WEXITSTATUS(wstatus)
                        << std::endl;
            }
          }
          else if (WIFSIGNALED(wstatus)) {
            std::cout << "Terminated by signal " << WTERMSIG(wstatus) << std::endl;
          }
        } while (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus));
      }
    }
  }

  return EXIT_SUCCESS;
}
