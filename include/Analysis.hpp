#ifndef ANALYSIS_HPP
#define ANALYSIS_HPP

#include "Mesh.hpp"

class Analysis {
public:
    static void enumerateDOF(Mesh *mesh);
    static void doAnalise(Mesh *mesh);
};

#endif // ANALYSIS_HPP
