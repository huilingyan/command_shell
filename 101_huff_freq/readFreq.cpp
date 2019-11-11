#include "readFreq.h"

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>  // std::cout
#include <istream>
#include <string>
#include <vector>  // std::vector

void printSym(std::ostream & s, unsigned sym) {
  if (sym > 256) {
    s << "INV";
  }
  else if (sym == 256) {
    s << "EOF";
  }
  else if (isprint(sym)) {
    char c = sym;
    s << "'" << c << "'";
  }
  else {
    std::streamsize w = s.width(3);
    s << std::hex << sym << std::dec;
    s.width(w);
  }
}
uint64_t * readFrequencies(const char * fname) {
  //WRITE ME!
  int ch;
  uint64_t * freqn = new uint64_t[256]();
  std::ifstream f;
  f.open(fname);
  if (!f.is_open()) {
    std::cerr << "Failed to open the file!\n" << std::endl;
    exit(EXIT_FAILURE);
  }
  while ((ch = f.get()) != EOF) {
    freqn[ch]++;
  }
  freqn[256]++;
  f.close();
  return freqn;
}
/* void sort_lines(std::istream & file) {
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
        files.open(argv[i], std::ifstream::in);
        //if (files.fail()) {
        if (!files.is_open()) {
          std::cerr << "Failed to open the file!\n" << std::endl;
          exit(EXIT_FAILURE);
        }
        sort_lines(files);
        files.close();
      }
    }
    return EXIT_SUCCESS;
  */
