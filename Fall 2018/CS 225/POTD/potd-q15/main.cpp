#include "potd.h"
#include <iostream>
using namespace std;

int main()
{
  // Test 1: An empty list
  Node *head = NULL;
  cout << stringList(head) << endl;

  // Test 2: A list with exactly one node
  Node *head1 = new Node;
  head1->next_ = NULL;
  head1->data_ = 100;
  cout << stringList(head1) << endl;

  // Test 3: A list with more than one node
  Node *head2 = new Node;
  Node *nextnode = new Node;
  nextnode->next_ = NULL;
  nextnode->data_ = 200;
  head2->next_ = nextnode;
  head2->data_ = 100;

  cout << stringList(head2) << endl;

  return 0;
}
