#include "Node.h"

using namespace std;

Node *listIntersection(Node *first, Node *second)
{
    Node *result = NULL;
    Node *t1 = first;
    while (t1 != NULL)
    {
        int flag = 0;
        Node *t2 = second;
        while (t2 != NULL)
        {
            if (t2->data_ == t1->data_)
            {
                flag = 1;
            }
            t2 = t2->next_;
        }
        if (flag == 1)
        {
            if (result == NULL)
            {
                Node *newNode = new Node();
                newNode->data_ = t1->data_;
                newNode->next_ = NULL;
                result = newNode;
            }
            else
            {
                Node *newNode = new Node();
                newNode->data_ = t1->data_;
                newNode->next_ = result;
                result = newNode;
            }
        }
        t1 = t1->next_;
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
