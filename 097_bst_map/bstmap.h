#ifndef __BSTMAP_H__
#define __BSTMAP_H__
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "map.h"
template<typename K, typename V>
class BstMap : public Map<K, V> {
  class Node {
   public:
    K key;
    V value;
    Node * left;
    Node * right;
    Node() {}
    Node(const K & k, const V & v) : key(k), value(v), left(NULL), right(NULL) {}
  };
  Node * root;

 public:
  //default constructor
  BstMap() : root(NULL) {}

  //copy constructor
  void BST_copy(Node * node) {
    if (node != NULL) {
      add(node->key, node->value);
      BST_copy(node->left);
      BST_copy(node->right);
    }
  }
  BstMap(const BstMap & rhs) : root(NULL) { BST_copy(rhs.root); }

  //assignment operator
  BstMap & operator=(const BstMap & rhs) {
    if (this != &rhs) {
      BstMap temp(rhs);
      std::swap(root, temp.root);
    }
    return *this;
  }
  //add a node
  virtual void add(const K & key, const V & value) {
    Node ** current = &root;
    while (*current != NULL) {
      if (key == (*current)->key) {
        (*current)->value = value;
        return;
      }
      else if (key < (*current)->key) {
        current = &((*current)->left);
      }
      else {
        current = &((*current)->right);
      }
    }
    *current = new Node(key, value);
  }

  //look up a key
  virtual const V & lookup(const K & key) const throw(std::invalid_argument) {
    Node * current = root;
    while (current != NULL) {
      if (key == current->key) {
        return current->value;
      }
      else if (key < current->key) {
        current = current->left;
      }
      else {
        current = current->right;
      }
    }
    throw std::invalid_argument("Cannot find the key!\n");
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
  Node * rm_helper(const K & key, Node * current) {
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
        /*std::swap(successor->key, current->key);
	  std::swap(successor->value, current->value);*/
        current->key = successor->key;
        current->value = successor->value;
        current->right = rm_helper(successor->key, current->right);
        /*successor = rm_helper(key, successor);*/
      }
      return current;
    }
  }
  virtual void remove(const K & key) {
    root = rm_helper(key, root);
    /*Node ** current = &root;
    while () {
      if ((*current)->left->key == key) {
        (*current)->left = rm_helper(key, (*current)->left);
        return;
      }
      else if ((*current)->right->key == key) {
        (*current)->right = rm_helper(key, (*current)->right);
        return;
      }
      else {
        if (key < (*current)->key) {
          (*current) = (*current)->left;
        }
        if (key > (*current)->key) {
          (*current) = (*current)->right;
        }
      }
      }*/
  }

  //destructor
  void destroy(Node * node) {
    if (node != NULL) {
      destroy(node->left);
      destroy(node->right);
      delete node;
    }
  }
  virtual ~BstMap<K, V>() { destroy(root); }

  //test function
  void print_helper(Node * node) {
    if (node != NULL) {
      print_helper(node->left);
      std::cout << node->key << " " << node->value << std::endl;
      print_helper(node->right);
    }
  }

  void printInOrder() { print_helper(root); }
};
#endif
