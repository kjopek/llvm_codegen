#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include <iostream>

#include <list>
#include <map>

#include "llvm/Analysis/Passes.h"
#include "llvm/Analysis/Verifier.h"

#include "llvm/PassManager.h"

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/DerivedTypes.h"

#include "llvm/Target/TargetMachine.h"

#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/TargetRegistry.h" 
#include "llvm/Support/Host.h"

#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/Support/Program.h"

using namespace llvm;

namespace SolverCreator {

    class Generator
    {
    private:
        TargetMachine *targetMachine;
        std::string triple;
        Module *module;

    public:
        Generator(CodeGenOpt::Level optLevel = CodeGenOpt::Aggressive);
        ~Generator();

        bool createFunction(const std::string name,
                         std::list< std::list< int > > enumerator,
                         std::list< int > to_eliminate);

        void saveToFile(const std::string &filename);
    };

}
#endif

