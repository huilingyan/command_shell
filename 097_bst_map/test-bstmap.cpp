#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "bstmap.h"

int main(void) {
  BstMap<int, int> map;
  map.add(60, 7);
  map.add(19, 4);
  map.add(93, 11);
  map.add(4, 2);
  map.add(25, 5);
  map.add(84, 9);
  map.add(1, 1);
  map.add(11, 3);
  map.add(35, 6);
  map.add(70, 8);
  map.add(86, 10);
  map.add(93, 12);
  map.printInOrder();
  std::cout << "\n";
  int val1;
  val1 = map.lookup(11);
  std::cout << "The corresponding value = " << val1 << std::endl;

  /*  map.remove(93);
  map.printInOrder();
  std::cout << "\n";
  map.remove(25);
  map.printInOrder();
  std::cout << "\n";*/
  map.remove(4);
  map.printInOrder();
  std::cout << "\n";

  /*  BstMap<int, int> map2 = map;
  map.printInOrder();
  std::cout << "\n";*/

  return EXIT_SUCCESS;
}
