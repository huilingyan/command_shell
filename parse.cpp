#include "parse.h"

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
  while (envList_trans.find(":") != std::string::npos) { // When found the :
    std::string parts = envList_trans.substr(0, envList_trans.find(":"));
    str_split.push_back(parts); // push into the vector
    envList_trans.erase(0, envList_trans.find(":") + 1);
  }
  str_split.push_back(envList_trans); // Push back the last path
  for (std::vector<std::string>::size_type i = 0; i != str_split.size(); ++i) {
    std::string Path = str_split[i] + "/" + command; // Concatenate path with command name
    const char * Pathname = Path.c_str();
    if (stat(Pathname, &sb) == 0) {  // If the absolute path is valid
      fullPath = Pathname;
      return fullPath;
    }
  }
  return command;
}

// Put the current value of var into the env for other programs
bool ExportVar(char * var,
               std::map<char *, char *> & VarMap,
               std::vector<std::string> & Env) {
  std::string VarToAdd = var;
  std::map<char *, char *>::iterator it = VarMap.begin();
  for (it = VarMap.begin(); it != VarMap.end(); ++it) {
    std::string keyToComp = it->first;
    std::string ValueToComp = it->second;
    if (keyToComp == VarToAdd) { // If we found the var-val pair in the map
      Env.push_back(keyToComp + "=" + ValueToComp); // Push into envp
      return true;
    }
  }
  return false;
}

//substitute every var with value
void VarParse(std::string & command, std::map<char *, char *> & VarToVal) {
  std::string::size_type pos = 0;
  while (pos < command.length()) {
    if ((pos = command.find("$", pos)) != std::string::npos) { // If found $
      std::map<char *, char *>::iterator it = VarToVal.begin();
      for (it = VarToVal.begin(); it != VarToVal.end(); ++it) { // Find the var in map
        std::string keyToFind = it->first;
        std::string keyToSubt = "$" + keyToFind;
        std::string ValToSubt = it->second;
        if (command.find(keyToSubt, pos) != std::string::npos) { // If found
          command.replace(pos, keyToSubt.length(), ValToSubt);  //Substitue var with value
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
