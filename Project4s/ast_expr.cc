/* File: ast_expr.cc
 * -----------------
 * Implementation of expression node classes.
 */

#include <string.h>
#include "ast_expr.h"
#include "ast_type.h"
#include "ast_decl.h"
#include "symtable.h"
#include "irgen.h"

IntConstant::IntConstant(yyltype loc, int val) : Expr(loc) {
    value = val;
}
void IntConstant::PrintChildren(int indentLevel) {
    printf("%d", value);
}

FloatConstant::FloatConstant(yyltype loc, double val) : Expr(loc) {
    value = val;
}
void FloatConstant::PrintChildren(int indentLevel) {
    printf("%g", value);
}

BoolConstant::BoolConstant(yyltype loc, bool val) : Expr(loc) {
    value = val;
}
void BoolConstant::PrintChildren(int indentLevel) {
    printf("%s", value ? "true" : "false");
}

VarExpr::VarExpr(yyltype loc, Identifier *ident) : Expr(loc) {
    Assert(ident != NULL);
    this->id = ident;
}

void VarExpr::PrintChildren(int indentLevel) {
    id->Print(indentLevel+1);
}

/**
* VarExpr
* Search the symbol table using identifer name
* Get the value (llvm:Value) from the symbol
* Use LoadInst to load the value inside the current BasicBlock
*/
llvm::Value* VarExpr::getValue(){
    SymbolTable &symtab = SymbolTable::getInstance();
    IRGenerator &irgen = IRGenerator::getInstance();

    Symbol *sym = symtab.find(id->GetName());
    llvm::Value *v = sym->value;

    // fprintf(stderr, "VarExpr id->GetName(): %s\n", id->GetName());
    // fprintf(stderr, "VarExpr symbol name: %s\n", sym->name);
    // if(sym){
    //     fprintf(stderr, "VarExpr found: true\n");
    // } else {
    //     fprintf(stderr, "VarExpr found: false\n");
    // }

    // v->dump();
    llvm::BasicBlock *bb = irgen.GetBasicBlock();
    llvm::LoadInst *inst = new llvm::LoadInst(v, "", bb);

    return inst;
}

Operator::Operator(yyltype loc, const char *tok) : Node(loc) {
    Assert(tok != NULL);
    strncpy(tokenString, tok, sizeof(tokenString));
}

void Operator::PrintChildren(int indentLevel) {
    printf("%s",tokenString);
}

bool Operator::IsOp(const char *op) const {
    return strcmp(tokenString, op) == 0;
}

CompoundExpr::CompoundExpr(Expr *l, Operator *o, Expr *r)
  : Expr(Join(l->GetLocation(), r->GetLocation())) {
    Assert(l != NULL && o != NULL && r != NULL);
    (op=o)->SetParent(this);
    (left=l)->SetParent(this);
    (right=r)->SetParent(this);
}

CompoundExpr::CompoundExpr(Operator *o, Expr *r)
  : Expr(Join(o->GetLocation(), r->GetLocation())) {
    Assert(o != NULL && r != NULL);
    left = NULL;
    (op=o)->SetParent(this);
    (right=r)->SetParent(this);
}

CompoundExpr::CompoundExpr(Expr *l, Operator *o)
  : Expr(Join(l->GetLocation(), o->GetLocation())) {
    Assert(l != NULL && o != NULL);
    (left=l)->SetParent(this);
    (op=o)->SetParent(this);
}

void CompoundExpr::PrintChildren(int indentLevel) {
   if (left) left->Print(indentLevel+1);
   op->Print(indentLevel+1);
   if (right) right->Print(indentLevel+1);
}

ConditionalExpr::ConditionalExpr(Expr *c, Expr *t, Expr *f)
  : Expr(Join(c->GetLocation(), f->GetLocation())) {
    Assert(c != NULL && t != NULL && f != NULL);
    (cond=c)->SetParent(this);
    (trueExpr=t)->SetParent(this);
    (falseExpr=f)->SetParent(this);
}

