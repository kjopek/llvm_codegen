#ifndef TREE_READER_H
#define TREE_READER_H

#include <cstdio>
#include <cstdlib>
#include <tuple>
#include <map>
#include <string>
#include <vector>

class Element {
    public:
        int x1, y1;
        int x2, y2;
        std::vector<int> nodes;
};

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

        Node(int num) : node(num) {};
        void setLeft(Node *left);
        void setRight(Node *right);
        void addElement (int k, int l);
};

class Mesh {
    private:
        std::vector<Node *> nodes;
        std::vector<Element *> elements;
        int polynomial=0;
        int dimensions=2;
    public:
        Mesh(int poly, int dim) : polynomial(poly),
                                  dimensions(dim) {
            
        };
        void addElement(Element *e);
        void addNode(Node *n);
        ~Mesh();
};

class TreeReader {
    public:
        static Node *getTree(const char *filename);
};

#endif // TREE_READER_H
