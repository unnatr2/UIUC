#include "Queue.h"

Queue::Queue()
{
	head = NULL;
	tail = NULL;
	_size = 0;
}

// `int size()` - returns the number of elements in the stack (0 if empty)
int Queue::size() const
{
	return _size;
}

// `bool isEmpty()` - returns if the list has no elements, else false
bool Queue::isEmpty() const
{
	if (_size == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// `void enqueue(int val)` - enqueue an item to the queue in O(1) time
void Queue::enqueue(int value)
{
	Node *newNode = new Node(value);
	if (head == NULL)
	{
		head = tail = newNode;
	}
	else
	{
		tail->next = newNode;
		tail = newNode;
	}
	_size++;
}

// `int dequeue()` - removes an item off the queue and returns the value in O(1) time
int Queue::dequeue()
{
	if (head == NULL)
	{
		return -1;
	}
	else
	{
		Node *temp = head;
		head = head->next;
		int data = temp->value;
		delete temp;
		_size--;
		return data;
	}
}
