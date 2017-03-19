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

IntConstant::IntConstant(int val): Expr(){
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
    fprintf(stderr, "VarExpr:getValue\n");
    SymbolTable &symtab = SymbolTable::getInstance();
    IRGenerator &irgen = IRGenerator::getInstance();

    Symbol *sym = symtab.find(id->GetName());
    llvm::Value *val = NULL;

    // fprintf(stderr, "VarExpr id->GetName(): %s\n", id->GetName());
    // fprintf(stderr, "VarExpr symbol name: %s\n", sym->name);
    if(sym){
        // fprintf(stderr, "Symbol found: true | %s\n", sym->name );
        val = sym->value;
        VarDecl* varDecl = dynamic_cast<VarDecl*>(sym->decl);
        // if (varDecl) {
        //     this->type = varDecl->GetType();
        // }
    } else {
        // fprintf(stderr, "Symbol found: false\n");
    }

    // v->dump();
    llvm::BasicBlock *bb = irgen.GetBasicBlock();
    llvm::LoadInst *inst = new llvm::LoadInst(val, "", bb);
    // fprintf(stderr, "VarExpr LoadInst\n");
    return inst;
}

llvm::Value *VarExpr::store(llvm::Value *rVal, bool isVolatile){
    SymbolTable &symtab = SymbolTable::getInstance();
    IRGenerator &irgen = IRGenerator::getInstance();

    // Find the value for lhs var
    const char *varName = id->GetName();
    Symbol * sym = symtab.find(varName);

    llvm::StoreInst *storeInst = new llvm::StoreInst(rVal, sym->value, false, irgen.GetBasicBlock());
    storeInst->setVolatile(isVolatile);
    return storeInst;
}

