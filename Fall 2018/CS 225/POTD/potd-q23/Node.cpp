#include "Node.h"
#include <iostream>

using namespace std;
void printList1(Node *head)
{
    if (head == NULL)
    {
        cout << "Empty list" << endl;
        return;
    }

    Node *temp = head;
    int count = 0;
    while (temp != NULL)
    {
        cout << "Node " << count << ": " << temp->data_ << endl;
        count++;
        temp = temp->next_;
    }
}

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

void removeDuplicates(Node *start)
{
    Node *startPointer = start;
    while (startPointer != NULL && startPointer->next_ != NULL)
    {
        Node *tempPointer = startPointer->next_;
        while (tempPointer)
        {
            if (tempPointer->data_ == startPointer->data_)
            {
                // cout << startPointer->data_ << endl;
                // cout << tempPointer->data_ << endl;
                // Node *temp = tempPointer;
                startPointer->next_ = tempPointer->next_;
                // printList1(start);
                // delete temp;
            }
            tempPointer = tempPointer->next_;
        }
        startPointer = startPointer->next_;
    }
}

Node *listSymmetricDifference(Node *first, Node *second)
{
    removeDuplicates(first);
    removeDuplicates(second);
    Node *unionList = listUnion(first, second);
    // cout << "Union:" << endl;
    // printList1(unionList);
    Node *intersectionList = listIntersection(first, second);
    // cout << "Intersection:" << endl;
    // printList1(intersectionList);
    Node *intPointer = intersectionList;
    while (intPointer)
    {
        Node *unionPointer = unionList;
        Node *prev;
        if (unionPointer->data_ == intPointer->data_)
        {
            unionList = unionPointer->next_;
            delete unionPointer;
            if (unionList == NULL)
            {
                return NULL;
            }
        }
        while (unionPointer && unionPointer->data_ != intPointer->data_)
        {
            prev = unionPointer;
            unionPointer = unionPointer->next_;
        }
        prev->next_ = unionPointer->next_;
        delete unionPointer;
        intPointer = intPointer->next_;
    }
    return unionList;
    // cout << "Test:" << endl;
    // printList1(second);
    // cout << "Test End:" << endl;
    // Node *result = NULL;
    // Node *firstPointer = first;
    // while (firstPointer)
    // {
    //     Node *newNode = new Node();
    //     newNode->data_ = firstPointer->data_;
    //     newNode->next_ = result;
    //     result = newNode;
    //     firstPointer = firstPointer->next_;
    // }
    // // cout << "Result:" << endl;
    // // printList1(result);
    // Node *secondPointer = second;
    // while (secondPointer)
    // {
    //     int flag = 0;
    //     Node *resultPointer = result;
    //     while (resultPointer)
    //     {
    //         if (secondPointer->data_ == resultPointer->data_)
    //         {
    //             flag = 1;
    //         }
    //         resultPointer = resultPointer->next_;
    //     }
    //     if (flag == 1)
    //     {
    //         deleteKey(result, secondPointer->data_);
    //     }
    //     if (flag == 0)
    //     {
    //         Node *newNode = new Node();
    //         newNode->data_ = secondPointer->data_;
    //         newNode->next_ = result;
    //         result = newNode;
    //     }
    //     secondPointer = secondPointer->next_;
    // }
    // return result;
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
