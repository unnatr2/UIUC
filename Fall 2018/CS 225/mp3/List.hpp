/**
 * @file list.cpp
 * Doubly Linked List (MP 3).
 */

/**
 * Returns a ListIterator with a position at the beginning of
 * the List.
 */
template <typename T>
typename List<T>::ListIterator List<T>::begin() const
{
	// @TODO: graded in MP3.1
	return List<T>::ListIterator(head_);
}

/**
 * Returns a ListIterator one past the end of the List.
 */
template <typename T>
typename List<T>::ListIterator List<T>::end() const
{
	// @TODO: graded in MP3.1
	return List<T>::ListIterator(NULL);
}

/**
 * Destroys the current List. This function should ensure that
 * memory does not leak on destruction of a list.
 */
template <typename T>
List<T>::~List()
{
	/// @todo Graded in MP3.1
	_destroy();
}

/**
 * Destroys all dynamically allocated memory associated with the current
 * List class.
 */
template <typename T>
void List<T>::_destroy()
{
	/// @todo Graded in MP3.1
	ListNode *temp = tail_;
	while (temp != NULL)
	{
		if (temp->prev != NULL)
		{
			temp = temp->prev;
			delete temp->next;
		}
		else
		{
			delete temp;
			temp = NULL;
		}
	}
	head_ = NULL;
	tail_ = NULL;
	length_ = 0;
}

/**
 * Inserts a new node at the front of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
template <typename T>
void List<T>::insertFront(T const &ndata)
{
	/// @todo Graded in MP3.1
	ListNode *temp = new ListNode(ndata);
	if (head_ != NULL)
	{
		temp->next = head_;
		head_->prev = temp;
		head_ = temp;
	}
	else
	{
		head_ = temp;
		tail_ = head_;
	}
	length_++;
	temp = NULL;
}

/**
 * Inserts a new node at the back of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
template <typename T>
void List<T>::insertBack(const T &ndata)
{
	/// @todo Graded in MP3.1
	ListNode *temp = new ListNode(ndata);
	if (tail_ != NULL)
	{
		tail_->next = temp;
		temp->prev = tail_;
		tail_ = temp;
	}
	else
	{
		head_ = temp;
		tail_ = head_;
	}
	length_++;
	temp = NULL;
}

/**
 * Reverses the current List.
 */
template <typename T>
void List<T>::reverse()
{
	reverse(head_, tail_);
}

/**
 * Helper function to reverse a sequence of linked memory inside a List,
 * starting at startPoint and ending at endPoint. You are responsible for
 * updating startPoint and endPoint to point to the new starting and ending
 * points of the rearranged sequence of linked memory in question.
 *
 * @param startPoint A pointer reference to the first node in the sequence
 *  to be reversed.
 * @param endPoint A pointer reference to the last node in the sequence to
 *  be reversed.
 */
template <typename T>
void List<T>::reverse(ListNode *&startPoint, ListNode *&endPoint)
{
	/// @todo Graded in MP3.1
	if (startPoint == NULL && endPoint == NULL)
	{
		return;
	}
	ListNode *temp = startPoint;
	ListNode *tempNext = temp->next;
	while (temp != endPoint)
	{
		temp->next = temp->prev;
		temp->prev = tempNext;
		temp = tempNext;
		tempNext = tempNext->next;
	}
	temp->next = temp->prev;
	temp->prev = tempNext;
	ListNode *saveStartPointNext = startPoint->next;
	ListNode *saveEndPointPrev = endPoint->prev;
	if (saveStartPointNext != NULL && saveEndPointPrev != NULL)
	{
		saveStartPointNext->next = endPoint;
		endPoint->prev = saveStartPointNext;
		saveEndPointPrev->prev = startPoint;
		startPoint->next = saveEndPointPrev;
	}
	else if (saveStartPointNext == NULL && saveEndPointPrev != NULL)
	{
		saveEndPointPrev->prev = startPoint;
		startPoint->next = saveEndPointPrev;
		endPoint->prev = NULL;
	}
	else if (saveStartPointNext != NULL && saveEndPointPrev == NULL)
	{
		saveStartPointNext->next = endPoint;
		endPoint->prev = saveStartPointNext;
		startPoint->next = NULL;
	}
	ListNode *nodeSave = startPoint;
	startPoint = endPoint;
	endPoint = nodeSave;
}

