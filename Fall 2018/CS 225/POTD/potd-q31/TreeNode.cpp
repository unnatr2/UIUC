#include "TreeNode.h"
#include <iostream>

int height(TreeNode *node)
{
  // if (node->left_ == NULL && node->right_ == NULL)
  if (node == NULL)
  {
    return 0;
  }
  return 1 + std::max(height(node->left_), height(node->right_));
}

bool isHeightBalanced(TreeNode *root)
{
  if (root == NULL)
  {
    return true;
  }
  int left_ = height(root->left_);
  int right_ = height(root->right_);
  if (abs(left_ - right_) <= 1 && isHeightBalanced(root->left_) && isHeightBalanced(root->right_))
  {
    return true;
  }
  return false;
}

void deleteTree(TreeNode *root)
{
  if (root == NULL)
    return;
  deleteTree(root->left_);
  deleteTree(root->right_);
  delete root;
  root = NULL;
}
