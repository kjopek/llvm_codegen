#ifndef NODE_HPP
#define NODE_HPP

#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>

#include "Element.hpp"

class Node {
    private:
        int node = -1;
        Node *left = NULL;
        Node *right = NULL;
        std::vector<Element *> mergedElements;
        std::string production;
        std::vector<uint64_t> dofs;

    public:
        int n_left = -1;
        int n_right = -1;

        Node(int num) : node(num) {}
        void setLeft(Node *left);
        void setRight(Node *right);

        Node *getLeft();
        Node *getRight();

        void addElement (Element *e);
        std::vector<Element *> &getElements();

        void addDof(uint64_t dof);
        std::vector<uint64_t> &getDofs();

        int getId();

        void setProduction(std::string &prodname);
};

#endif // NODE_HPP
