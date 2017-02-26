/*
 * file: symtable.cc
 * author: Alex Gian, Nazior Rahman
 */
#include "symtable.h"
#include "utility.h"

 /**
 * ScopedTable
 * TODO:
 * [] destructor
 * [x] constructor
 * [x] insert
 * [x] remove
 * [x] find
 */
 ScopedTable::ScopedTable(){
    //  SetDebugForKey("symtable", false);
    //  PrintDebug("symtable", "Init ScopeTable - symbols.size() %d\n", (int)symbols.size());
 }

 ScopedTable::~ScopedTable(){
     // Do Clean Up
 }

void ScopedTable::insert(Symbol &sym){
    // When inserting it should check for redeclaration
    // using Symbol *find(const char *name);
    // to see if we are redeclaring in the same scope
    // symbol.
    SymbolIterator it = symbols.find(sym.name);

    if( it == symbols.end() ){
        // printf("Inserting new decl: %s\n", sym.name);
        // symbols[sym.name] = sym;
    } else {
        // throw redeclartion error here
        Decl *tmpDecl = this->find(sym.name)->decl;
        ReportError::DeclConflict(sym.decl, tmpDecl);
    }
    symbols.insert(SymPair(sym.name, sym));

}

void ScopedTable::remove(Symbol &sym){
    symbols.erase(sym.name);
}

Symbol *ScopedTable::find(const char *name){
    // Search for the symbol
    SymbolIterator it = symbols.find(name);
    // dereference the iterator's element (Symbol)
    return &it->second;
}

/**
* SymbolTable
* TODO:
* [] destructor
* [x] constructor
* [] push
* [] pop
* [x] insert
* [x] remove
* [x] find
*/
SymbolTable::SymbolTable(){
    // Create a new scope (global scope)
    this->push();
}

/**
* insert
* Adds a symbol into the symbol table (map)
*/
void SymbolTable::insert(Symbol &sym){
    ScopedTable *st = tables.back();
    st->insert(sym);
}
/**
*
*/
void SymbolTable::remove(Symbol &sym){
    ScopedTable *st = tables.back();
    st->remove(sym);
}

void SymbolTable::push(){
    tables.push_back(new ScopedTable());
}

void SymbolTable::pop(){
    tables.pop_back();
}

Symbol *SymbolTable::find(const char* name){
    ScopedTable *st = tables.back();
    return st->find(name);
    // for(int i = 0; i < tables.size(); i++){
    //     Symbol * res = tables[i]->find(name);
    //     if(res){
    //         return res;
    //     }
    // }
    // return NULL;
}
