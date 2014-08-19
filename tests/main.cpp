#include <cstdio>
#include <unistd.h>

#include "Analysis.hpp"
#include "Generator.hpp"
#include "Mesh.hpp"
#include <fstream>

#include <list>

void usage(char *progname) {
    printf("Test case1 usage: %s [-O0|-O1|-O2|-O3] -o binary.o\n", progname);
}

using namespace SolverCreator;
using namespace std;

int main(int argc, char ** argv) {

    std::string output = "a.out";
    std::string input = "mesh.txt";

    int c;
    
    CodeGenOpt::Level optLevel = CodeGenOpt::None;
    
    while ( (c = getopt(argc, argv, "f:o:O:")) != EOF) {
        switch(c) {
            case 'f':
                input = std::string(optarg);
                break;
            case 'o':
                output = std::string(optarg);
                break;
            case 'O':
                if (std::string(optarg) == "0") {
                    optLevel = CodeGenOpt::None;
                    break;
                }
                else if (std::string(optarg) == "1") {
                    optLevel = CodeGenOpt::Less;
                    break;
                }
                else if (std::string(optarg) == "2") {
                    optLevel = CodeGenOpt::Default;
                    break;
                }
                else if (std::string(optarg) == "3") {
                    optLevel = CodeGenOpt::Aggressive;
                    break;
                }
                // pass-trough
            default:
                usage(argv[0]);
                return 1;
        }
    }

    Generator *generator = new Generator(optLevel);
    Mesh *m = Mesh::loadFromFile(input.c_str());
    Analysis::enumerateDOF(m);
    Analysis::doAnalise(m);
    generator->generateCode(m);
    delete m;

    generator->saveToFile(output);

    delete generator;

    return 0;
}
