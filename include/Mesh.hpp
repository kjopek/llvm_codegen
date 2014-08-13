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
        int vertexDofs;
        int edgeDofs;
        int innerDofs;

    public:
        Mesh(int poly, int dim) : polynomial(poly),
                                  dimensions(dim) {
            vertexDofs = std::pow(2,dimensions);
            edgeDofs = (poly-1)*(poly-1) * (dim == 2 ? 4 : 12);
            innerDofs = (poly-1)*(poly-1)*(poly-1) * (dim == 2 ? 1 : 6);
        }

        void addElement(Element *e);
        void addNode(Node *n);
        static Mesh *loadFromFile(const char *filename);
        Node *getRootNode();
        std::vector<Element *> &getElements();
        int getPolynomial();

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
