#ifndef __SYMTABLE_H__
#define __SYMTABLE_H__

#include <vector>
#include <unordered_map>
#include "typechecking.h"
#include "astree.h"
#include "yyparse.h"

using namespace std;

struct symbol;
using symbol_table = unordered_map<const string*, symbol*>;
using symbol_entry = symbol_table::value_type;

struct symbol {
    const string* key;
    attr_bitset attributes;
    symbol_table* fields;
    size_t filenr, linenr, offset;
    size_t block_nr;
    vector<symbol*>* parameters;
    symbol* identifier;
};

void create_symbol_table(astree* yyparse_astree);



#endif