/**
 * Reverses blocks of size n in the current List. You should use your
 * reverse( ListNode * &, ListNode * & ) helper function in this method!
 *
 * @param n The size of the blocks in the List to be reversed.
 */
template <typename T>
void List<T>::reverseNth(int n)
{
	/// @todo Graded in MP3.1
	if (n <= 1 || length_ <= 1)
	{
		return;
	}
	if (length_ == n)
	{
		reverse(head_, tail_);
		return;
	}
	ListNode *endPoint = head_;
	ListNode *startPoint = head_;
	int count = 0;
	int checkStart = 1;
	while (endPoint != NULL && count < length_)
	{
		for (int i = 1; i < n; i++)
		{
			if (endPoint->next == NULL)
			{
				break;
			}
			endPoint = endPoint->next;
		}
		reverse(startPoint, endPoint);
		if (checkStart == 1)
		{
			head_ = startPoint;
		}
		checkStart = 0;
		if (endPoint->next == NULL)
		{
			tail_ = endPoint;
		}
		endPoint = endPoint->next;
		startPoint = endPoint;
		count += n;
	}
	startPoint = NULL;
	endPoint = NULL;
}

/**
 * Modifies the List using the waterfall algorithm.
 * Every other node (starting from the second one) is removed from the
 * List, but appended at the back, becoming the new tail. This continues
 * until the next thing to be removed is either the tail (**not necessarily
 * the original tail!**) or NULL.  You may **NOT** allocate new ListNodes.
 * Note that since the tail should be continuously updated, some nodes will
 * be moved more than once.
 */
template <typename T>
void List<T>::waterfall()
{
	/// @todo Graded in MP3.1
	if (head_ == NULL || head_ == tail_ || head_->next == tail_)
	{
		return;
	}
	ListNode *nodePointer = head_;
	ListNode *nodeShift = nodePointer->next;
	while (nodePointer->next != tail_)
	{
		nodePointer->next = nodeShift->next;
		nodeShift->next->prev = nodePointer;
		nodeShift->prev = tail_;
		nodeShift->next = NULL;
		tail_->next = nodeShift;
		tail_ = nodeShift;
		nodePointer = nodePointer->next;
		nodeShift = nodePointer->next;
	}
}

/**
 * Splits the given list into two parts by dividing it at the splitPoint.
 *
 * @param splitPoint Point at which the list should be split into two.
 * @return The second list created from the split.
 */
template <typename T>
List<T> List<T>::split(int splitPoint)
{
	if (splitPoint > length_)
		return List<T>();

	if (splitPoint < 0)
		splitPoint = 0;

	ListNode *secondHead = split(head_, splitPoint);

	int oldLength = length_;
	if (secondHead == head_)
	{
		// current list is going to be empty
		head_ = NULL;
		tail_ = NULL;
		length_ = 0;
	}
	else
	{
		// set up current list
		tail_ = head_;
		while (tail_->next != NULL)
			tail_ = tail_->next;
		length_ = splitPoint;
	}

	// set up the returned list
	List<T> ret;
	ret.head_ = secondHead;
	ret.tail_ = secondHead;
	if (ret.tail_ != NULL)
	{
		while (ret.tail_->next != NULL)
			ret.tail_ = ret.tail_->next;
	}
	ret.length_ = oldLength - splitPoint;
	return ret;
}

/**
 * Helper function to split a sequence of linked memory at the node
 * splitPoint steps **after** start. In other words, it should disconnect
 * the sequence of linked memory after the given number of nodes, and
 * return a pointer to the starting node of the new sequence of linked
 * memory.
 *
 * This function **SHOULD NOT** create **ANY** new List or ListNode objects!
 *
 * @param start The node to start from.
 * @param splitPoint The number of steps to walk before splitting.
 * @return The starting node of the sequence that was split off.
 */
