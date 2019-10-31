#include <assert.h>

#include <cstdio>
#include <cstdlib>
#include <list>

#include "il.h"

void testList() {
  /*//empty list
  IntList lls1;
  assert((lls1.getSize() == 0) && (lls1.size == 0) && (lls1.tail == NULL) &&
         (lls1.head == NULL));
  lls1.addFront(1);
  assert((lls1.size == 1) && (lls1.getSize() == 1) && (lls1.head == lls1.tail) &&
         (lls1.head != NULL));
  lls1.addBack(2);
  assert((lls1.size == 2) && (lls1.getSize() == 2) && (lls1[0] == 1) && (lls1[1] == 2) &&
         (lls1.head->next->next == NULL) && (lls1.head == lls1.head->next->prev) &&
         (lls1.tail == lls1.head->next));
  //assignment operator
  IntList lls2;
  lls2 = lls1;
  assert((lls2[0] == 1) && (lls2[1] = 2));
  assert(lls2.getSize() == 2 && lls2.size == 2);
  assert(lls2.head->next->next == NULL);
  assert(lls2.head == lls2.head->next->prev);
  assert(lls2.tail == lls2.head->next);
  //copy constructor
  IntList lls3(lls1);
  assert((lls3[0] == 1) && (lls3[1] = 2));
  assert(lls3.getSize() == 2 && lls3.size == 2);
  assert(lls3.head->next->next == NULL);
  assert(lls3.head == lls3.head->next->prev);
  assert(lls3.tail == lls3.head->next);
  // Intlist lls1:224312 change to 24312
  lls1.addFront(3);
  lls1.addFront(4);
  lls1.addFront(2);
  lls1.addFront(2);
  assert(lls1[0] == 2 && lls1[1] == 2 && lls1[2] == 4);
  assert(lls1.size == 6 && lls1.getSize() == 6);
  bool x = lls1.remove(2);
  assert(lls1.size == 5 && lls1.getSize() == 5);
  assert(x == true);
  assert(lls1[0] == 2 && lls1[4] == 2);
  assert(lls1.head->data == 2);
  assert(lls1.head->next->data == 4);
  bool y = lls1.remove(10);
  assert(y == false);
  int f1 = lls1.find(2);
  assert(f1 == 0);
  int f2 = lls1.find(4);
  assert(f2 == 1);
  int f3 = lls1.find(9);
  assert(f3 == -1);*/
  IntList l1;
  assert(l1.getSize() == 0 && l1.size == 0 && l1.head == NULL && l1.tail == NULL);
  l1.addBack(1);
  assert(l1.getSize() == 1 && l1.size == 1 && l1.head == l1.tail && l1.head != NULL);
  l1.addFront(2);
  assert(l1.getSize() == 2 && l1.size == 2);
  assert(l1[0] == 2);
  assert(l1[1] == 1);
  assert(l1.head->next->next == NULL);
  assert(l1.head == l1.head->next->prev);
  assert(l1.tail == l1.head->next);
  //create Intlist l2 by assignment operator
  IntList l2;
  l2 = l1;
  assert(l2.getSize() == 2 && l2.size == 2);
  assert(l2[0] == 2);
  assert(l2[1] == 1);
  assert(l2.head->next->next == NULL);
  assert(l2.head == l2.head->next->prev);
  assert(l2.tail == l2.head->next);
  // create Intlist l3 by copy constructor
  IntList l3(l1);
  assert(l3.getSize() == 2 && l3.size == 2);
  assert(l3[0] == 2);
  assert(l3[1] == 1);
  assert(l3.head->next->next == NULL);
  assert(l3.head == l3.head->next->prev);
  assert(l3.tail == l3.head->next);
  // Intlist l1:224321 change to 24321
  l1.addFront(3);
  l1.addFront(4);
  l1.addFront(2);
  l1.addFront(2);
  assert(l1[0] == 2 && l1[1] == 2 && l1[2] == 4);
  assert(l1.size == 6 && l1.getSize() == 6);
  bool x = l1.remove(2);
  assert(l1.size == 5 && l1.getSize() == 5);
  assert(x == true);
  assert(l1[0] == 2 && l1[3] == 2);
  assert(l1.head->data == 2);
  assert(l1.head->next->data == 4);
  bool y = l1.remove(10);
  assert(y == false);
  int f1 = l1.find(2);
  assert(f1 == 0);
  /*int f2 = l1.find(4);
    assert(f2 == 1);*/
  int f3 = l1.find(9);
  assert(f3 == -1);
  /*  // Intlist l1:24321 change to 24321510
  l1.addBack(5);
  l1.addBack(10);
  assert(l1.size == 7 && l1.getSize() == 7);
  assert(l1[5] == 5 && l1[6] == 10);
  // remove l2
  l2.remove(2);
  l2.remove(1);
  assert(l2.size == 0 && l2.getSize() == 0 && l2.head == NULL && l2.tail == NULL);
  bool rm = l2.remove(2);
  assert(rm == false);
  // Inlist l3:21 change to 1
  l3.remove(2);
  assert(l3.getSize() == 1 && l3.size == 1 && l3.head == l3.tail && l3.head != NULL);
  assert(l3[0] == 1);
  assert(l3.head->next == NULL && l3.head->prev == NULL);
  */
  l3.~IntList();
}

int main(void) {
  testList();
  return EXIT_SUCCESS;
}
