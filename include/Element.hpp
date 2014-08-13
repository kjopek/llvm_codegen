#ifndef ELEMENT_HPP
#define ELEMENT_HPP

#include <vector>

class Element {
    public:
        int x1, y1;
        int x2, y2;
        int k, l;
        std::vector<int> dofs;
};

#endif // ELEMENT_HPP
