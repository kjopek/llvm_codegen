#include "TreeReader.hpp"
#include <cstdio>
#include <cstdlib>

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

Node *TreeReader::getTree(const char *filename)
{
    FILE *fp;
    Mesh *mesh;
    fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("fopen");
        return NULL;
    }
    // read the P
    unsigned int p = 0;
    unsigned int elements = 0;
    unsigned int nodes = 0;

    fscanf(fp, "%u", &p);
    fscanf(fp, "%u", &elements);
    
    mesh = new Mesh(p, 2);

    std::map<std::tuple<int, int>, Element> elementsMap;
    std::vector<Node *> nodesVector;
    
    for (int i=0; i<elements; ++i) {
        unsigned int k, l;
        unsigned int x1, y1, x2, y2;
        fscanf(fp, "%u %u %u %u %u %u", &k, &l, &x1, &y1, &x2, &y2);
        Element e;
        e.x1 = x1;
        e.x2 = x2;
        e.y1 = y1;
        e.y2 = y2;
        std::tuple<int, int> t(k,l);
        elementsMap[t] = e;
    }

    fscanf(fp, "%u", &nodes);
    
    for (int i=0; i<nodes; ++i) {
        unsigned int node_id;
        unsigned int nr_elems;
        fscanf(fp, "%u %u", &node_id, &nr_elems);
        Node *n = new Node(node_id);
        nodesVector[i] = n;
        for (int q=0; q<nr_elems; ++q) {
            unsigned int k, l;
            fscanf(fp, "%u %u", &k, &l);
            n->addElement(k, l);
        }
        if (nr_elems > 1) {
            unsigned int leftSon, rightSon;
            fscanf(fp, "%u %u", &leftSon, &rightSon);
            n->n_left = leftSon;
            n->n_right = rightSon;
        }
    }
    
    // all nodes read? built the Tree!

    for (int i=0; i<nodes; ++i) {
        if (nodesVector[i]->n_left != -1) {
            nodesVector[i]->setLeft(nodesVector[nodesVector[i]->n_left]);
        }
        if (nodesVector[i]->n_right != -1) {
            nodesVector[i]->setRight(nodesVector[nodesVector[i]->n_right]);
        }
    }
    
    fclose(fp);
    return nodesVector[0];   
}