Operator::Operator(yyltype loc, const char *tok) : Node(loc) {
    Assert(tok != NULL);
    strncpy(tokenString, tok, sizeof(tokenString));
}
Operator::Operator(const char *tok): Node() {
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

llvm::BinaryOperator::BinaryOps getBinaryOps(bool isFloat, Operator *op){
    llvm::BinaryOperator::BinaryOps _op;
    if (op->IsOp("+") || op->IsOp("++")) {
        if(isFloat) {
            _op = llvm::BinaryOperator::FAdd;
        } else {
            _op = llvm::BinaryOperator::Add;
        }
    } else if (op->IsOp("-")|| op->IsOp("--")){
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

llvm::BinaryOperator::BinaryOps getLogicalBinaryOps(Operator *op){
    if (op->IsOp("&&")) {
        return llvm::BinaryOperator::And;
    } else {
        return llvm::BinaryOperator::Or;
    }
}

llvm::CmpInst::OtherOps getCmpOp(bool isFloat){
    if(isFloat){
        return llvm::FCmpInst::FCmp;
    } else {
        return llvm::ICmpInst::ICmp;
    }
}

llvm::CmpInst::Predicate getEqualityOP(bool isFloat, Operator *op){
    if (op->IsOp("==")) { // Equal
        if(isFloat) {
            return llvm::CmpInst::FCMP_OEQ;
        } else {
            return llvm::CmpInst::ICMP_EQ;
        }

    } else { // Not Equal
        if(isFloat) {
            return llvm::CmpInst::FCMP_ONE;
        } else {
            return llvm::CmpInst::ICMP_NE;
        }
    }
}

llvm::CmpInst::Predicate getRelationalOP(bool isFloat, Operator *op){
    if (op->IsOp("<")) {
        if (isFloat ){
            return llvm::CmpInst::FCMP_OLT;
        } else {
            return llvm::CmpInst::ICMP_SLT;
        }
    } else if (op->IsOp("<=")) {
        if (isFloat ){
            return llvm::CmpInst::FCMP_OLE;
        } else {
            return llvm::CmpInst::ICMP_SLE;
        }
    } else if (op->IsOp(">")) {
        if (isFloat ){
            return llvm::CmpInst::FCMP_OGT;
        } else {
            return llvm::CmpInst::ICMP_SGT;
        }
    } else {
        if (isFloat ){
            return llvm::CmpInst::FCMP_OGE;
        } else {
            return llvm::CmpInst::ICMP_SGE;
        }
    }
}

void ArithmeticExpr::Emit(){
    fprintf(stderr, "ArithmeticExpr::Emit(): %s\n", op->toString());
    this->getValue();
}

llvm::Value* ArithmeticExpr::getValue(){
    fprintf(stderr, "ArithmeticExpr::getValue()\n");
    IRGenerator &irgen = IRGenerator::getInstance();

    llvm::Value *leftOp;
    llvm::Value *rightOp;

    // Check if unary to set value to a constant for decrement/increment
    if(left){
        leftOp = left->getValue();
        rightOp = right->getValue();
    } else {
        Expr *one = new IntConstant(1);
        rightOp = one->getValue();
        leftOp = right->getValue();
    }

    bool isFloat = leftOp->getType()->isFloatTy();

    llvm::BinaryOperator::BinaryOps binOp = getBinaryOps(isFloat, op);

    llvm::Value *retVal = llvm::BinaryOperator::Create(binOp, leftOp, rightOp, "", irgen.GetBasicBlock());

    // Check if unary to determine if we should performa volatile store
    if(left){
        return retVal;
    } else {
        VarExpr *rightVar = dynamic_cast<VarExpr*>(right);
        rightVar->store(retVal, true);
        return retVal;
    }
}

/**
* PostfixExpr::Emit
* Handles postfix bitcode i.e. x++
* 1) Return value before evaluating unary expression
* 2) Calculate unary expr
*
* TODO: Handle swizzle
*/
void PostfixExpr::Emit(){
    fprintf(stderr, "PostfixExpr::Emit()\n");
    // Return value before evaluating unary expression
    llvm::Value* assVal = this->getValue();
}

llvm::Value* PostfixExpr::getValue(){
    fprintf(stderr, "PostfixExpr::getValue()\n");
    IRGenerator &irgen = IRGenerator::getInstance();

    llvm::Value *orgVal = left->getValue();

    llvm::Value *leftOp;
    llvm::Value *rightOp;

    Expr *one = new IntConstant(1);
    rightOp = one->getValue();
    leftOp = orgVal;

    // Check if it is float
    bool isFloat = leftOp->getType()->isFloatTy();

    llvm::BinaryOperator::BinaryOps binOp = getBinaryOps(isFloat, op);
    llvm::Value *retVal = llvm::BinaryOperator::Create(binOp, leftOp, rightOp, "", irgen.GetBasicBlock());

    VarExpr *leftVar = dynamic_cast<VarExpr*>(left);
    leftVar->store(retVal, true);
    return orgVal;
}
void AssignExpr::Emit(){
    fprintf(stderr, "AssignExpr::Emit()\n");
    llvm::Value* assVal = this->getValue();
}

llvm::Value* AssignExpr::getValue(){
    fprintf(stderr, "AssignExpr::getValue()\n");
    IRGenerator &irgen = IRGenerator::getInstance();
    SymbolTable &symtab = SymbolTable::getInstance();

    llvm::Value *assignValue;

    if (op->IsOp("+=")){ // Handle basic assignment case: z += 2
        // fprintf(stderr, "AssignExpr op is `+=`\n");
        // Create a new ArithmeticExpr to handle addition of the left variable expr with the right expr
        // Using custom Operator to perform addition operation
        // ArithmeticExpr(Expr *lhs, Operator *op, Expr *rhs)
        ArithmeticExpr *aExpr = new ArithmeticExpr(left, new Operator("+"), right);
        llvm::Value *rVal = aExpr->getValue();

        // Return rhs
        assignValue = rVal;
    } else if (op->IsOp("-=")){ // Handle basic assignment case: z -= 2
        // fprintf(stderr, "AssignExpr op is `-=`\n");

        ArithmeticExpr *aExpr = new ArithmeticExpr(left, new Operator("-"), right);
        llvm::Value *rVal = aExpr->getValue();

        // Return rhs
        assignValue = rVal;
    }  else if (op->IsOp("*=")){ // Handle basic assignment case: z *= 2
        // fprintf(stderr, "AssignExpr op is `-=`\n");

        ArithmeticExpr *aExpr = new ArithmeticExpr(left, new Operator("*"), right);
        llvm::Value *rVal = aExpr->getValue();

        // Return rhs
        assignValue = rVal;
    } else if (op->IsOp("/=")){ // Handle basic assignment case: z /= 2
        // fprintf(stderr, "AssignExpr op is `-=`\n");

        ArithmeticExpr *aExpr = new ArithmeticExpr(left, new Operator("/"), right);
        llvm::Value *rVal = aExpr->getValue();

        // Return rhs
        assignValue = rVal;
    } else { // Handle basic assignment case: z = x + y;
        fprintf(stderr, "AssignExpr op is `=`\n");
        llvm::Value *rVal = right->getValue();
        assignValue = rVal;
    }
    fprintf(stderr, "AssignExpr storing\n");
    // Cast the lhs to a VarExpr and store rhs into lhs
    VarExpr *leftVar = dynamic_cast<VarExpr*>(left);
    leftVar->store(assignValue);

    return assignValue;
}

void EqualityExpr::Emit(){
    fprintf(stderr, "EqualityExpr::Emit()\n");
    this->getValue();
}

llvm::Value* EqualityExpr::getValue(){
    fprintf(stderr, "EqualityExpr::getValue()\n");
    IRGenerator &irgen = IRGenerator::getInstance();

    llvm::Value *lVal = left->getValue();
    llvm::Value *rVal = right->getValue();

    bool isFloat = lVal->getType()->isFloatTy() || lVal->getType()->isVectorTy();

    llvm::CmpInst::Predicate preOp = getEqualityOP(isFloat, op);

    llvm::Value *retVal = llvm::CmpInst::Create(getCmpOp(isFloat), preOp, lVal, rVal, "", irgen.GetBasicBlock());

    // TODO: Handle vector result w/ boolean

    return retVal;
}

llvm::Value* LogicalExpr::getValue(){
    fprintf(stderr, "LogicalExpr::getValue()\n");
    IRGenerator &irgen = IRGenerator::getInstance();
    SymbolTable &symtab = SymbolTable::getInstance();

    llvm::Value *leftOp = left->getValue();
    llvm::Value *rightOp = right->getValue();

    llvm::BinaryOperator::BinaryOps logicOp = getLogicalBinaryOps(op);

    return llvm::BinaryOperator::Create(logicOp, leftOp, rightOp, "", irgen.GetBasicBlock());
}

llvm::Value* RelationalExpr::getValue(){
    fprintf(stderr, "RelationalExpr::getValue()\n");
    IRGenerator &irgen = IRGenerator::getInstance();
    SymbolTable &symtab = SymbolTable::getInstance();

    llvm::Value *lVal = left->getValue();
    llvm::Value *rVal = right->getValue();

    bool isFloat = lVal->getType()->isFloatTy() || lVal->getType()->isVectorTy();

    llvm::CmpInst::Predicate preOp = getRelationalOP(isFloat, op);

    llvm::Value *retVal = llvm::CmpInst::Create(getCmpOp(isFloat), preOp, lVal, rVal, "", irgen.GetBasicBlock());

    return retVal;
}

void FieldAccess::PrintChildren(int indentLevel) {
    if (base) base->Print(indentLevel+1);
    field->Print(indentLevel+1);
}

llvm::Value* FieldAccess::getValue(){
    fprintf(stderr, "FieldAccess::getValue()\n");
    return NULL;
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
