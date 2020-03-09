/**
 * @file binarytree.cpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file.
 */
#include "TreeTraversals/InorderTraversal.h"
#include <iostream>

/**
 * @return The height of the binary tree. Recall that the height of a binary
 *  tree is just the length of the longest path from the root to a leaf, and
 *  that the height of an empty tree is -1.
 */
template <typename T>
int BinaryTree<T>::height() const
{
    // Call recursive helper function on root
    return height(root);
}

/**
 * Private helper function for the public height function.
 * @param subRoot
 * @return The height of the subtree
 */
template <typename T>
int BinaryTree<T>::height(const Node *subRoot) const
{
    // Base case
    if (subRoot == NULL)
        return -1;

    // Recursive definition
    return 1 + max(height(subRoot->left), height(subRoot->right));
}

/**
 * Prints out the values of the nodes of a binary tree in order.
 * That is, everything to the left of a node will be printed out before that
 * node itself, and everything to the right of a node will be printed out after
 * that node.
 */
template <typename T>
void BinaryTree<T>::printLeftToRight() const
{
    // Call recursive helper function on the root
    printLeftToRight(root);

    // Finish the line
    cout << endl;
}

/**
 * Private helper function for the public printLeftToRight function.
 * @param subRoot
 */
template <typename T>
void BinaryTree<T>::printLeftToRight(const Node *subRoot) const
{
    // Base case - null node
    if (subRoot == NULL)
        return;

    // Print left subtree
    printLeftToRight(subRoot->left);

    // Print this node
    cout << subRoot->elem << ' ';

    // Print right subtree
    printLeftToRight(subRoot->right);
}

/**
 * Flips the tree over a vertical axis, modifying the tree itself
 *  (not creating a flipped copy).
 */
template <typename T>
void BinaryTree<T>::mirror()
{
    mirror(root);
}

template <typename T>
void BinaryTree<T>::mirror(Node *subRoot)
{
    if (subRoot == NULL)
    {
        return;
    }
    Node *temp = subRoot->left;
    subRoot->left = subRoot->right;
    subRoot->right = temp;
    mirror(subRoot->left);
    mirror(subRoot->right);
}

/**
 * isOrdered() function iterative version
 * @return True if an in-order traversal of the tree would produce a
 *  nondecreasing list output values, and false otherwise. This is also the
 *  criterion for a binary tree to be a binary search tree.
 */
template <typename T>
bool BinaryTree<T>::isOrderedIterative() const
{
    stack<T> st;
    T temp;
    InorderTraversal<T> tree(this->getRoot());
    for (typename TreeTraversal<T>::Iterator it = tree.begin(); it != tree.end(); ++it)
    {
        st.push((*it)->elem);
    }
    if (st.empty() != true)
    {
        temp = st.top();
        st.pop();
    }
    while (st.empty() != true)
    {
        if (temp < st.top())
        {
            return false;
        }
        temp = st.top();
        st.pop();
    }
    return true;
}

/**
 * isOrdered() function recursive version
 * @return True if an in-order traversal of the tree would produce a
 *  nondecreasing list output values, and false otherwise. This is also the
 *  criterion for a binary tree to be a binary search tree.
 */
template <typename T>
bool BinaryTree<T>::isOrderedRecursive() const
{
    return isOrderedRecursive(root);
}

template <typename T>
bool BinaryTree<T>::isOrderedRecursive(Node *subRoot) const
{
    if (subRoot == NULL)
    {
        return true;
    }
    if (subRoot->left == NULL && subRoot->right == NULL)
    {
        return true;
    }
    bool flag = isOrderedRecursive(subRoot->left) && isOrderedRecursive(subRoot->right);
    if (flag == false)
    {
        return false;
    }
    if (subRoot->left == NULL)
    {
        if (subRoot->elem < maxVal(subRoot->right))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    if (subRoot->right == NULL)
    {
        if (subRoot->elem > maxVal(subRoot->left))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    if (subRoot->elem > maxVal(subRoot->left) && subRoot->elem < maxVal(subRoot->right))
    {
        return true;
    }
    else
    {
        return false;
    }
}

template <typename T>
T BinaryTree<T>::maxVal(Node *subRoot) const
{
    InorderTraversal<T> tree(subRoot);
    T maxval = 0;
    for (typename TreeTraversal<T>::Iterator it = tree.begin(); it != tree.end(); ++it)
    {
        if ((*it)->elem > maxval)
        {
            maxval = (*it)->elem;
        }
    }
    return maxval;
}
/**
 * creates vectors of all the possible paths from the root of the tree to any leaf
 * node and adds it to another vector.
 * Path is, all sequences starting at the root node and continuing
 * downwards, ending at a leaf node. Paths ending in a left node should be
 * added before paths ending in a node further to the right.
 * @param paths vector of vectors that contains path of nodes
 */
template <typename T>
void BinaryTree<T>::getPaths(vector<vector<T>> &paths) const
{
    vector<T> pathVector;
    getPaths(paths, pathVector, root);
}

template <typename T>
void BinaryTree<T>::getPaths(vector<vector<T>> &paths, vector<T> pathVector, Node *subRoot) const
{
    if (subRoot == NULL)
    {
        return;
    }
    if (subRoot->left == NULL && subRoot->right == NULL)
    {
        pathVector.push_back(subRoot->elem);
        paths.push_back(pathVector);
        return;
    }
    pathVector.push_back(subRoot->elem);
    getPaths(paths, pathVector, subRoot->left);
    getPaths(paths, pathVector, subRoot->right);
}

/**
 * Each node in a tree has a distance from the root node - the depth of that
 * node, or the number of edges along the path from that node to the root. This
 * function returns the sum of the distances of all nodes to the root node (the
 * sum of the depths of all the nodes). Your solution should take O(n) time,
 * where n is the number of nodes in the tree.
 * @return The sum of the distances of all nodes to the root
 */
template <typename T>
int BinaryTree<T>::sumDistances() const
{
    if (root == NULL)
    {
        return -1;
    }
    return sumDistances(root, 0);
}

template <typename T>
int BinaryTree<T>::sumDistances(Node *subRoot, int level) const
{
    if (subRoot->left == NULL && subRoot->right == NULL)
    {
        return level;
    }
    if (subRoot->right == NULL)
    {
        return level + sumDistances(subRoot->left, level + 1);
    }
    if (subRoot->left == NULL)
    {
        return level + sumDistances(subRoot->right, level + 1);
    }
    return level + sumDistances(subRoot->left, level + 1) + sumDistances(subRoot->right, level + 1);
}