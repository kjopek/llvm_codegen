#include "generator.hpp"
#include "llvm/Target/TargetLibraryInfo.h"

#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCDisassembler.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstPrinter.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Support/MemoryObject.h"

Generator::Generator(CodeGenOpt::Level optLevel)
{
    InitializeNativeTarget();
    InitializeNativeTargetAsmParser();
    InitializeNativeTargetAsmPrinter();

    TargetOptions options;
    std::string error;

    this->triple = sys::getDefaultTargetTriple();

    const Target *target = TargetRegistry::lookupTarget(triple, error);

    this->targetMachine = target->createTargetMachine(triple,
        "", "", options, Reloc::Default, CodeModel::Default, optLevel);

    this->module = new Module("solver_creator", getGlobalContext());
}

void Generator::save_to_file(const std::string &filename)
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

bool Generator::create_function(const std::string name,
                                std::list< std::list< int > > enumerator,
                                std::list< int > to_sum)
{
    IRBuilder<> builder(getGlobalContext());

    std::vector<Type*> func_args(enumerator.size()*2 + 2,
         PointerType::get((Type*)Type::getDoublePtrTy(getGlobalContext()), 0) );

    FunctionType *funcType = FunctionType::get(Type::getVoidTy(getGlobalContext()), func_args, false);
    Function *function = Function::Create(funcType, Function::ExternalLinkage, name, this->module);

    BasicBlock *entryBlock = BasicBlock::Create(getGlobalContext(), "entry", function);
    builder.SetInsertPoint(entryBlock);

    builder.CreateRetVoid();
//    for 
    return true;
}