template <typename T>
typename List<T>::ListNode *List<T>::split(ListNode *start, int splitPoint)
{
	/// @todo Graded in MP3.2
	ListNode *pointer1 = start;
	ListNode *pointer2 = start;
	int count = 1;
	while (count != splitPoint)
	{
		pointer1 = pointer1->next;
		count++;
	}
	pointer2 = pointer1->next;
	pointer1->next = NULL;
	pointer2->prev = NULL;

	return pointer2;
}

/**
 * Merges the given sorted list into the current sorted list.
 *
 * @param otherList List to be merged into the current list.
 */
template <typename T>
void List<T>::mergeWith(List<T> &otherList)
{
	// set up the current list
	head_ = merge(head_, otherList.head_);
	tail_ = head_;

	// make sure there is a node in the new list
	if (tail_ != NULL)
	{
		while (tail_->next != NULL)
			tail_ = tail_->next;
	}
	length_ = length_ + otherList.length_;

	// empty out the parameter list
	otherList.head_ = NULL;
	otherList.tail_ = NULL;
	otherList.length_ = 0;
}

/**
 * Helper function to merge two **sorted** and **independent** sequences of
 * linked memory. The result should be a single sequence that is itself
 * sorted.
 *
 * This function **SHOULD NOT** create **ANY** new List objects.
 *
 * @param first The starting node of the first sequence.
 * @param second The starting node of the second sequence.
 * @return The starting node of the resulting, sorted sequence.
 */
template <typename T>
typename List<T>::ListNode *List<T>::merge(ListNode *first, ListNode *second)
{
	/// @todo Graded in MP3.2
	ListNode *firstPointer = first;
	ListNode *secondPointer = second;
	if (first == NULL)
	{
		return second;
	}
	if (second == NULL)
	{
		return first;
	}
	if (secondPointer->data < firstPointer->data)
	{
		first = secondPointer;
		secondPointer->prev = NULL;
		firstPointer->prev = secondPointer;
		second = secondPointer->next;
		if (second != NULL)
		{
			second->prev = NULL;
		}
		secondPointer->next = firstPointer;
		secondPointer = second;
	}
	while (second != NULL)
	{
		if (secondPointer->data < firstPointer->data)
		{
			second = secondPointer->next;
			firstPointer->prev->next = secondPointer;
			secondPointer->prev = firstPointer->prev;
			firstPointer->prev = secondPointer;
			secondPointer->next = firstPointer;
			secondPointer = second;
		}
		else if (firstPointer->next != NULL)
		{
			firstPointer = firstPointer->next;
		}
		else
		{
			firstPointer->next = secondPointer;
			secondPointer->prev = firstPointer;
			second = NULL;
		}
	}
	return first;
}

/**
 * Sorts the current list by applying the Mergesort algorithm.
 */
template <typename T>
void List<T>::sort()
{
	if (empty())
		return;
	head_ = mergesort(head_, length_);
	tail_ = head_;
	while (tail_->next != NULL)
		tail_ = tail_->next;
}

/**
 * Sorts a chain of linked memory given a start node and a size.
 * This is the recursive helper for the Mergesort algorithm (i.e., this is
 * the divide-and-conquer step).
 *
 * @param start Starting point of the chain.
 * @param chainLength Size of the chain to be sorted.
 * @return A pointer to the beginning of the now sorted chain.
 */
template <typename T>
typename List<T>::ListNode *List<T>::mergesort(ListNode *start, int chainLength)
{
	/// @todo Graded in MP3.2
	if (chainLength == 1 || chainLength == 0)
	{
		return start;
	}
	int midPoint = chainLength - (chainLength / 2);
	ListNode *mid = split(start, chainLength / 2);
	return merge(mergesort(start, chainLength / 2), mergesort(mid, midPoint));
}
