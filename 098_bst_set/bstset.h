#ifndef __BSTSET_H__
#define __BSTSET_H__
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "set.h"

/*template<typename T>
class Set {
public:
  virtual void add(const T & key) = 0;
  virtual bool contains(const T& key) const = 0;
  virtual void remove(const T& key) = 0;
  virtual ~Set<T>() {}
};

*/
template<typename T>
class BstSet : public Set<T> {
  class Node {
   public:
    T key;
    Node * left;
    Node * right;
    Node() {}
    Node(const T & t) : key(t), left(NULL), right(NULL) {}
  };
  Node * root;

 public:
  //default constructor
  BstSet() : root(NULL) {}

  //copy constructor
  void BST_copy(Node * node) {
    if (node != NULL) {
      add(node->key);
      BST_copy(node->left);
      BST_copy(node->right);
    }
  }
  BstSet(const BstSet & rhs) : root(NULL) { BST_copy(rhs.root); }

  //assignment operator
  BstSet & operator=(const BstSet & rhs) {
    if (this != &rhs) {
      BstSet temp(rhs);
      std::swap(root, temp.root);
    }
    return *this;
  }
  //add a node
  virtual void add(const T & key) throw(std::invalid_argument) {
    Node ** current = &root;
    while (*current != NULL) {
      if (key == (*current)->key) {
        throw std::invalid_argument("The key already exists!\n");
      }
      else if (key < (*current)->key) {
        current = &((*current)->left);
      }
      else {
        current = &((*current)->right);
      }
    }
    *current = new Node(key);
  }

  //look up a key
  virtual bool contains(const T & key) const {
    Node * current = root;
    while (current != NULL) {
      if (key == current->key) {
        return true;
      }
      else if (key < current->key) {
        current = current->left;
      }
      else {
        current = current->right;
      }
    }
    return false;
  }

  // remove a key
  Node * find_succ(Node * root) {
    if (root == NULL) {
      return NULL;
    }
    root = root->right;
    while (root->left != NULL) {
      root = root->left;
    }
    return root;
  }
  Node * rm_helper(const T & key, Node * current) {
    if (current == NULL) {
      return NULL;
    }

    if (key < current->key) {
      current->left = rm_helper(key, current->left);
      return current;
    }
    else if (key > current->key) {
      current->right = rm_helper(key, current->right);
      return current;
    }
    else {
      if (current->right == NULL) {  //the tree only have the left child
        Node * temp = current->left;
        delete current;
        current = temp;
      }
      else if (current->left == NULL) {  //the tree only have the right child
        Node * temp = current->right;
        delete current;
        current = temp;
      }
      else {  //the tree has two children
        Node * successor = find_succ(current);

        current->key = successor->key;
        current->right = rm_helper(successor->key, current->right);
      }
      return current;
    }
  }
  virtual void remove(const T & key) { root = rm_helper(key, root); }

  //destructor
  void destroy(Node * node) {
    if (node != NULL) {
      destroy(node->left);
      destroy(node->right);
      delete node;
    }
  }
  virtual ~BstSet<T>() { destroy(root); }

  //test function
  void print_helper(Node * node) {
    if (node != NULL) {
      print_helper(node->left);
      std::cout << node->key << std::endl;
      print_helper(node->right);
    }
  }

  void printInOrder() { print_helper(root); }
};
#endif
