#ifndef __SYMTABLE_H__
#define __SYMTABLE_H__

#include <unordered_map>
#include <vector>

#include "typechecking.h"

struct symbol;
using symbol_table = std::unordered_map<const std::string*, symbol*>;
using symbol_entry = symbol_table::value_type;


struct symbol {
    const std::string* key;
    attr_bitset attributes;
    symbol_table* fields;
    size_t filenr, linenr, offset;
    size_t block_nr;
    std::vector<symbol*>* parameters;
    symbol* identifier;
};

struct global_container;
global_container* create_symbol_table(astree* yyparse_astree);



#endif