void ConditionalExpr::PrintChildren(int indentLevel) {
    cond->Print(indentLevel+1, "(cond) ");
    trueExpr->Print(indentLevel+1, "(true) ");
    falseExpr->Print(indentLevel+1, "(false) ");
}
ArrayAccess::ArrayAccess(yyltype loc, Expr *b, Expr *s) : LValue(loc) {
    (base=b)->SetParent(this);
    (subscript=s)->SetParent(this);
}

void ArrayAccess::PrintChildren(int indentLevel) {
    base->Print(indentLevel+1);
    subscript->Print(indentLevel+1, "(subscript) ");
}

FieldAccess::FieldAccess(Expr *b, Identifier *f)
  : LValue(b? Join(b->GetLocation(), f->GetLocation()) : *f->GetLocation()) {
    Assert(f != NULL); // b can be be NULL (just means no explicit base)
    base = b;
    if (base) base->SetParent(this);
    (field=f)->SetParent(this);
}

llvm::BinaryOperator::BinaryOps toBinaryOps(bool isFloat, Operator *op){
    llvm::BinaryOperator::BinaryOps _op;
    if (op->IsOp("+")) {
        if(isFloat) {
            _op = llvm::BinaryOperator::FAdd;
        } else {
            _op = llvm::BinaryOperator::Add;
        }
    } else if (op->IsOp("-")){
        if(isFloat) {
            _op = llvm::BinaryOperator::FSub;
        } else {
            _op = llvm::BinaryOperator::Sub;
        }
    } else if (op->IsOp("*")){
        if(isFloat) {
            _op = llvm::BinaryOperator::FMul;
        } else {
            _op = llvm::BinaryOperator::Mul;
        }
    } else {
        if(isFloat) {
            _op = llvm::BinaryOperator::FDiv;
        } else {
            _op = llvm::BinaryOperator::SDiv;
        }
    }
    return _op;
}

llvm::Value* ArithmeticExpr::getValue(){
    fprintf(stderr, "ArithmeticExpr::getValue() %s\n", "hi");
    IRGenerator &irgen = IRGenerator::getInstance();

    llvm::Value *leftOp = left->getValue();
    llvm::Value *rightOp = right->getValue();

    bool isFloat = leftOp->getType()->isFloatTy();

    llvm::BinaryOperator::BinaryOps binOp = toBinaryOps(isFloat, op);

    return llvm::BinaryOperator::Create(binOp, leftOp, rightOp, "", irgen.GetBasicBlock());
}

llvm::Value* PostfixExpr::getValue(){
    fprintf(stderr, "PostfixExpr::getValue()\n");
    return NULL;
}

llvm::Value* AssignExpr::getValue(){
    fprintf(stderr, "AssignExpr::getValue()\n");
    return NULL;
}

llvm::Value* EqualityExpr::getValue(){
    fprintf(stderr, "EqualityExpr::getValue()\n");
    return NULL;
}
llvm::Value* RelationalExpr::getValue(){
    fprintf(stderr, "RelationalExpr::getValue()\n");
    return NULL;
}

void FieldAccess::PrintChildren(int indentLevel) {
    if (base) base->Print(indentLevel+1);
    field->Print(indentLevel+1);
}

Call::Call(yyltype loc, Expr *b, Identifier *f, List<Expr*> *a) : Expr(loc)  {
    Assert(f != NULL && a != NULL); // b can be be NULL (just means no explicit base)
    base = b;
    if (base) base->SetParent(this);
    (field=f)->SetParent(this);
    (actuals=a)->SetParentAll(this);
}

void Call::PrintChildren(int indentLevel) {
   if (base) base->Print(indentLevel+1);
   if (field) field->Print(indentLevel+1);
   if (actuals) actuals->PrintAll(indentLevel+1, "(actuals) ");
}
