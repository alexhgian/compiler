/* File: ast_stmt.cc
 * -----------------
 * Implementation of statement node classes.
 */
#include "ast_stmt.h"
#include "ast_type.h"
#include "ast_decl.h"
#include "ast_expr.h"
#include "errors.h"
#include "symtable.h"
#include <string.h>

Program::Program(List<Decl*> *d) {
    Assert(d != NULL);
    (decls=d)->SetParentAll(this);

}

void Program::PrintChildren(int indentLevel) {
    decls->PrintAll(indentLevel+1);
    printf("\n");
}

void Program::Check() {
     PrintDebug("stmtCheck", "Program::Check()\n");
    /* pp3: here is where the semantic analyzer is kicked off.
     *      The general idea is perform a tree traversal of the
     *      entire program, examining all constructs for compliance
     *      with the semantic rules.  Each node can have its own way of
     *      checking itself, which makes for a great use of inheritance
     *      and polymorphism in the node classes.
     */

    // sample test - not the actual working code
    // replace it with your own implementation
    if ( decls->NumElements() > 0 ) {
      for ( int i = 0; i < decls->NumElements(); ++i ) {
        Decl *d = decls->Nth(i);
        d->Check();
        /* !!! YOUR CODE HERE !!!
         * Basically you have to make sure that each declaration is
         * semantically correct.
         */
      }
    }
}

StmtBlock::StmtBlock(List<VarDecl*> *d, List<Stmt*> *s) {
    Assert(d != NULL && s != NULL);
    (decls=d)->SetParentAll(this);
    (stmts=s)->SetParentAll(this);
}

void StmtBlock::PrintChildren(int indentLevel) {
    decls->PrintAll(indentLevel+1);
    stmts->PrintAll(indentLevel+1);
}

/**
* Check()
*/
void StmtBlock::Check(){
     PrintDebug("stmtCheck", "StmtBlock::Check()\n");

    int numOfDecls = decls->NumElements();
    for (int i = 0; i < numOfDecls; i++){
        decls->Nth(i)->Check();
        PrintDebug("stmtCheck", "StmtBlock decl loop: %s\n",decls->Nth(i)->GetIdentifier());
    }

    int numOfStmt = stmts->NumElements();
    for (int i = 0; i < numOfStmt; i++){
        PrintDebug("stmtCheck", "StmtBlock stmt loop: %s\n",stmts->Nth(i)->GetPrintNameForNode());

        // Handle if statement is in function scope or block scope
        if( strcmp(stmts->Nth(i)->GetPrintNameForNode(), "StmtBlock") == 0 ){
            PrintDebug("stmtCheck", "StmtBlock creating new scope\n");
            // ----- START block scope -----
            symbolTable->push();
            stmts->Nth(i)->Check();
            symbolTable->pop();
            // ----- END block scope -----
        } else {
            stmts->Nth(i)->Check();
        }
    }
}

/*
* DeclStmt
*/
DeclStmt::DeclStmt(Decl *d) {
    Assert(d != NULL);
    (decl=d)->SetParent(this);
}

void DeclStmt::PrintChildren(int indentLevel) {
    decl->Print(indentLevel+1);
}

void DeclStmt::Check(){
    PrintDebug("stmtCheck", "DeclStmt::Check()\n");
    decl->Check();
}

/**
* TODO:
*/
void BreakStmt::Check() {
    bool canBreak = symbolTable->isBreakable();
    PrintDebug("stmtCheck", "breakable %d\n", canBreak);
    if (!canBreak){
      ReportError::BreakOutsideLoop(this);
    }
}

/*
* ConditionalStmt
*/
ConditionalStmt::ConditionalStmt(Expr *t, Stmt *b) {
    Assert(t != NULL && b != NULL);
    (test=t)->SetParent(this);
    (body=b)->SetParent(this);
}

/**
* TODO:
*/
void ContinueStmt::Check() {
    bool canCont = symbolTable->isContinuable();
    PrintDebug("stmtCheck", "continuable %d\n", canCont);
    if (!canCont){
      ReportError::ContinueOutsideLoop(this);
    }
}

/*
* ForStmt
*/
ForStmt::ForStmt(Expr *i, Expr *t, Expr *s, Stmt *b): LoopStmt(t, b) {
    Assert(i != NULL && t != NULL && b != NULL);
    (init=i)->SetParent(this);
    step = s;
    if ( s )
      (step=s)->SetParent(this);
}

