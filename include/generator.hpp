#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include <iostream>

#include <vector>
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

    enum RowColOrder {
        ROW_MAJOR,
        COL_MAJOR
    };

    class Generator
    {
      private:
        TargetMachine *targetMachine;
        std::string triple;
        Module *module;
        // wordSize may be 32 or 64
        int wordSize;
        RowColOrder dataOrder = ROW_MAJOR;
        
        Value *getMatrixElement(IRBuilder<> &builder, Value *matrix, uint64_t x, uint64_t y);
        Value *getVectorElement(IRBuilder<> &builder, Value *vector, uint64_t x);

        Value *getMatrixTgtPtr(IRBuilder<> &builder, Value *matrix, uint64_t x, uint64_t y);
        Value *getVectorTgtPtr(IRBuilder<> &builder, Value *vector, uint64_t x);

      public:
        Generator(CodeGenOpt::Level optLevel = CodeGenOpt::None, int wordSize = 64);
        ~Generator();

        bool createMergeFunction(const std::string &name,
        	const std::vector< uint64_t > &nodesA,
		    const std::vector< uint64_t > &nodesB,
            const std::vector< uint64_t > &outOrder);

	    bool createEliminationFunction(const std::string &name,
			int nodesToEliminate);
			 
        bool createBackSubstitutionFunction(const std::string &name,
            const std::vector< int > &localNodes,
            const std::list< int > &parentNodes);

        void saveToFile(const std::string &filename);
    };

}
#endif
