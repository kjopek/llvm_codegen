#include "Generator.hpp"
#include "llvm/Target/TargetLibraryInfo.h"

#include "llvm/IR/Constants.h"

#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCDisassembler.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstPrinter.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Support/MemoryObject.h"

#include <stack>
#include <set>

using namespace SolverCreator;

Generator::Generator(CodeGenOpt::Level optLevel, int wordSize)
{
    InitializeNativeTarget();
    InitializeNativeTargetAsmParser();
    InitializeNativeTargetAsmPrinter();

    TargetOptions options;
    std::string error;

    this->triple = sys::getDefaultTargetTriple();
    this->wordSize = wordSize;
    const Target *target = TargetRegistry::lookupTarget(triple, error);

    this->targetMachine = target->createTargetMachine(triple,
        "", "", options, Reloc::Default, CodeModel::Default, optLevel);

    this->module = new Module("solver_creator", getGlobalContext());

}

void Generator::saveToFile(const std::string &filename)
{
    PassManager pass;
    std::string error;

    pass.add(new TargetLibraryInfo(Triple(targetMachine->getTargetTriple())));

    raw_fd_ostream dest(filename.c_str(), error, sys::fs::F_Binary);
    formatted_raw_ostream destf(dest);

    if (this->targetMachine->addPassesToEmitFile(pass, destf, TargetMachine::CGFT_ObjectFile)) {
        std::cout << error << std::endl;
    }

    pass.run(*this->module);
    destf.flush();
    dest.flush();
}

Generator::~Generator()
{
    delete this->module;
}

Value *Generator::getMatrixElement(IRBuilder<> &builder, Value *matrix, uint64_t x, uint64_t y)
{
    return builder.CreateLoad(builder.CreateGEP(
                                     builder.CreateLoad(builder.CreateGEP(matrix,
                                     ConstantInt::get(getGlobalContext(), APInt(this->wordSize, x)))),
                                     ConstantInt::get(getGlobalContext(), APInt(this->wordSize, y))));
}

Value *Generator::getVectorElement(IRBuilder<> &builder, Value *vector, uint64_t x)
{
    return builder.CreateLoad(builder.CreateGEP(vector, ConstantInt::get(getGlobalContext(), APInt(this->wordSize, x))));
}

Value *Generator::getMatrixTgtPtr(IRBuilder<> &builder, Value *matrix, uint64_t x, uint64_t y)
{
    return builder.CreateGEP(builder.CreateLoad(builder.CreateGEP(matrix,
                                                         ConstantInt::get(getGlobalContext(), APInt(this->wordSize, x)))),
                                                         ConstantInt::get(getGlobalContext(), APInt(this->wordSize, y))) ;
}

Value *Generator::getVectorTgtPtr(IRBuilder<> &builder, Value *vector, uint64_t x)
{
    return builder.CreateGEP(vector, ConstantInt::get(getGlobalContext(), APInt(this->wordSize, x)));
}

void Generator::generateCode(Mesh *m)
{
    std::stack<Node*> stack;
    stack.push(m->getRootNode());

    while (!stack.empty()) {
        Node *n = stack.top();
        std::string prodName = "A" + std::to_string(n->getId());
        stack.pop();
        printf("Creating production: %s\n", prodName.c_str());
        if (n->getLeft() != NULL && n->getRight() != NULL) {

            this->createMergeFunction(prodName, n->getLeft()->getDofs(),
                                      n->getRight()->getDofs(),
                                      n->getDofs());
            stack.push(n->getLeft());
            stack.push(n->getRight());
        } else {
            this->createPreprocessFunction(prodName, n->getElements()[0]->dofs, n->getDofs());
        }
    }
}

