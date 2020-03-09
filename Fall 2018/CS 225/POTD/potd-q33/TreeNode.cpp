#include "TreeNode.h"
#include <algorithm>
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

void rebalance(TreeNode *&subtree)
{
    // your code here
    if (subtree == NULL)
    {
        return;
    }
    int balance = height(subtree->right_) - height(subtree->left_);
    if (subtree->left_ != NULL)
    {
        int leftBalance = height(subtree->left_->right_) - height(subtree->left_->left_);
        if (balance <= -2)
        {
            if (leftBalance == -1)
            {
                rightRotate(subtree);
            }
        }
    }
    if (subtree->right_ != NULL)
    {
        int rightBalance = height(subtree->right_->right_) - height(subtree->right_->left_);
        if (balance >= 2)
        {
            if (rightBalance == 1)
            {
                leftRotate(subtree);
            }
        }
    }
}

void rightRotate(TreeNode *root)
{
    TreeNode *parent = root->parent_;
    TreeNode *middle = root->left_;
    if (parent != NULL)
    {
        if (parent->left_->val_ == root->val_)
        {
            parent->left_ = middle;
        }
        else
        {
            parent->right_ = middle;
        }
    }
    middle->parent_ = parent;
    root->parent_ = middle;
    root->left_ = middle->right_;
    if (middle->right_ != NULL)
    {
        middle->right_->parent_ = root;
    }
    middle->right_ = root;
    root = middle;
    if (isHeightBalanced(root) == false)
    {
        TreeNode *temp = findLastUnbalanced(root);
        rebalance(temp);
    }
}

void leftRotate(TreeNode *root)
{
    TreeNode *parent = root->parent_;
    TreeNode *middle = root->right_;
    if (parent != NULL)
    {
        if (parent->left_->val_ == root->val_)
        {
            parent->left_ = middle;
        }
        else
        {
            parent->right_ = middle;
        }
    }
    middle->parent_ = parent;
    root->parent_ = middle;
    root->right_ = middle->left_;
    if (middle->left_ != NULL)
    {
        middle->left_->parent_ = root;
    }
    middle->left_ = root;
    root = middle;
    if (isHeightBalanced(root) == false)
    {
        TreeNode *temp = findLastUnbalanced(root);
        rebalance(temp);
    }
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
