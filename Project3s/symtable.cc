/*
 * file: symtable.cc
 * author: Alex Gian, Nazior Rahman
 */
#include "symtable.h"


 /**
 * ScopedTable
 */
 ScopedTable::ScopedTable(){

 }

void ScopedTable::insert(Symbol &sym){
    // When inserting it should check for redeclaration
    // using Symbol *find(const char *name);
    // to see if we are redeclaring in the same scope
    // symbol.
    if( this->find(sym.name) ){
        // throw redeclartion error here
        // ReportError::DeclConflict
        return;
    }
    symbols.insert(pair<const char *, Symbol>(sym.name, sym));
}

Symbol *ScopedTable::find(const char *name){
    return NULL;
}

/**
* SymbolTable
*/
SymbolTable::SymbolTable(){

}

void SymbolTable::insert(Symbol &sym){
    ScopedTable *st = tables.back();
    st->insert(sym);
    // Push scope table in symbol tab table
}

void SymbolTable::push(){
    tables.push_back(new ScopedTable());
}

void SymbolTable::pop(){
    tables.pop_back();
}
