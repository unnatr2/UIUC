#include "potd.h"
#include <iostream>

using namespace std;

void insertSorted(Node **head, Node *insert)
{
	if (*head == NULL || (*head)->data_ >= insert->data_)
	{
		insert->next_ = *head;
		*head = insert;
	}
	else
	{
		Node *tempptr = *head;
		while (tempptr->next_ != NULL && tempptr->next_->data_ < insert->data_)
		{
			tempptr = tempptr->next_;
		}
		insert->next_ = tempptr->next_;
		tempptr->next_ = insert;
	}
}
