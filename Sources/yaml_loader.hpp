#ifndef YAML_LOADER_HPP
#define YAML_LOADER_HPP

#include "llvm/Support/YAMLParser.h"
#include "llvm/Support/YAMLTraits.h"

#include "loader.hpp"

class YAMLLoader : Loader
{
    private:
        std::string filename;
    public:
        YAMLLoader(const std::string &filename) : filename(filename) {}
        ~YAMLLoader() {}
        std::vector<Function> load();
};

#endif
