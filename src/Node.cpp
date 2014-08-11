#include "Node.hpp"

void Node::setLeft(Node *left)
{
    this->left = left;
}

void Node::setRight(Node *right)
{
    this->right = right;
}

void Node::addElement(int k, int l)
{
    this->mergedElements.push_back(std::tuple<int, int>(k, l));
}
