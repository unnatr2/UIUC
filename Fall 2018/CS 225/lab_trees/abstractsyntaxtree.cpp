#include "abstractsyntaxtree.h"

/**
 * Calculates the value from an AST (Abstract Syntax Tree). To parse numbers from strings, please use std::stod
 * @return A double representing the calculated value from the expression transformed into an AST
 */
double AbstractSyntaxTree::eval() const
{
    // @TODO Your code goes here...
    if (root != NULL)
        return eval(root);
    else
        return -1;
}
double AbstractSyntaxTree::eval(Node *subRoot) const
{
    if (subRoot == NULL)
    {
        return 0;
    }
    if (subRoot->left == NULL && subRoot->right == NULL)
    {
        return std::stod(subRoot->elem);
    }
    double leftValue = AbstractSyntaxTree::eval(subRoot->left);
    double rightValue = AbstractSyntaxTree::eval(subRoot->right);
    if (subRoot->elem == "+")
    {
        return leftValue + rightValue;
    }
    if (subRoot->elem == "-")
    {
        return leftValue - rightValue;
    }
    if (subRoot->elem == "*")
    {
        return leftValue * rightValue;
    }
    return leftValue / rightValue;
}