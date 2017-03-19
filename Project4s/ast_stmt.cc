/* File: ast_stmt.cc
 * -----------------
 * Implementation of statement node classes.
 */
#include "ast_stmt.h"
#include "ast_type.h"
#include "ast_decl.h"
#include "ast_expr.h"
#include "symtable.h"

#include "irgen.h"
#include "llvm/Bitcode/ReaderWriter.h"
#include "llvm/Support/raw_ostream.h"

#include "utility.h"

Program::Program(List<Decl*> *d) {
    Assert(d != NULL);
    (decls=d)->SetParentAll(this);
}

void Program::PrintChildren(int indentLevel) {
    decls->PrintAll(indentLevel+1);
    printf("\n");
}

void Program::Emit() {
    IRGenerator &irgen = IRGenerator::getInstance();
    SymbolTable &symtab = SymbolTable::getInstance();
    llvm::Module *mod = irgen.GetOrCreateModule("foo.bc");

    for (int i = 0; i < decls->NumElements(); ++i) {
        Decl *d = decls->Nth(i);
        // fprintf(stderr, "Decl %s\n\n",d->GetIdentifier()->GetName());
        d->Emit();
    }

    // write the BC into standard output
    llvm::WriteBitcodeToFile(mod, llvm::outs());

    //uncomment the next line to generate the human readable/assembly file
    // mod->dump();
}

StmtBlock::StmtBlock(List<VarDecl*> *d, List<Stmt*> *s) {
    Assert(d != NULL && s != NULL);
    (decls=d)->SetParentAll(this);
    (stmts=s)->SetParentAll(this);
}

void StmtBlock::Emit() {
  // fprintf(stderr, "StmtBlock::Emit\n\n");
  SymbolTable &symTable = SymbolTable::getInstance();
  IRGenerator &irgen = IRGenerator::getInstance();


  // Enter block scope
  // symTable.push();

  for (int i = 0; i < stmts->NumElements(); i++) {
       if (irgen.GetBasicBlock()->getTerminator()){ break; }
       Stmt* st = stmts->Nth(i);
        // fprintf(stderr, "================================================\n");
    //    fprintf(stderr, "StmtBlock Stmt ForLoop: %s\n\n",st->GetPrintNameForNode());
       st->Emit();
  }

  // Leave block scope
  // symTable.pop();
}

void StmtBlock::PrintChildren(int indentLevel) {
    decls->PrintAll(indentLevel+1);
    stmts->PrintAll(indentLevel+1);
}

DeclStmt::DeclStmt(Decl *d) {
    Assert(d != NULL);
    (decl=d)->SetParent(this);
}

void DeclStmt::PrintChildren(int indentLevel) {
    decl->Print(indentLevel+1);
}

void DeclStmt::Emit() {
    // fprintf(stderr, "DeclStmt::Emit\n\n");
    decl->Emit();
}

ConditionalStmt::ConditionalStmt(Expr *t, Stmt *b) {
    Assert(t != NULL && b != NULL);
    (test=t)->SetParent(this);
    (body=b)->SetParent(this);
}

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

void WhileStmt::PrintChildren(int indentLevel) {
    test->Print(indentLevel+1, "(test) ");
    body->Print(indentLevel+1, "(body) ");
}

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

void IfStmt::Emit(){
    fprintf(stderr, "IfStmt::Emit\n");
    SymbolTable &symtab = SymbolTable::getInstance();
    IRGenerator &irgen = IRGenerator::getInstance();

    llvm::BasicBlock *thenBB = irgen.createFunctionBlock("ThenBB");

    llvm::BasicBlock *footerBB = NULL; // Cannot create footerBB yet otherwise
                                       // it might be out of order if elseBody exists
    llvm::BasicBlock *elseBB = NULL;

    // create elseBB if elseBody exists
    if (elseBody) {
        elseBB = irgen.createFunctionBlock("ElseBB");
        footerBB = irgen.createFunctionBlock("footerBB");
        irgen.branchConditionally(thenBB, elseBB, test->getValue());
    } else {
        footerBB = irgen.createFunctionBlock("footerBB");
        irgen.branchConditionally(thenBB, footerBB, test->getValue());
    }

    //===== thenBB begin ======
    irgen.SetBasicBlock(thenBB);

    symtab.push();
    body->Emit();
    symtab.pop();

    irgen.setTerminator(footerBB);
    //===== thenBB end ======


    //===== elseBB begin ======
    if (elseBody) { // Check if elseBody exists
        irgen.SetBasicBlock(elseBB);

        symtab.push();
        elseBody->Emit();
        symtab.pop();

        // terminate elseBB
        irgen.setTerminator(footerBB);
    }
    //===== elseBB end ======


    // set footerBB to exit
    irgen.SetBasicBlock(footerBB);
}

ReturnStmt::ReturnStmt(yyltype loc, Expr *e) : Stmt(loc) {
    expr = e;
    if (e != NULL) expr->SetParent(this);
}

void ReturnStmt::PrintChildren(int indentLevel) {
    if ( expr )
      expr->Print(indentLevel+1);
}

void ReturnStmt::Emit(){
    // fprintf(stderr, "ReturnStmt::Emit\n\n");
    SymbolTable &symTable = SymbolTable::getInstance();
    IRGenerator &irgen = IRGenerator::getInstance();

    llvm::LLVMContext *context = irgen.GetContext();
    llvm::Value *returnValue = NULL;

    // fprintf(stderr, "ReturnStmt::getValue: %s\n\n", expr->GetPrintNameForNode());
    if (expr) {
        // Symbol *sym = symtab.find(expr->GetName());
        // llvm::Value *v = sym->value;
        returnValue = expr->getValue();
    }
    // fprintf(stderr, "ReturnStmt::Create\n\n");
    llvm::ReturnInst::Create(*context, returnValue, irgen.GetBasicBlock());

}


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
