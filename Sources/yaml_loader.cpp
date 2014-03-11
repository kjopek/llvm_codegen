#include "yaml_loader.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

using llvm::yaml::IO;

//typedef Sequence<Function> FunctionList;


std::vector<Function> YAMLLoader::load()
{
    std::ostringstream data;
    std::ifstream input_stream(this->filename.c_str(), std::ios::in | std::ios::binary);
    data << input_stream.rdbuf();
    input_stream.close();

    llvm::yaml::Input yin(data.str());

    std::vector<Function> doc;
    yin >> doc;

    return doc;
}
