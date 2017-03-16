/*
 * Symbol table implementation
 *
 */

 /*
  * file: symtable.cc
  * author: Alex Gian, Nazior Rahman
  */
 #include "symtable.h"
 typedef pair<const char *, Symbol> SymPair;

  /**
  * ScopedTable
  */
  ScopedTable::ScopedTable(){
    //   printf("ScopeTable - symbols.size() %d\n", (int)symbols.size());
  }

 void ScopedTable::insert(Symbol &sym){
     // When inserting it should check for redeclaration
     // using Symbol *find(const char *name);
     // to see if we are redeclaring in the same scope
     // symbol.
     SymbolIterator it = symbols.find(sym.name);

     if( it == symbols.end() ){
        //  printf("Inserting new decl: %s\n", sym.name);
         // symbols[sym.name] = sym;
         symbols.insert(SymPair(sym.name, sym));
     } else {
         // throw redeclartion error here
         Decl *tmpDecl = this->find(sym.name)->decl;
         ReportError::DeclConflict(sym.decl, tmpDecl);
         symbols[sym.name] = sym;
     }

 }

 Symbol *ScopedTable::find(const char *name){
     // Search for the symbol
     SymbolIterator it = symbols.find(name);
     // dereference the iterator's element (Symbol)
     return &it->second;
 }

 /**
 * SymbolTable
 */
 SymbolTable::SymbolTable(){
     // Create a new scope (global scope)
     this->push();
 }

 void SymbolTable::insert(Symbol &sym){
     ScopedTable *st = tables.back();
     st->insert(sym);
     // Push scope table in symbol tab table
 }
 void ScopedTable::remove(Symbol &sym){
     symbols.erase(sym.name);
 }
 void SymbolTable::push(){
     tables.push_back(new ScopedTable());
 }

 void SymbolTable::pop(){
     tables.pop_back();
 }

 Symbol *SymbolTable::find(const char* name){
     ScopedTable *scope = tables.back();
     if(scope){
         return scope->find(name);
     }
     return NULL;
 }
