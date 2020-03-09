#include "Node.h"
#include <iostream>
using namespace std;

Node *listUnion(Node *first, Node *second)
{
    Node *result = NULL;
    Node *firstPointer = first;
    Node *secondPointer = second;
    while (firstPointer != NULL)
    {
        Node *temp = result;
        int flag = 0;
        while (temp != NULL)
        {
            if (temp->data_ == firstPointer->data_)
            {
                flag = 1;
            }
            temp = temp->next_;
        }
        if (flag == 0)
        {
            Node *newNode = new Node();
            newNode->data_ = firstPointer->data_;
            newNode->next_ = result;
            result = newNode;
        }
        firstPointer = firstPointer->next_;
    }
    while (secondPointer != NULL)
    {
        Node *temp = result;
        int flag = 0;
        while (temp != NULL)
        {
            if (temp->data_ == secondPointer->data_)
            {
                flag = 1;
            }
            temp = temp->next_;
        }
        if (flag == 0)
        {
            Node *newNode = new Node();
            newNode->data_ = secondPointer->data_;
            newNode->next_ = result;
            result = newNode;
        }
        secondPointer = secondPointer->next_;
    }
    return result;
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
