#include "Analysis.hpp"

#include <set>
#include <map>
#include <vector>
#include <tuple>

using namespace std;

typedef tuple<int, int> vertex;
typedef tuple<vertex, vertex> edge;
typedef tuple<vertex, vertex> face;

void Analysis::doAnalise(Mesh *mesh)
{

}

void Analysis::enumerateDOF(Mesh *mesh)
{
    map<int, vector<Element*>> elementMap;
    set<int> levels;
    map<vertex, int> vertices;
    map<edge, int> edges;

    int n = 1;

    // now, we have level plan for mesh
    for (Element *e : mesh->getElements()) {
        levels.insert(e->k);
        elementMap[e->k].push_back(e);
    }

    // implementation assumes that the neighbours may vary on one level only
    for (int level : levels) {
        vector<Element *> elems = elementMap[level];
        // on the first layer we do not need to care about adaptation
        if (level == 1) {
            for (Element *elem : elems) {
                // check vertices
                vertex v1(elem->x1, elem->y1);
                vertex v2(elem->x2, elem->y1);
                vertex v3(elem->x2, elem->y2);
                vertex v4(elem->x1, elem->y2);

                edge e1(v1, v2);
                edge e2(v2, v3);
                edge e3(v4, v3);
                edge e4(v1, v4);

                face f1(v1, v3);

                // vertices
                if (!vertices.count(v1)) {
                    vertices[v1] = n++;
                }
                elem->dofs.push_back(vertices[v1]);

                if (!vertices.count(v2)) {
                    vertices[v2] = n++;
                }
                elem->dofs.push_back(vertices[v2]);

                if (!vertices.count(v3)) {
                    vertices[v3] = n++;
                }
                elem->dofs.push_back(vertices[v3]);

                if (!vertices.count(v4)) {
                    vertices[v4] = n++;
                }
                elem->dofs.push_back(vertices[v4]);

                // edges
                if (!edges.count(e1)) {
                    edges[e1] = n;
                    n += (mesh->getPolynomial()-1);
                }
                for (int i=0;i<mesh->getPolynomial()-1; ++i) {
                    elem->dofs.push_back(edges[e1]+i);
                }

                if (!edges.count(e2)) {
                    edges[e2] = n;
                    n += (mesh->getPolynomial()-1);
                }
                for (int i=0;i<mesh->getPolynomial()-1; ++i) {
                    elem->dofs.push_back(edges[e2]+i);
                }

                if (!edges.count(e3)) {
                    edges[e3] = n;
                    n += (mesh->getPolynomial()-1);
                }
                for (int i=0;i<mesh->getPolynomial()-1; ++i) {
                    elem->dofs.push_back(edges[e3]+i);
                }
                if (!edges.count(e4)) {
                    edges[e4] = n;
                    n += (mesh->getPolynomial()-1);
                }
                for (int i=0;i<mesh->getPolynomial()-1; ++i) {
                    elem->dofs.push_back(edges[e4]+i);
                }
                // in 2-dimensional space the faces do not overlap
                for (int i=0;i<(mesh->getPolynomial()-1)*(mesh->getPolynomial()-1); ++i) {
                    elem->dofs.push_back(n+i);
                }
                n += (mesh->getPolynomial()-1)*(mesh->getPolynomial()-1);
            }
        } else {

        }
    }
}
