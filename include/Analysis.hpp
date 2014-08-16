#ifndef ANALYSIS_HPP
#define ANALYSIS_HPP
#include <set>
#include <map>
#include <vector>
#include <tuple>
#include <map>
#include <algorithm>
#include "Mesh.hpp"

typedef std::tuple<int, int> vertex;
typedef std::tuple<vertex, vertex> edge;
typedef std::tuple<vertex, vertex> face;

class Analysis {
    private:

        static void enumerateElem1(Mesh *mesh, Element *elem,
                    std::map<int, std::map<vertex, int>> &levelVertices,
                    std::map<int, std::map<edge, int>> &levelEdges, int &n);
        static void enumerateElem(Mesh *mesh, Element *elem,
                    std::map<int, std::map<vertex, int>> &levelVertices,
                    std::map<int, std::map<edge, int>> &levelEdges, int &n, int level);
        // returns modified edge if taken from parent or returns
        // original edge with the information which coordinate has been modified
        static std::tuple<edge, int> parentEdge(edge e,
                    std::map<int, std::map<vertex, int>> &levelVertices,
                    std::map<int, std::map<edge, int>> &levelEdges, int level);
    public:
        static void enumerateDOF(Mesh *mesh);
        static void doAnalise(Mesh *mesh);
        static void nodeAnaliser(Node *n, std::set<int> *parent);
};

#endif // ANALYSIS_HPP
