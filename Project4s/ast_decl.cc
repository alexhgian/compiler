/* File: ast_decl.cc
 * -----------------
 * Implementation of Decl node classes.
 */
#include "ast_decl.h"
#include "ast_type.h"
#include "ast_stmt.h"
#include "symtable.h"
#include "irgen.h"
#include <stdio.h>

Decl::Decl(Identifier *n) : Node(*n->GetLocation()) {
    Assert(n != NULL);
    (id=n)->SetParent(this);
}

VarDecl::VarDecl(Identifier *n, Type *t, Expr *e) : Decl(n) {
    Assert(n != NULL && t != NULL);
    (type=t)->SetParent(this);
    if (e) (assignTo=e)->SetParent(this);
    typeq = NULL;
}

VarDecl::VarDecl(Identifier *n, TypeQualifier *tq, Expr *e) : Decl(n) {
    Assert(n != NULL && tq != NULL);
    (typeq=tq)->SetParent(this);
    if (e) (assignTo=e)->SetParent(this);
    type = NULL;
}

VarDecl::VarDecl(Identifier *n, Type *t, TypeQualifier *tq, Expr *e) : Decl(n) {
    Assert(n != NULL && t != NULL && tq != NULL);
    (type=t)->SetParent(this);
    (typeq=tq)->SetParent(this);
    if (e) (assignTo=e)->SetParent(this);
}

void VarDecl::PrintChildren(int indentLevel) {
   if (typeq) typeq->Print(indentLevel+1);
   if (type) type->Print(indentLevel+1);
   if (id) id->Print(indentLevel+1);
   if (assignTo) assignTo->Print(indentLevel+1, "(initializer) ");
}

void VarDecl::Emit(){
    fprintf(stderr, "VarDecl::Emit \n");
    SymbolTable &symtab = SymbolTable::getInstance();
    IRGenerator &irgen = IRGenerator::getInstance();

    Identifier *id = this->GetIdentifier();

    llvm::Type *vType = irgen.getType(this->GetType());
    llvm::Value *v = NULL;

    // Check if variable is global
    if(symtab.isGlobalScope()){
         v = new llvm::GlobalVariable(vType, false, llvm::GlobalValue::ExternalLinkage, NULL, id->GetName());
    } else {
        llvm::BasicBlock *entryBB = &(irgen.GetFunction()->getEntryBlock());
         v = new llvm::AllocaInst(vType, id->GetName(), entryBB);
        //  llvm::LoadInst *inst = new llvm::LoadInst(v, id->GetName(), entryBB);
    }
    fprintf(stderr, "VarDecl id->GetName(): %s\n", id->GetName());
    Symbol tmpSym(id->GetName(), this, E_VarDecl, v);

    symtab.insert(tmpSym);
}

FnDecl::FnDecl(Identifier *n, Type *r, List<VarDecl*> *d) : Decl(n) {
    Assert(n != NULL && r!= NULL && d != NULL);
    (returnType=r)->SetParent(this);
    (formals=d)->SetParentAll(this);
    body = NULL;
    returnTypeq = NULL;
}

FnDecl::FnDecl(Identifier *n, Type *r, TypeQualifier *rq, List<VarDecl*> *d) : Decl(n) {
    Assert(n != NULL && r != NULL && rq != NULL&& d != NULL);
    (returnType=r)->SetParent(this);
    (returnTypeq=rq)->SetParent(this);
    (formals=d)->SetParentAll(this);
    body = NULL;
}

void FnDecl::SetFunctionBody(Stmt *b) {
    (body=b)->SetParent(this);
}

void FnDecl::PrintChildren(int indentLevel) {
    if (returnType) returnType->Print(indentLevel+1, "(return type) ");
    if (id) id->Print(indentLevel+1);
    if (formals) formals->PrintAll(indentLevel+1, "(formals) ");
    if (body) body->Print(indentLevel+1, "(body) ");
}


void FnDecl::Emit() {
    SymbolTable &symtab = SymbolTable::getInstance();
    IRGenerator &irgen = IRGenerator::getInstance();

    // get the module singleton
    llvm::Module *mod = irgen.GetOrCreateModule(NULL);

    // generate function arguments
    std::vector<llvm::Type *> argTypes;
    Type *argType;
    for (int i = 0; i < formals->NumElements(); ++i) {
        argType = formals->Nth(i)->GetType();
        argTypes.push_back(irgen.getType(argType));
    }


    // generate function header
    llvm::ArrayRef<llvm::Type *> argArray(argTypes);
    llvm::Type *retType = irgen.getType(returnType);
    llvm::FunctionType *funcTy = llvm::FunctionType::get(retType, argArray, false);
    mod->getOrInsertFunction(id->GetName(), funcTy);

    // generate function name
    llvm::Function *f = mod->getFunction(id->GetName());
    irgen.SetFunction(f);


    // Add function to scope
    Symbol tmpSym(id->GetName(), this, E_FunctionDecl, f);
    symtab.insert(tmpSym);

    // Create function scope
    symtab.push();

    // create BasicBlock entry
    llvm::BasicBlock *bbEntry = irgen.createFunctionBlock("entry");
    irgen.SetBasicBlock(bbEntry);

    // create BasicBlock next
    llvm::BasicBlock *bbNext = irgen.createFunctionBlock("next");
    irgen.SetBasicBlock(bbNext);

    // function arguments
    llvm::Function::arg_iterator arg = f->arg_begin();
    for (int i = 0; arg != f->arg_end() && i < formals->NumElements(); i++, arg++) {
        VarDecl *argDecl = formals->Nth(i);
        Identifier *id = argDecl->GetIdentifier();
        llvm::Type *varType = irgen.getType(argDecl->GetType());

        arg->setName(id->GetName());

        // printf(stderr, "%s\n", "arg"+std::to_string(i));
        llvm::BasicBlock *tmpEntryBB = &(irgen.GetFunction()->getEntryBlock());
        char argName[32];
        sprintf (argName,"arg%d",i);
        llvm::Value *storeVal = new llvm::AllocaInst(varType, argName, tmpEntryBB);

        // Symbol *symRes = symtab.find(argDecl->GetIdentifier()->GetName());
        llvm::Value *storeInst = new llvm::StoreInst(arg, storeVal, false, bbNext);
        // llvm::LoadInst *inst = new llvm::LoadInst(storeVal, "", bbNext);

        // Store in symtable
        fprintf(stderr, "Storing functin args: %s\n", id->GetName());
        Symbol tmpSym(id->GetName(), this, E_VarDecl, storeVal);
        symtab.insert(tmpSym);
        //   Scope::current->AssignVar(argDecl->GetIdentifier(), arg);
    }

    // ----- generate body  -----

    body->Emit();

    // branch from entry to next
    llvm::BranchInst::Create(bbNext, bbEntry);

    // Unset basic block
    irgen.SetBasicBlock(NULL);
    symtab.pop();
}
