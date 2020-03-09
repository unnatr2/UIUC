#include "HuffmanNode.h"
#include "HuffmanUtils.h"
#include <string>
#include <map>
using std::string;

/**
 * binaryToString
 *
 * Write a function that takes in the root to a huffman tree
 * and a binary string.
 *
 * Remember 0s in the string mean left and 1s mean right.
 */

string binaryToString(string binaryString, HuffmanNode *huffmanTree)
{
    /* TODO: Your code here */
    string result = "";
    HuffmanNode *currentNode = huffmanTree;
    for (size_t i = 0; i < binaryString.size(); i++)
    {
        if (binaryString[i] == '0')
        {
            currentNode = currentNode->left_;
        }
        else
        {
            currentNode = currentNode->right_;
        }
        if (currentNode->left_ == NULL && currentNode->right_ == NULL)
        {
            result += currentNode->char_;
            currentNode = huffmanTree;
        }
    }
    return result;
}

/**
 * stringToBinary
 *
 * Write a function that takes in the root to a huffman tree
 * and a character string. Return the binary representation of the string
 * using the huffman tree.
 *
 * Remember 0s in the binary string mean left and 1s mean right
 */

bool findPath(char currentChar, HuffmanNode *node, string &path)
{
    if (node == NULL)
    {
        return false;
    }
    if (node->left_ == NULL && node->right_ == NULL && node->char_ == currentChar)
    {
        return true;
    }
    if (findPath(currentChar, node->left_, path))
    {
        path = '0' + path;
        return true;
    }
    if (findPath(currentChar, node->right_, path))
    {
        path = '1' + path;
        return true;
    }
    return false;
}

string stringToBinary(string charString, HuffmanNode *huffmanTree)
{
    /* TODO: Your code here */
    string result = "";
    for (size_t i = 0; i < charString.size(); i++)
    {
        string path = "";
        findPath(charString[i], huffmanTree, path);

        // cout << path << endl;
        result += path;
    }
    return result;
}
