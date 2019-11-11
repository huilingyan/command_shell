#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <fstream>
#include <functional>
#include <iostream>
#include <istream>
#include <map>
#include <queue>

#include "node.h"
#include "readFreq.h"

void writeHeader(BitFileWriter * bfw, const std::map<unsigned, BitString> & theMap) {
  for (int i = 0; i < 257; i++) {
    std::map<unsigned, BitString>::const_iterator it = theMap.find(i);
    if (it != theMap.end()) {
      bfw->writeByte(it->second.size());
      bfw->writeBitString(it->second);
    }
    else {
      bfw->writeByte(0);
    }
  }
}

void writeCompressedOutput(const char * inFile,
                           const char * outFile,
                           const std::map<unsigned, BitString> & theMap) {
  BitFileWriter bfw(outFile);
  writeHeader(&bfw, theMap);

  //WRITE YOUR CODE HERE!
  //open the input file for reading
  int ch;
  std::ifstream f;
  f.open(inFile);
  if (!f.is_open()) {
    std::cerr << "Failed to open the file!\n" << std::endl;
    exit(EXIT_FAILURE);
  }
  //You need to read the input file, lookup the characters in the map,
  //and write the proper bit string with the BitFileWriter
  while ((ch = f.get()) != EOF) {
    if (theMap.find(ch) == theMap.end()) {
      std::cerr << "Cannot find such a character!\n" << std::endl;
      exit(EXIT_FAILURE);
    }
    bfw.writeBitString(theMap.find(ch)->second);
  }
  //dont forget to lookup 256 for the EOF marker, and write it out.
  bfw.writeBitString(theMap.find(256)->second);
  //BitFileWriter will close the output file in its destructor
  //but you probably need to close your input file.
  f.close();
}

int main(int argc, char ** argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage: compress input output\n");
    return EXIT_FAILURE;
  }
  //WRITE YOUR CODE HERE
  //Implement main
  uint64_t * freq = readFrequencies(argv[1]);
  Node * myTree = buildTree(freq);
  delete[] freq;
  BitString b;
  std::map<unsigned, BitString> myMap;
  myTree->buildMap(b, myMap);
  writeCompressedOutput(argv[1], argv[2], myMap);
  delete myTree;
  //hint 1: most of the work is already done.
  //hint 2: you can look at the main from the previous tester for 90% of this

  return EXIT_SUCCESS;
}
