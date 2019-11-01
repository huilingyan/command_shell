#ifndef _LL_H_
#define _LL_H_
#include <assert.h>

#include <cstdlib>
#include <exception>

//YOUR CODE GOES HERE

class Error : public std::exception {
  virtual const char * what() const throw() { return "Index out of range!\n"; }
};

template<typename T>
class LinkedList {
  class Node {
   public:
    T data;
    Node * next;
    Node * prev;

    Node() : data(0), next(NULL), prev(NULL) {}
    Node(const T & item, Node * n, Node * p) : data(item), next(n), prev(p) {}
  };
  Node * head;
  Node * tail;
  int size;

 public:
  LinkedList() : head(NULL), tail(NULL), size(0) {}
  LinkedList(const LinkedList & rhs) : head(NULL), tail(NULL), size(0) {
    Node * temp = rhs.tail;
    while (temp != NULL) {
      addFront(temp->data);
      temp = temp->prev;
    }
  }
  LinkedList & operator=(const LinkedList & rhs) {
    /*if (this != &rhs) {
      while (head != NULL) {
        Node * temp = head->next;
        delete head;
        head = temp;
      }
      tail = NULL;
      size = 0;
      Node * current = rhs.tail;
      while (current != NULL) {
        this->addFront(current->data);
        current = current->prev;
      }
      size = rhs.size;
      }*/
    LinkedList temp(rhs);
    std::swap(head, rhs.head);
    std::swap(tail, rhs.tail);
    std::swap(size, rhs.size);
    return *this;
  }
  ~LinkedList() {
    while (head != NULL) {
      Node * temp = head->next;
      delete head;
      head = temp;
    }
    tail = NULL;
    size = 0;
  }
  void addFront(const T & item) {
    head = new Node(item, head, NULL);
    if (tail == NULL) {
      tail = head;
    }
    else {
      head->next->prev = head;
    }
    size++;
  }
  void addBack(const T & item) {
    tail = new Node(item, NULL, tail);
    if (head == NULL) {
      head = tail;
    }
    else {
      tail->prev->next = tail;
    }
    size++;
  }
  bool remove(const T & item) {
    Node * rm = head;
    //empty list
    if (rm == NULL) {
      return false;
    }
    else {
      while (rm != NULL) {
        if (item == rm->data) {
          // list of size 1, after removing, becomes a empty list
          if (head == tail) {
            head = NULL;
            tail = NULL;
            size = 0;
          }
          // the head needs to be removed
          else if (rm->prev == NULL) {
            head = rm->next;
            rm->next->prev = NULL;
          }
          // the tail needs to be removed
          else if (rm->next == NULL) {
            tail = rm->prev;
            rm->prev->next = NULL;
          }
          else {
            rm->prev->next = rm->next;
            rm->next->prev = rm->prev;
          }
          delete rm;
          size--;
          return true;
        }
        rm = rm->next;
      }
      return false;
    }
  }
  T & operator[](int index) {
    if ((index < 0) && (index >= size)) {
      throw Error();
    }
    else {
      Node * current = head;
      for (int i = 0; i < index; i++) {
        current = current->next;
      }
      return current->data;
    }
  }
  const T & operator[](int index) const {
    if ((index < 0) && (index >= size)) {
      throw Error();
    }
    else {
      Node * current = head;
      for (int i = 0; i < index; i++) {
        current = current->next;
      }
      return current->data;
    }
  }
  int find(const T & item) {
    int index = 0;
    Node * current = head;
    while (current != NULL) {
      if (current->data == item) {
        return index;
      }
      index++;
      current = current->next;
    }
    return -1;
  }
  int getSize() const { return size; }
};

#endif
