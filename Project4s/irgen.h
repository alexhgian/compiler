/**
 * File: irgen.h
 * -----------
 *  This file defines a class for LLVM IR Generation.
 *
 *  All LLVM instruction related functions or utilities can be implemented
 *  here. You'll need to customize this class heavily to provide any helpers
 *  or untility as you need.
 */

#ifndef _H_IRGen
#define _H_IRGen

// LLVM headers
#include "llvm/IR/Module.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Constants.h"
#include "ast_type.h"

class IRGenerator {
  public:
    IRGenerator();
    ~IRGenerator();

    static IRGenerator& getInstance();

    llvm::Module   *GetOrCreateModule(const char *moduleID);
    llvm::LLVMContext *GetContext() const { return context; }

    // Add your helper functions here
    llvm::Function *GetFunction() const;
    void      SetFunction(llvm::Function *func);

    llvm::BasicBlock *GetBasicBlock() const;
    void        SetBasicBlock(llvm::BasicBlock *bb);

    llvm::Type *getIntType() const;
    llvm::Type *getBoolType() const;
    llvm::Type *getFloatType() const;
    llvm::Type *getVoidType() const;
    llvm::Type *getArrayType(ArrayType* arrayType);
    llvm::Type *getVec2Type() const;
    llvm::Type *getVec3Type() const;
    llvm::Type *getVec4Type() const;

    // new declarations
    llvm::Type *getType(Type *type);
    llvm::BasicBlock *createFunctionBlock(const char* name);
    void setTerminator(llvm::BasicBlock *termBB);
    void branchConditionally(llvm::BasicBlock *success, llvm::BasicBlock *fail, llvm::Value* test);
    void pushLoop(llvm::BasicBlock* body, llvm::BasicBlock* footer);
    void popLoop();
    llvm::BasicBlock* getCurrentLoop();
    llvm::BasicBlock* getCurrentBreak();
    void createBreak();
    void createContinue();
    void pushBreak(llvm::BasicBlock *bb);
    void popBreak();
    // llvm::BasicBlock* getLoopTop();
  private:
    llvm::LLVMContext *context;
    llvm::Module      *module;

    // track which function or basic block is active
    llvm::Function    *currentFunc;
    llvm::BasicBlock  *currentBB;

    static const char *TargetTriple;
    static const char *TargetLayout;

    std::vector<llvm::BasicBlock*> loopStack;
    std::vector<llvm::BasicBlock*> breakStack;
};

#endif
