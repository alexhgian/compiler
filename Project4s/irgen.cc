/* irgen.cc -  LLVM IR generator
 *
 * You can implement any LLVM related functions here.
 */

#include "irgen.h"

IRGenerator& IRGenerator::getInstance() {
  static IRGenerator irgen;
  return irgen;
}

IRGenerator::IRGenerator() :
    context(NULL),
    module(NULL),
    currentFunc(NULL),
    currentBB(NULL)
{
}

IRGenerator::~IRGenerator() {
}

llvm::Module *IRGenerator::GetOrCreateModule(const char *moduleID)
{
   if ( module == NULL ) {
     context = new llvm::LLVMContext();
     module  = new llvm::Module(moduleID, *context);
     module->setTargetTriple(TargetTriple);
     module->setDataLayout(TargetLayout);
   }
   return module;
}

void IRGenerator::SetFunction(llvm::Function *func) {
   currentFunc = func;
}

llvm::Function *IRGenerator::GetFunction() const {
   return currentFunc;
}





void IRGenerator::SetBasicBlock(llvm::BasicBlock *bb) {
   currentBB = bb;
}

llvm::BasicBlock *IRGenerator::GetBasicBlock() const {
   return currentBB;
}

llvm::BasicBlock * IRGenerator::createFunctionBlock(const char* name) {
    llvm::BasicBlock *bbTemp = llvm::BasicBlock::Create(*GetContext(), name, GetFunction());
    return bbTemp;
}




llvm::Type *IRGenerator::getIntType() const {
   llvm::Type *ty = llvm::Type::getInt32Ty(*context);
   return ty;
}

llvm::Type *IRGenerator::getBoolType() const {
   llvm::Type *ty = llvm::Type::getInt1Ty(*context);
   return ty;
}

llvm::Type *IRGenerator::getFloatType() const {
   llvm::Type *ty = llvm::Type::getFloatTy(*context);
   return ty;
}

llvm::Type *IRGenerator::getVoidType() const {
   return llvm::Type::getVoidTy(*context);
}

llvm::Type *IRGenerator::getType(Type *type){


    std::string tName = string(type->toString());

    if(tName == "int"){
        return getIntType();
    } else if(tName == "bool"){
        return  getBoolType();
    } else if(tName == "float"){
        return getFloatType();
    } else if(tName == "void"){
        return getVoidType();
    } else {
        return NULL;
    }


}

void IRGenerator::setTerminator(llvm::BasicBlock *termBB){
    IRGenerator &irgen = IRGenerator::getInstance();
    if (!irgen.GetBasicBlock()->getTerminator()) {
        llvm::Value* endBr = llvm::BranchInst::Create(termBB, irgen.GetBasicBlock());
    }
}

void IRGenerator::branchConditionally(llvm::BasicBlock *success, llvm::BasicBlock *fail, llvm::Value* test){
    IRGenerator &irgen = IRGenerator::getInstance();
    llvm::Value* br = llvm::BranchInst::Create(success, fail, test, irgen.GetBasicBlock());
}

const char *IRGenerator::TargetLayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128";

const char *IRGenerator::TargetTriple = "x86_64-redhat-linux-gnu";