void ForStmt::PrintChildren(int indentLevel) {
    init->Print(indentLevel+1, "(init) ");
    test->Print(indentLevel+1, "(test) ");
    if ( step )
      step->Print(indentLevel+1, "(step) ");
    body->Print(indentLevel+1, "(body) ");
}
void ForStmt::Check() {
  // init->Check();

    // bool isBool = test->checkType()->IsBool();
    // if (!isBool) {
    //     ReportError::TestNotBoolean(test);
    // }

  // step->Check();

  symbolTable->push();

  symbolTable->setBreakable(true);
  symbolTable->setContinuable(true);

  body->Check();
  symbolTable->pop();
}

/*
* WhileStmt
*/
void WhileStmt::PrintChildren(int indentLevel) {
    test->Print(indentLevel+1, "(test) ");
    body->Print(indentLevel+1, "(body) ");
}

void WhileStmt::Check() {
  // if (!test->CheckAndGetType()->IsBool())
  //   ReportError::TestNotBoolean(test);

  symbolTable->push();

  symbolTable->setBreakable(true);
  symbolTable->setContinuable(true);

  body->Check();
  symbolTable->pop();
}

/*
* IfStmt
*/
IfStmt::IfStmt(Expr *t, Stmt *tb, Stmt *eb): ConditionalStmt(t, tb) {
    Assert(t != NULL && tb != NULL); // else can be NULL
    elseBody = eb;
    if (elseBody) elseBody->SetParent(this);
}

void IfStmt::PrintChildren(int indentLevel) {
    if (test) test->Print(indentLevel+1, "(test) ");
    if (body) body->Print(indentLevel+1, "(then) ");
    if (elseBody) elseBody->Print(indentLevel+1, "(else) ");
}
void IfStmt::Check() {
    PrintDebug("stmtCheck", "IfStmt %s\n",  body->GetPrintNameForNode());
    symbolTable->push();
    body->Check();
    symbolTable->pop();

    if(elseBody){
        symbolTable->push();
        elseBody->Check();
        symbolTable->pop();
    }
}

/*
* ReturnStmt
*/
ReturnStmt::ReturnStmt(yyltype loc, Expr *e) : Stmt(loc) {
    expr = e;
    if (e != NULL) expr->SetParent(this);
}

void ReturnStmt::PrintChildren(int indentLevel) {
    if ( expr )
      expr->Print(indentLevel+1);
}

void ReturnStmt::Check(){
    Type* expect = symbolTable->getLastFn()->GetType();
    Type* actual = expr->checkType();
    // PrintDebug("stmtCheck", "fn type: %s\n", expect->getTypeName());
    // PrintDebug("stmtCheck", "ReturnStmt %s | %s\n", actual->GetPrintNameForNode(), actual->getTypeName());
    if(!expect->IsEquivalentTo(actual)){
        ReportError::ReturnMismatch(this, Type::errorType, Type::errorType);
    }
}

/*
* SwitchLabel
*/
SwitchLabel::SwitchLabel(Expr *l, Stmt *s) {
    Assert(l != NULL && s != NULL);
    (label=l)->SetParent(this);
    (stmt=s)->SetParent(this);
}

SwitchLabel::SwitchLabel(Stmt *s) {
    Assert(s != NULL);
    label = NULL;
    (stmt=s)->SetParent(this);
}

void SwitchLabel::PrintChildren(int indentLevel) {
    if (label) label->Print(indentLevel+1);
    if (stmt)  stmt->Print(indentLevel+1);
}

void Case::Check(){
  // Type* testExprType = NULL;
  // Type* labelType = label->CheckAndGetType();
  stmt->Check();
}
void Default::Check(){
  stmt->Check();
}


SwitchStmt::SwitchStmt(Expr *e, List<Stmt *> *c, Default *d) {
    Assert(e != NULL && c != NULL && c->NumElements() != 0 );
    (expr=e)->SetParent(this);
    (cases=c)->SetParentAll(this);
    def = d;
    if (def) def->SetParent(this);
}

void SwitchStmt::PrintChildren(int indentLevel) {
    if (expr) expr->Print(indentLevel+1);
    if (cases) cases->PrintAll(indentLevel+1);
    if (def) def->Print(indentLevel+1);
}

void SwitchStmt::Check() {
    // check expression
    // switch scope
    // symbolTable->push();
    symbolTable->setBreakable(true);

    int size = cases->NumElements();
    for (int i = 0; i < size; i++) {
        cases->Nth(i)->Check();
    }

    if (def) {
        def->Check();
    }
    symbolTable->setBreakable(false);
    // symbolTable->pop();
}
