#include <cstdio>
#include <unistd.h>

#include "generator.hpp"

#include <fstream>
#include "yaml-cpp/yaml.h"

void usage() {
    printf("Usage: Creator -f input.file -o binary.o\n");
}

int main(int argc, char ** argv) {

    std::string output;
    std::string input;

    int c;
    while ( (c = getopt(argc, argv, "i:o:")) != EOF) {
        switch(c) {
            case 'i':
                input = std::string(optarg);
                break;
            case 'o':
                output = std::string(optarg);
                break;
            default:
                usage();
                return 1;
        }
    }

    std::cout << output << std::endl;

    Generator *generator = new Generator();

    generator->save_to_file(output);
    delete generator;
    return 0;
}
