#include "TreeNode.h"
#include <iostream>

int height(TreeNode *node)
{
  if (node == NULL)
  {
    return -1;
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

TreeNode *findLastUnbalanced(TreeNode *root)
{
  if (root == NULL)
  {
    return NULL;
  }
  TreeNode *left = findLastUnbalanced(root->left_);
  TreeNode *right = findLastUnbalanced(root->right_);
  if (left == NULL && right == NULL)
  {
    if (abs(height(root->left_) - height(root->right_)) > 1)
    {
      return root;
    }
    return NULL;
  }
  if (left != NULL && right == NULL)
  {
    return left;
  }
  if (left == NULL && right != NULL)
  {
    return right;
  }
  return NULL;
}

// TreeNode *findLastUnbalanced(TreeNode *root)
// {
//   if (root == NULL)
//   {
//     return NULL;
//   }
//   findLastUnbalanced(root->left_);
//   if (!isHeightBalanced(root))
//   {
//     return root;
//   }
//   findLastUnbalanced(root->right_);
//   return NULL;
// }

void deleteTree(TreeNode *root)
{
  if (root == NULL)
    return;
  deleteTree(root->left_);
  deleteTree(root->right_);
  delete root;
  root = NULL;
}
