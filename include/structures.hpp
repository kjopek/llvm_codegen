#ifndef STRUCTURES_HPP
#define STRUCTURES_HPP

#include <stdint.h>
#include <map>
#include <list>

namespace SolverCreator {

/* This file contains useful stuctures for
   integration with other components*/

    typedef list<uint64_t> Element;

    typedef struct _Node {
        _Node *left;
        _Node *right;
        Element e;
    } Node;

} // namespace SolverCreator

#endif // STRUCTURES_HPP
