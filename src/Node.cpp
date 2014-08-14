#include "Node.hpp"

void Node::setLeft(Node *left)
{
    this->left = left;
}

void Node::setRight(Node *right)
{
    this->right = right;
}

void Node::addElement(Element *e)
{
    this->mergedElements.push_back(e);
}

void Node::setProduction(std::string &prodname)
{
    this->production = prodname;
}

Node *Node::getLeft()
{
    return this->left;
}

Node *Node::getRight()
{
    return this->right;
}

std::vector<Element *> &Node::getElements()
{
    return this->mergedElements;
}

int Node::getId()
{
    return this->node;
}
