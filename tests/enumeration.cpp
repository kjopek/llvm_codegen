#include "Mesh.hpp"
#include "Analysis.hpp"

#include <cstdio>
#include <sys/time.h>

int main(int argc, char ** argv)
{
    struct timeval tm1, tm2;
    if (argc != 2) {
        printf("Usage: %s <mesh file>\n", argv[0]);
        return 1;
    }

    Mesh *m = Mesh::loadFromFile(argv[1]);

    if (m == NULL) {
        return 1;
    }

    Analysis::enumerateDOF(m);
    gettimeofday(&tm1, NULL);
    std::set<uint64_t> *p = new std::set<uint64_t>;
    Analysis::nodeAnaliser(m->getRootNode(), p);
    gettimeofday(&tm2, NULL);
    printf("Analysis time: %f [s]\n", (tm2.tv_sec - tm1.tv_sec) + (tm2.tv_usec - tm1.tv_usec)*1e-6);
    delete p;
    delete m;

    return 0;
}
