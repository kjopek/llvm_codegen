#include <cstdio>

#include <map>
#include <vector>

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

using namespace llvm;

static Module *mainModule;

int main()
{
    LLVMContext &context = getGlobalContext();

    mainModule = new Module("solver_creator", context);

    mainModule->dump();    

    return 0;
}

