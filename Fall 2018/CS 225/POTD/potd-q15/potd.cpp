#include "potd.h"
#include <iostream>

using namespace std;

string stringList(Node *head)
{
    if (head == NULL)
    {
        return "Empty list";
    }
    Node *nodeptr = head;
    int count = 0;
    string output;
    while (nodeptr->next_ != NULL)
    {
        output += "Node " + to_string(count) + ": " + to_string(nodeptr->data_) + " -> ";
        count++;
        nodeptr = nodeptr->next_;
    }
    output += "Node " + to_string(count) + ": " + to_string(nodeptr->data_);
    return output;
}
