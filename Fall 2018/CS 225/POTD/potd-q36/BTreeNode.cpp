#include <vector>
#include "BTreeNode.h"

void traverse(BTreeNode *subroot, std::vector<int> &v)
{
    unsigned i;
    for (i = 0; i < subroot->elements_.size(); i++)
    {
        if (subroot->is_leaf_ == false)
        {
            traverse(subroot->children_[i], v);
        }
        v.push_back(subroot->elements_[i]);
    }
    if (subroot->is_leaf_ == false)
    {
        traverse(subroot->children_[i], v);
    }
}

std::vector<int> traverse(BTreeNode *root)
{
    // your code here
    std::vector<int> v;
    traverse(root, v);
    return v;
}
