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
#include <cmath>

class Mesh {
    private:
        std::vector<Node *> nodes;
        std::vector<Element *> elements;
        int polynomial=0;
        int dimensions=2;
        int dofs = 0;

    public:
        Mesh(int poly, int dim) : polynomial(poly),
                                  dimensions(dim) {
        }

        void addElement(Element *e);
        void addNode(Node *n);
        static Mesh *loadFromFile(const char *filename);
        Node *getRootNode();
        std::vector<Element *> &getElements();
        int getPolynomial();
        void setDofs(int dofs);
        int getDofs();

        ~Mesh() {
            for (Node *n : nodes) {
                delete (n);
            }
            for (Element *e : elements) {
                delete (e);
            }
        }

};


#endif // MESH_HPP
