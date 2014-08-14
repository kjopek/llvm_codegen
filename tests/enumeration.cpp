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

    for (Element *e : m->getElements()) {
        printf("At: %d x %d -> %d x %d: ", e->x1, e->y1, e->x2, e->y2);

        for (auto i : e->dofs) {
            printf("%d ", i);
        }
        printf("\n");

    }
    std::vector<int> *p = new std::vector<int>;
    std::vector<int> *q = Analysis::nodeAnaliser(m->getRootNode(), p);
    for (int i : *q) {
        printf("%d ", i);
    }
    printf("\n");

    for (int i : *Analysis::nodeAnaliser(m->getRootNode()->getLeft(), q)) {
        printf("%d ", i);
    }
    printf("\n");

    delete q;
    delete p;
    delete m;

    return 0;
}
