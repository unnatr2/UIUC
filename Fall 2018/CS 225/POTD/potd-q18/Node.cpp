#include "Node.h"

using namespace std;

void mergeList(Node *first, Node *second)
{
    if (second == NULL)
    {
        return;
    }
    if (first == NULL && second != NULL)
    {
        first = second;
    }
    Node *tempFirst = first->next_;
    Node *tempSecond = second->next_;
    Node *nodePtr = second;
    first->next_ = nodePtr;
    while (tempFirst != NULL && tempSecond != NULL)
    {
        nodePtr->next_ = tempFirst;
        tempFirst = tempFirst->next_;
        nodePtr = nodePtr->next_;
        nodePtr->next_ = tempSecond;
        tempSecond = tempSecond->next_;
        nodePtr = nodePtr->next_;
    }
    if (tempFirst != NULL && tempSecond == NULL)
    {
        nodePtr->next_ = tempFirst;
    }
}

Node::Node()
{
    numNodes++;
}

Node::Node(Node &other)
{
    this->data_ = other.data_;
    this->next_ = other.next_;
    numNodes++;
}

Node::~Node()
{
    numNodes--;
}

int Node::numNodes = 0;
