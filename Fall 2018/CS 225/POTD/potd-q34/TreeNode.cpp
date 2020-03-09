#include "TreeNode.h"

TreeNode::RotationType balanceTree(TreeNode *&subroot)
{
  int balance = getHeight(subroot->right_) - getHeight(subroot->left_);
  if (subroot->left_ != NULL)
  {
    int left_Balance = getHeight(subroot->left_->right_) - getHeight(subroot->left_->left_);
    if (balance <= -2)
    {
      if (left_Balance == 1)
      {
        return TreeNode::leftRight;
      }
      else if (left_Balance == -1)
      {
        return TreeNode::right;
      }
    }
  }
  if (subroot->right_ != NULL)
  {
    int right_Balance = getHeight(subroot->right_->right_) - getHeight(subroot->right_->left_);
    if (balance >= 2)
    {
      if (right_Balance == -1)
      {
        return TreeNode::rightLeft;
      }
      else if (right_Balance == 1)
      {
        return TreeNode::left;
      }
    }
  }
}
