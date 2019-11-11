#include "node.h"

Node * buildTree(uint64_t * counts) {
  //WRITE ME!

  //construct the priority queue
  priority_queue_t pq;
  for (int i = 0; i < 257; i++) {
    pq.push(new Node(i, counts[i]));
  }
  //construct the huffman tree
  if (pq.size() == 0) {
    return NULL;
  }
  while (pq.size() > 1) {
    Node * l = pq.top();
    pq.pop();
    Node * r = pq.top();
    pq.pop();
    Node * comb = new Node(l, r);
    pq.push(comb);
  }

  return pq.top();
}
