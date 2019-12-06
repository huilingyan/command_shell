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

std::string skip_white_space(std::string str) {
  //void skip_white_space(std::string str) {
  if (str.find_first_not_of(" ") != std::string::npos) {
    str.erase(0, str.find_first_not_of(" "));
  }
  return str;
}

std::string reverse(std::string & s) {
  std::string answer(s.length(), '\0');
  for (size_t i = 0; i < s.length(); i++) {
    answer[i] = s[s.length() - 1 - i];
  }
  return answer;
}

void StrVecToCharArr(char **& arr, std::vector<std::string> & vec) {
  /*for (std::vector<std::string>::size_type i = 0; i != vec.size(); ++i) {
    arr[i] = const_cast<char *>(vec[i].c_str());
    std::cout << arr[i] << std::endl;
    }*/
  arr = new char *[vec.size() + 1];
  for (std::string::size_type j = 0; j < vec.size(); j++) {
    arr[j] = new char[vec[j].length() + 1];
    strcpy(arr[j], vec[j].c_str());
    std::cout << arr[j] << std::endl;
  }
  arr[vec.size()] = NULL;  // the last element of arr[] must be NULL
}

//when read in an command, split it for execve()
std::vector<std::string> parse(std::string & command) {
  std::vector<std::string> str_split;
  int closed_quote = 0;
  int backslash = 0;

  //skip the leading white space
  command = skip_white_space(command);
  //skip_white_space(command);
  //parse the command out, set as argv[0]
  std::string first_command = command.substr(0, command.find(" "));
  str_split.push_back(first_command);
  command.erase(0, command.find(" "));

  std::string arguments = skip_white_space(command);

  if (str_split[0] == "set") {  // if the command is "set", parse specially
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
      str_split.push_back(var);
      arguments = arguments.erase(0, pos);
      std::string value = skip_white_space(
          arguments);  // skip the leading space and rest of argument is set as value
      str_split.push_back(value);
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
  }

  return str_split;
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
  return command;
}

bool ExportVar(char * var,
               std::map<char *, char *> & VarMap,
               std::vector<std::string> & Env) {
  std::string VarToAdd = var;
  std::map<char *, char *>::iterator it = VarMap.begin();
  for (it = VarMap.begin(); it != VarMap.end(); ++it) {
    std::string keyToComp = it->first;
    std::string ValueToComp = it->second;
    if (keyToComp == VarToAdd) {
      Env.push_back(keyToComp + "=" + ValueToComp);
      return true;
    }
  }
  return false;
}

//substitute every var with value
void VarParse(std::string & command, std::map<char *, char *> & VarToVal) {
  std::string::size_type pos = 0;
  while (pos < command.length()) {
    if ((pos = command.find("$", pos)) != std::string::npos) {
      std::map<char *, char *>::iterator it = VarToVal.begin();
      for (it = VarToVal.begin(); it != VarToVal.end(); ++it) {
        std::string keyToFind = it->first;
        std::string keyToSubt = "$" + keyToFind;
        std::string ValToSubt = it->second;
        if (command.find(keyToSubt, pos) != std::string::npos) {
          command.replace(pos, keyToSubt.length(), ValToSubt);
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
      std::map<char *, char *>::iterator it = VarToVal.begin();
      for (it = VarToVal.begin(); it != VarToVal.end(); ++it) {
        free(it->first);
        free(it->second);
      }
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
    StrVecToCharArr(parsed_command, cmdParse);  //Need converting to char** for execve()

    // CheckBuiltinFunc
    std::string path_org = parsed_command[0];

    if (path_org == "cd") {  // if the command is "cd"
      // ChangeDirt
      if (chdir(parsed_command[1]) == -1) {  //if chdir() failed
                                             //perror("chdir");
                                             //exit(EXIT_FAILURE);
        std::cerr << "No such file or directory!" << std::endl;
        continue;  // exit and repeat the process
      }
    }
    if (path_org == "set") {  //if the command is "set"
      //SetVar
      char * key = strdup(parsed_command[1]);
      char * value = strdup(parsed_command[2]);
      VarToVal.insert(std::pair<char *, char *>(key, value));

      // print out the elements of map for test
      std::map<char *, char *>::iterator it = VarToVal.begin();
      std::cout << "VarToVal contains: " << std::endl;
      for (it = VarToVal.begin(); it != VarToVal.end(); ++it) {
        std::cout << it->first << " => " << it->second << std::endl;
      }
      continue;
    }

    if (path_org == "rev") {
      //RevVal
      std::string var_to_rev = parsed_command[1];
      std::map<char *, char *>::iterator it = VarToVal.begin();
      for (it = VarToVal.begin(); it != VarToVal.end(); ++it) {
        std::string keyToCompare = it->first;
        std::string ValueToRev = it->second;
        if (keyToCompare == var_to_rev) {
          ValueToRev = reverse(ValueToRev);
          strcpy(it->second, ValueToRev.c_str());
          break;
        }
      }
      if (it == VarToVal.end()) {
        std::cerr << "No such variable!" << std::endl;
        continue;
      }
      else {
        std::cout << "After reverse, the value is:" << it->second << std::endl;
        continue;
      }
    }

    if (path_org == "export") {
      //ExPortVar
      char * VarToExp = strdup(parsed_command[1]);
      if (!ExportVar(VarToExp, VarToVal, Environ)) {
        std::cerr << "No such variable to export!" << std::endl;
      }
      // print out for test
      for (std::string::size_type j = 0; j < Environ.size(); j++) {
        std::cout << Environ[j] << std::endl;
      }

      continue;
    }

    if (path_org.find("/") ==
        std::string::npos) {  //if the command doesn't have "/", then search for the path

      std::string path_full = getFullPath(path_org, ECE551PATH);
      if (path_full == path_org) {
        std::cout << "Command " << path_org << " not found" << std::endl;
      }
      else {
        parsed_command[0] = const_cast<char *>(path_full.c_str());
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
        for (std::string::size_type j = 0; j < cmdParse.size(); j++) {
          delete[] parsed_command[j];
        }
        delete[] parsed_command;
        exit(EXIT_FAILURE);
      }
    }
    else {  // Code executed by parent process
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
