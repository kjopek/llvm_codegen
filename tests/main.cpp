#include <cstdio>
#include <unistd.h>

#include "generator.hpp"

#include <fstream>

#include <list>

void usage(char *progname) {
    printf("Test case1 usage: %s [-O0|-O1|-O2|-O3] -o binary.o\n", progname);
}

using namespace SolverCreator;
using namespace std;

int main(int argc, char ** argv) {

    std::string output = "a.out";

    int c;
    
    CodeGenOpt::Level optLevel = CodeGenOpt::None;
    
    while ( (c = getopt(argc, argv, "o:O:")) != EOF) {
        switch(c) {
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
    vector <uint64_t> a;
    vector <uint64_t> b;
    vector <uint64_t> rc;
    
    a.push_back(20);
    b.push_back(20);

    a.push_back(123);
    b.push_back(123);
    
    rc.push_back(20);
    rc.push_back(123);
    generator->createMergeFunction("test", a, b, rc);

    generator->saveToFile(output);

    delete generator;

    return 0;
}