void Generator::createMergeFunction(const std::string &name,
                                    const std::vector< uint64_t > &nodesA,
                                    const std::vector< uint64_t > &nodesB,
                                    const std::vector< uint64_t > &outOrder)
{

    // data structures for inverse mapping
    // these mappings will be used later to create summing and moving instructions

    std::map<uint64_t, uint64_t> revNodesA;
    std::map<uint64_t, uint64_t> revNodesB;
    
    uint64_t counter = 0;
    
    for (std::vector<uint64_t>::const_iterator i = nodesA.cbegin(); 
                                        i != nodesA.cend(); 
                                        ++i) {
#ifdef DEBUG
        printf("%ld at %ld\n", *i, counter);
#endif
        revNodesA[*i] = counter++; 
    }
    
    counter = 0;
    
    for (std::vector<uint64_t>::const_iterator i = nodesB.cbegin(); 
                                        i != nodesB.cend(); 
                                        ++i) {
#ifdef DEBUG
        printf("%ld at %ld\n", *i, counter);
#endif
        revNodesB[*i] = counter++;
    }
    
    IRBuilder<> builder(getGlobalContext());

    std::vector<Type*> func_args;
    func_args.push_back(PointerType::get(Type::getDoublePtrTy(getGlobalContext()), 0));
    func_args.push_back(Type::getDoublePtrTy(getGlobalContext()));
    func_args.push_back(PointerType::get(Type::getDoublePtrTy(getGlobalContext()), 0));
    func_args.push_back(Type::getDoublePtrTy(getGlobalContext()));
    func_args.push_back(PointerType::get(Type::getDoublePtrTy(getGlobalContext()), 0));
    func_args.push_back(Type::getDoublePtrTy(getGlobalContext()));

    FunctionType *funcType = FunctionType::get(Type::getVoidTy(getGlobalContext()), 
                                               func_args, false);

    Function *function = Function::Create(funcType, Function::ExternalLinkage, 
                                          name, this->module);

    Function::arg_iterator arg_iterator = function->arg_begin();

    BasicBlock *entryBlock = BasicBlock::Create(getGlobalContext(), "entry", function);
    builder.SetInsertPoint(entryBlock);

    Value *matrixA = arg_iterator++;
    Value *rhsA = arg_iterator++;
    Value *matrixB = arg_iterator++;
    Value *rhsB = arg_iterator++;
    Value *matrixOut = arg_iterator++;
    Value *rhsOut = arg_iterator++;
    
    matrixA->setName("matrixA");
    matrixB->setName("matrixB");
    matrixOut->setName("matrixOut");
    rhsA->setName("rhsA");
    rhsB->setName("rhsB");
    rhsOut->setName("rhsOut");
    
    for (uint64_t i=0; i<outOrder.size(); ++i) {
        uint64_t x_node = outOrder[i];
        for (uint64_t j=0; j<outOrder.size(); ++j) {
            uint64_t y_node = outOrder[j];

            bool inA = (revNodesA.count(x_node) > 0) && (revNodesA.count(y_node) > 0);
            bool inB = (revNodesB.count(x_node) > 0) && (revNodesB.count(y_node) > 0);

            if (inA && inB) {
                uint64_t offsetAX = revNodesA[x_node];
                uint64_t offsetAY = revNodesA[y_node];
                uint64_t offsetBX = revNodesB[x_node];
                uint64_t offsetBY = revNodesB[y_node];
                
                Value *valA = this->getMatrixElement(builder, matrixA, offsetAX, offsetAY);
                Value *valB = this->getMatrixElement(builder, matrixB, offsetBX, offsetBY);
                
                Value *sumAB = builder.CreateFAdd(valA, valB);
                Value *targetPtr = this->getMatrixTgtPtr(builder, matrixOut, i, j);
                builder.CreateStore(sumAB, targetPtr);
                
                
            }
            else if (inA && !inB) {
                uint64_t offsetAX = revNodesA[x_node];
                uint64_t offsetAY = revNodesA[y_node];
                Value *valA = this->getMatrixElement(builder, matrixA, offsetAX, offsetAY);
                Value *targetPtr = this->getMatrixTgtPtr(builder, matrixOut, i, j);
                builder.CreateStore(valA, targetPtr);
            }
            else if (!inA && inB) {
                uint64_t offsetBX = revNodesB[x_node];
                uint64_t offsetBY = revNodesB[y_node];
                Value *valB = this->getMatrixElement(builder, matrixB, offsetBX, offsetBY);
                Value *targetPtr = this->getMatrixTgtPtr(builder, matrixOut, i, j);
                builder.CreateStore(valB, targetPtr);
                
            }
        }
        bool inA = revNodesA.count(x_node) > 0;
        bool inB = revNodesB.count(x_node) > 0;
        if (inA && inB) {
            uint64_t offsetAX = revNodesA[x_node];
            uint64_t offsetBX = revNodesB[x_node];
            Value *valRhsA = this->getVectorElement(builder, rhsA, offsetAX);
            Value *valRhsB = this->getVectorElement(builder, rhsB, offsetBX);
            Value *sumRHS = builder.CreateFAdd(valRhsA, valRhsB);

            Value *targetRhsPtr = this->getVectorTgtPtr(builder, rhsOut, i);
            builder.CreateStore(sumRHS, targetRhsPtr);
        } else if (inA && !inB) {
            uint64_t offsetAX = revNodesA[x_node];
            Value *valRhsA = this->getVectorElement(builder, rhsA, offsetAX);
            Value *targetRhsPtr = this->getVectorTgtPtr(builder, rhsOut, i);
            builder.CreateStore(valRhsA, targetRhsPtr);
        } else if (!inA && inB) {
            uint64_t offsetBX = revNodesB[x_node];
            Value *valRhsB = this->getVectorElement(builder, rhsB, offsetBX);
            Value *targetRhsPtr = this->getVectorTgtPtr(builder, rhsOut, i);
            builder.CreateStore(valRhsB, targetRhsPtr);
        }
    }

    builder.CreateRetVoid();
}

