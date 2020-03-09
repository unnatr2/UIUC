#include "TreeNode.h"

#include <cstddef>
#include <iostream>
using namespace std;

TreeNode::TreeNode() : left_(NULL), right_(NULL) {}

int TreeNode::getHeight()
{
  TreeNode *current = this;
  if (current == NULL)
  {
    return -1;
  }
  else
  {
    int heightLeft = current->left_->getHeight();
    int heightRight = current->right_->getHeight();
    if (heightLeft > heightRight)
    {
      return heightLeft + 1;
    }
    else
    {
      return heightRight + 1;
    }
  }
  // return -1;
}
