#ifndef __TYPECHECKING_H__
#define __TYPECHECKING_H__

#include <bitset>

enum { ATTR_void,
       ATTR_bool,     ATTR_char,   ATTR_int,    ATTR_null,
       ATTR_string,   ATTR_struct, ATTR_array,  ATTR_function,
       ATTR_variable, ATTR_field,  ATTR_typeid, ATTR_param,
       ATTR_lval,     ATTR_const,  ATTR_vreg,   ATTR_vaddr,
       
       ATTR_bitset_size,
    };


using attr_bitset = std::bitset<ATTR_bitset_size>;

struct symbol;
struct astree;

void throw_error(astree* root1, astree* root2, std::string error);
void throw_error(symbol* sym1, symbol* sym2, std::string error);


#endif