void Generator::createPreprocessFunction(const std::string &name,
                                         const std::vector<uint64_t> &nodesIn,
                                         const std::vector<uint64_t> &nodesOut)
{
    std::map<uint64_t, uint64_t> revNodes; // reverse mapping
    uint64_t counter = 0;

    for (std::vector<uint64_t>::const_iterator i = nodesIn.cbegin();
                                        i != nodesIn.cend();
                                        ++i) {
#ifdef DEBUG
        printf("%ld at %ld\n", *i, counter);
#endif
        revNodes[*i] = counter++;
    }

    IRBuilder<> builder(getGlobalContext());
    std::vector<Type*> func_args;
    func_args.push_back(PointerType::get(Type::getDoublePtrTy(getGlobalContext()), 0));
    func_args.push_back(Type::getDoublePtrTy(getGlobalContext()));
    func_args.push_back(PointerType::get(Type::getDoublePtrTy(getGlobalContext()), 0));
    func_args.push_back(Type::getDoublePtrTy(getGlobalContext()));

    FunctionType *funcType = FunctionType::get(Type::getVoidTy(getGlobalContext()),
                                               func_args, false);

    Function *function = Function::Create(funcType, Function::ExternalLinkage,
                                          name, this->module);

    Function::arg_iterator arg_iterator = function->arg_begin();

    BasicBlock *entryBlock = BasicBlock::Create(getGlobalContext(), "entry", function);
    builder.SetInsertPoint(entryBlock);

    Value *matrixIn = arg_iterator++;
    Value *rhsIn = arg_iterator++;
    Value *matrixOut = arg_iterator++;
    Value *rhsOut = arg_iterator++;

    matrixIn->setName("matrixIn");
    rhsIn->setName("rhsIn");
    matrixOut->setName("matrixOut");
    rhsOut->setName("rhsOut");

    for (uint64_t i=0; i<nodesOut.size(); ++i) {
        uint64_t x_node = nodesOut[i];
        uint64_t offsetInX = revNodes[x_node];
        for (uint64_t j=0; j<nodesOut.size(); ++j) {
            uint64_t y_node = nodesOut[j];
            uint64_t offsetInY = revNodes[y_node];
            Value *value = this->getMatrixElement(builder, matrixIn, offsetInX, offsetInY);
            Value *targetPtr = this->getMatrixTgtPtr(builder, matrixOut, i, j);
            builder.CreateStore(value, targetPtr);

        }
        Value *valueRhs = this->getVectorElement(builder, rhsIn, offsetInX);
        Value *targetRhsPtr = this->getVectorTgtPtr(builder, rhsOut, i);
        builder.CreateStore(valueRhs, targetRhsPtr);
    }
    builder.CreateRetVoid();
}
