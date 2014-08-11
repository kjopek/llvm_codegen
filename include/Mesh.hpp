#ifndef MESH_HPP
#define MESH_HPP

#include <cstdio>
#include <cstdlib>
#include <tuple>
#include <map>
#include <string>
#include <vector>
#include "Element.hpp"
#include "Node.hpp"

class Mesh {
    private:
        std::vector<Node *> nodes;
        std::vector<Element *> elements;
        int polynomial=0;
        int dimensions=2;

    public:
        Mesh(int poly, int dim) : polynomial(poly),
                                  dimensions(dim) {

        }
        void addElement(Element *e);
        void addNode(Node *n);
        static Mesh *loadFromFile(const char *filename);
        Node *getRootNode();
        ~Mesh();
};


#endif // MESH_HPP
