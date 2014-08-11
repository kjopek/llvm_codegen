#ifndef NODE_HPP
#define NODE_HPP

#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>

class Node {
    private:
        int node = -1;
        Node *left = NULL;
        Node *right = NULL;
        std::vector<std::tuple<int, int>> mergedElements;
        std::string production;

    public:
        int n_left = -1;
        int n_right = -1;

        Node(int num) : node(num) {}
        void setLeft(Node *left);
        void setRight(Node *right);
        void addElement (int k, int l);
};

#endif // NODE_HPP
