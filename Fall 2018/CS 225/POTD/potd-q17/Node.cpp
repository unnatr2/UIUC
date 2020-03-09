#include "Node.h"

using namespace std;

void sortList(Node **head)
{
    Node *curr = *head;
    int count = 0;
    while (curr != NULL)
    {
        curr = curr->next_;
        count++;
    }
    for (int i = count; i > 1; i--)
    {
        Node *temp, *swap1;
        swap1 = *head;
        for (int j = 0; j < count - 1; j++)
        {
            if (swap1->data_ > swap1->next_->data_)
            {
                Node *swap2 = swap1->next_;
                swap1->next_ = swap2->next_;
                swap2->next_ = swap1;
                if (swap1 == *head)
                {
                    *head = swap2;
                    swap1 = swap2;
                }
                else
                {
                    swap1 = swap2;
                    temp->next_ = swap2;
                }
            }
            temp = swap1;
            swap1 = swap1->next_;
        }
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
