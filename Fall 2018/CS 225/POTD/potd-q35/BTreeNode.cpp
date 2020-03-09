#include <vector>
#include "BTreeNode.h"

BTreeNode *find(BTreeNode *root, int key)
{
	unsigned i = 0;
	while (i < root->elements_.size() && key > root->elements_[i])
	{
		i++;
	}
	if (key == root->elements_[i])
	{
		return root;
	}
	if (root->is_leaf_ == true)
	{
		return NULL;
	}
	return find(root->children_[i], key);
}
