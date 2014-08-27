#include "Mesh.hpp"
#include "Analysis.hpp"
#include "Generator.hpp"

#include <cstdio>
#include <sys/time.h>

int main(int argc, char ** argv)
{
    struct timeval tm1, tm2;
    if (argc != 2) {
        printf("Usage: %s <mesh file>\n", argv[0]);
        return 1;
    }


    CodeGenOpt::Level optLevel = CodeGenOpt::Default;
    Mesh *m = Mesh::loadFromFile(argv[1]);
    SolverCreator::Generator *generator = new SolverCreator::Generator(optLevel);
    if (m == NULL) {
        return 1;
    }

    gettimeofday(&tm1, NULL);
    Analysis::enumerateDOF(m);
    gettimeofday(&tm2, NULL);
    printf("Enumeration time: %f [s]\n", (tm2.tv_sec - tm1.tv_sec) + (tm2.tv_usec - tm1.tv_usec)*1e-6);
    gettimeofday(&tm1, NULL);
    std::set<uint64_t> *p = new std::set<uint64_t>;
    Analysis::nodeAnaliser(m->getRootNode(), p);
    gettimeofday(&tm2, NULL);
    printf("Analysis time: %f [s]\n", (tm2.tv_sec - tm1.tv_sec) + (tm2.tv_usec - tm1.tv_usec)*1e-6);

    gettimeofday(&tm1, NULL);
    generator->generateCode(m);
    gettimeofday(&tm2, NULL);
    printf("Generation time: %f [s]\n", (tm2.tv_sec - tm1.tv_sec) + (tm2.tv_usec - tm1.tv_usec)*1e-6);
    delete p;
    delete m;
    delete generator;

    return 0;
}
