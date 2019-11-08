#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "bstset.h"

int main(void) {
  BstSet<int> set;
  set.add(60);
  set.add(19);
  set.add(93);
  set.add(4);
  set.add(25);
  set.add(84);
  set.add(1);
  set.add(11);
  set.add(35);
  set.add(70);
  set.add(86);
  set.printInOrder();
  std::cout << "\n";

  bool ctn1;
  ctn1 = set.contains(11);
  std::cout << "The value is:  " << ctn1 << std::endl;

  bool ctn2;
  ctn2 = set.contains(15);
  std::cout << "The value is:  " << ctn2 << std::endl;

  BstSet<int> set2 = set;
  set.printInOrder();
  std::cout << "\n";

  /*set.remove(93);
  set.printInOrder();
  std::cout << "\n";*/
  set.remove(4);
  set.printInOrder();
  std::cout << "\n";
  /*set.remove(4);
  set.printInOrder();
  std::cout << "\n";*/

  return EXIT_SUCCESS;
}
