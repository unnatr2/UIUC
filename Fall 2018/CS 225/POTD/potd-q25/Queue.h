#ifndef _QUEUE_H
#define _QUEUE_H

#include <cstddef>

class Queue
{
  class Node
  {
  public:
    Node(int val) : next(NULL), value(val){};
    Node *next;
    int value;
  };

public:
  Queue();
  int size() const;
  bool isEmpty() const;
  void enqueue(int value);
  int dequeue();

private:
  Node *head;
  Node *tail;
  int _size;
};

#endif
