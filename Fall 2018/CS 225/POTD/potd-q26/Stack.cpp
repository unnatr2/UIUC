#include "Stack.h"

Stack::Stack()
{
  top = NULL;
  _size = 0;
}

// `int size()` - returns the number of elements in the stack (0 if empty)
int Stack::size() const
{
  return _size;
}

// `bool isEmpty()` - returns if the list has no elements, else false
bool Stack::isEmpty() const
{
  if (_size == 0)
  {
    return true;
  }
  return false;
}

// `void push(int val)` - pushes an item to the stack in O(1) time
void Stack::push(int value)
{
  Node *newNode = new Node(value);
  if (top == NULL)
  {
    top = newNode;
  }
  else
  {
    newNode->next = top;
    top = newNode;
  }
  _size++;
}

// `int pop()` - removes an item off the stack and returns the value in O(1) time
int Stack::pop()
{
  if (top == NULL)
  {
    return -1;
  }
  else
  {
    Node *temp = top;
    top = top->next;
    int data = temp->value;
    delete temp;
    _size--;
    return data;
  }
}
