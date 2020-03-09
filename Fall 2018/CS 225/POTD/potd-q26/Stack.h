#ifndef _STACK_H
#define _STACK_H

#include <cstddef>

class Stack
{
  class Node
  {
  public:
    Node(int val) : next(NULL), value(val){};
    Node *next;
    int value;
  };

public:
  Stack();
  int size() const;
  bool isEmpty() const;
  void push(int value);
  int pop();

private:
  Node *top;
  int _size;
};

#endif
