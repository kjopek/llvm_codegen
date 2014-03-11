#ifndef LOADER_HPP
#define LOADER_HPP

#include <string>
#include <vector>

struct Function
{
    std::string name;
    std::vector< std::vector < uint32_t > > input;
    std::vector< uint32_t > to_elim;
};

class Loader
{
    public:
        virtual std::vector<Function> load() = 0;
};

#endif
