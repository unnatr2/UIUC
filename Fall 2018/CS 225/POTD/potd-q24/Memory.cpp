#include "Memory.h"
#include <iostream>
#include <iomanip>
#include "Node.h"
#include <vector>
#include <utility>

using namespace std;

/**
Frees the given memory address. Returns if the free was successful or not
Be sure to merge adjacent free blocks when possible
*/
bool Memory::free(unsigned address)
{
    Node *current = head;
    while (current != NULL)
    {
        if (current->address == address)
        {
            current->inUse = false;
            if (current->previous != NULL && current->next != NULL && current->previous->inUse == false && current->next->inUse == false)
            {
                current->previous->next = current->next->next;
                if (current->next->next != NULL)
                {
                    current->next->next->previous = current->previous;
                }
                delete current;
            }
            else if (current->previous != NULL && current->previous->inUse == false)
            {
                current->previous->next = current->next;
                if (current->next != NULL)
                {
                    current->next->previous = current->previous;
                }
                delete current;
            }
            else if (current->next != NULL && current->next->inUse == false)
            {
                Node *temp = current->next;
                current->next = temp->next;
                if (temp->next != NULL)
                {
                    temp->next->previous = current;
                }
                delete temp;
            }
            // cout << getSize(current) << endl;
            return true;
        }
        current = current->next;
    }

    return false;
}

int areTheyNeighbours(Node *A, Node *B)
{
    return (A->next == B && B->previous == A) || (A->previous == B && B->next == A);
}

void refreshOuterPointers(Node *A)
{
    if (A->previous != NULL)
        A->previous->next = A;

    if (A->next != NULL)
        A->next->previous = A;
}

size_t getSize1(Node *node)
{
    Node *next = node->next;
    size_t thisSize;
    unsigned thisAddress = node->address;
    if (next == NULL)
    {
        thisSize = maxAddress - thisAddress;
    }
    else
    {
        thisSize = next->address - thisAddress;
    }
    return thisSize;
}

void swap(Node *A, Node *B)
{
    Node *swapperVector[4];
    Node *temp;

    if (A == B)
        return;

    if (B->next == A)
    {
        temp = A;
        A = B;
        B = temp;
    }

    swapperVector[0] = A->previous;
    swapperVector[1] = B->previous;
    swapperVector[2] = A->next;
    swapperVector[3] = B->next;

    if (areTheyNeighbours(A, B))
    {
        int temp = getSize1(B);
        B->address = A->address;
        A->address += temp;
        A->previous = swapperVector[2];
        B->previous = swapperVector[0];
        A->next = swapperVector[3];
        B->next = swapperVector[1];
    }
    else
    {
        A->previous = swapperVector[1];
        B->previous = swapperVector[0];
        A->next = swapperVector[3];
        B->next = swapperVector[2];
    }

    refreshOuterPointers(A);
    refreshOuterPointers(B);
}

/**
Reorganizes memory structure so that all of the allocated memory is grouped at the bottom (0x0000) and there is one large
chunk of free memory above.

Note: We do not care about the order of the allocated memory chunks
*/
void Memory::defragment()
{
    Node *current = head;
    while (current != NULL)
    {

        if (current->inUse)
        {
            // Do nothing
        }
        else
        {
            // TODO: Find the next chunk of allocated memory and shift it down to current's address
            // We recommend using the helper function `findNextAllocatedMemoryChunk`

            Node *tempptr = findNextAllocatedMemoryChunk(current);
            // Node *tempptrSave = tempptr;
            // cout << tempptr->address << endl;
            if (tempptr != NULL)
            {
                swap(current, tempptr);
                // current = tempptrSave;
            }
        }

        current = current->next;
    }
    // TODO: Finally merge all of the free blocks of memory together
    current = head;
    while (current != NULL)
    {
        if (current->inUse == false)
        {
            if (current->previous != NULL && current->next != NULL && current->previous->inUse == false && current->next->inUse == false)
            {
                Node *temp = current;
                current->previous->next = current->next->next;
                if (current->next->next != NULL)
                {
                    current->next->next->previous = current->previous;
                }
                current = current->next;
                delete temp;
            }
            else if (current->previous != NULL && current->previous->inUse == false)
            {
                Node *temp = current;
                current->previous->next = current->next;
                if (current->next != NULL)
                {
                    current->next->previous = current->previous;
                }
                current = current->next;
                delete temp;
            }
            else if (current->next != NULL && current->next->inUse == false)
            {
                Node *temp = current->next;
                current->next = temp->next;
                if (temp->next != NULL)
                {
                    temp->next->previous = current;
                }
                delete temp;
            }
        }
        current = current->next;
    }
}
