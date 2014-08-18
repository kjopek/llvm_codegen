#include "Mesh.hpp"
#include "Analysis.hpp"

#include <cstdio>

int main(int argc, char ** argv)
{
    if (argc != 2) {
        printf("Usage: %s <mesh file>\n", argv[0]);
        return 1;
    }

    Mesh *m = Mesh::loadFromFile(argv[1]);

    if (m == NULL) {
        return 1;
    }

    Analysis::enumerateDOF(m);
/*
    for (Element *e : m->getElements()) {
        printf("At: %d x %d -> %d x %d: ", e->x1, e->y1, e->x2, e->y2);

        for (auto i : e->dofs) {
            printf("%d ", i);
        }
        printf("\n");

    }
*/
    std::set<uint64_t> *p = new std::set<uint64_t>;
    Analysis::nodeAnaliser(m->getRootNode(), p);

    delete p;
    delete m;

    return 0;
}
