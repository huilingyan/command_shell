#include <algorithm>  // std::sort
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>  // std::cout
#include <string>
#include <vector>  // std::vector

void sort_lines(std::istream & file) {
  std::vector<std::string> lines;
  std::string str;

  while (getline(file, str)) {
    lines.push_back(str);
  }
  std::sort(lines.begin(), lines.end());

  for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); ++it) {
    std::cout << *it << std::endl;
  }
}

int main(int argc, char ** argv) {
  std::ifstream files;

  if (argc == 1) {
    sort_lines(std::cin);
  }
  else {
    for (int i = 1; i < argc; i++) {
      files.open(argv[i]);
      if (files.fail()) {
        std::cerr << "Failed to open the file!\n" << std::endl;
        exit(EXIT_FAILURE);
      }
      sort_lines(files);
    }
  }
  return EXIT_SUCCESS;
}
