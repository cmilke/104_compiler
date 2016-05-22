#include <stdio.h>
#include <iostream>
#include "symtable.h"
#include "switch_functions.h"



vector<symbol_table*> symbol_stack;
int block_number = 0;
vector<int> block_stack;


void create_symbol_table(astree* yyparse_astree) {
    block_stack.push_back(block_number);
    symbol_stack.push_back(nullptr);

    for( astree* child : yyparse_astree->children ) {
        invoke_switchboard(child);
    }
}



attr_bitset invoke_switchboard(astree* root) {
    switch(root->symbol) {
        case '=':            return switch_assignment(root); break;
        case TOK_VOID:       return update_node(root,0x10000); break;
        case TOK_BOOL:       return update_node(root,0x08000); break;
        case TOK_CHAR:       return update_node(root,0x04000); break;
        case TOK_INT:        return update_node(root,0x02000); break;
        case TOK_STRING:     return update_node(root,0x00800); break;
        case TOK_IF:         return switch_tok_if(root); break;
        case TOK_ELSE:       return switch_tok_else(root); break;
        case TOK_WHILE:      return switch_tok_while(root); break;
        case TOK_RETURN:     return switch_tok_return(root); break;
        case TOK_STRUCT:     return switch_tok_struct(root); break;
        case TOK_FALSE:      return update_node(root,0x08004); break;
        case TOK_TRUE:       return update_node(root,0x08004); break;
        case TOK_NULL:       return switch_tok_null(root); break;
        case TOK_NEW:        return switch_tok_new(root); break;
        case TOK_ARRAY:      return switch_tok_array(root); break;
        case TOK_EQ:         return switch_tok_eq(root); break;
        case TOK_NE:         return switch_tok_ne(root); break;
        case TOK_LT:         return switch_tok_lt(root); break;
        case TOK_LE:         return switch_tok_le(root); break;
        case TOK_GT:         return switch_tok_gt(root); break;
        case TOK_GE:         return switch_tok_ge(root); break;
        case TOK_IDENT:      return switch_tok_ident(root); break;
        case TOK_INTCON:     return update_node(root,0x02004); break;
        case TOK_CHARCON:    return update_node(root,0x04004); break;
        case TOK_STRINGCON:  return update_node(root,0x00804); break;
        case TOK_BLOCK:      return switch_tok_block(root); break;
        case TOK_CALL:       return switch_tok_call(root); break;
        case TOK_IFELSE:     return switch_tok_ifelse(root); break;
        case TOK_DECLID:     return switch_tok_declid(root); break;
        case TOK_PARAMLIST:  return switch_tok_paramlist(root); break;
        case TOK_POS:        return switch_tok_pos(root); break;
        case TOK_NEG:        return switch_tok_neg(root); break;
        case TOK_NEWARRAY:   return switch_tok_newarray(root); break;
        case TOK_TYPEID:     return switch_tok_typeid(root); break;
        case TOK_FIELD:      return switch_tok_field(root); break;
        case TOK_ORD:        return switch_tok_ord(root); break;
        case TOK_CHR:        return switch_tok_chr(root); break;
        case TOK_ROOT:       return switch_tok_root(root); break;
        case TOK_FUNCTION:   return switch_tok_function(root); break;
        case TOK_TEMP:       return switch_tok_temp(root); break;
        case TOK_VARDECL:    return switch_tok_vardecl(root); break;
        case TOK_RETURNVOID: return switch_tok_returnvoid(root); break;
        case TOK_NEWSTRING:  return switch_tok_newstring(root); break;
        case TOK_ERROR:      return switch_tok_error(root); break;
        default:             return switch_error(root); break;
    }
}



attr_bitset update_node( astree* root, unsigned long val ) {
    attr_bitset new_bits(val);
    root->block_nr = block_stack.back();
    root->attributes = new_bits;
    return new_bits;
}



attr_bitset update_binary( astree* root ) {
    astree* lval = root->children[0];
    astree* rval = root->children[1];

    attr_bitset ltype = invoke_switchboard(lval);
    attr_bitset rtype = invoke_switchboard(rval);
    attr_bitset bit_value = compare_types(ltype,rtype);

    return bit_value;
}



attr_bitset switch_assignment( astree* root ) {
    printf("UNIMPLEMENTED %d\n",root->symbol);
    return -1;
}



attr_bitset switch_tok_if( astree* root ) {
    printf("UNIMPLEMENTED %d\n",root->symbol);
    return -1;
}



attr_bitset switch_tok_else( astree* root ) {
    printf("UNIMPLEMENTED %d\n",root->symbol);
    return -1;
}



attr_bitset switch_tok_while( astree* root ) {
    printf("UNIMPLEMENTED %d\n",root->symbol);
    return -1;
}



attr_bitset switch_tok_return( astree* root ) {
    printf("UNIMPLEMENTED %d\n",root->symbol);
    return -1;
}



attr_bitset switch_tok_struct( astree* root ) {
    printf("UNIMPLEMENTED %d\n",root->symbol);
    return -1;
}



attr_bitset switch_tok_false( astree* root ) {
    printf("UNIMPLEMENTED %d\n",root->symbol);
    return -1;
}



attr_bitset switch_tok_true( astree* root ) {
    printf("UNIMPLEMENTED %d\n",root->symbol);
    return -1;
}



attr_bitset switch_tok_null( astree* root ) {
    printf("UNIMPLEMENTED %d\n",root->symbol);
    return -1;
}



attr_bitset switch_tok_new( astree* root ) {
    printf("UNIMPLEMENTED %d\n",root->symbol);
    return -1;
}



attr_bitset switch_tok_array( astree* root ) {
    printf("UNIMPLEMENTED %d\n",root->symbol);
    return -1;
}



attr_bitset switch_tok_eq( astree* root ) {
    printf("UNIMPLEMENTED %d\n",root->symbol);
    return -1;
}



attr_bitset switch_tok_ne( astree* root ) {
    printf("UNIMPLEMENTED %d\n",root->symbol);
    return -1;
}



attr_bitset switch_tok_lt( astree* root ) {
    printf("UNIMPLEMENTED %d\n",root->symbol);
    return -1;
}



attr_bitset switch_tok_le( astree* root ) {
    printf("UNIMPLEMENTED %d\n",root->symbol);
    return -1;
}



attr_bitset switch_tok_gt( astree* root ) {
    printf("UNIMPLEMENTED %d\n",root->symbol);
    return -1;
}



attr_bitset switch_tok_ge( astree* root ) {
    printf("UNIMPLEMENTED %d\n",root->symbol);
    return -1;
}



attr_bitset switch_tok_ident( astree* root ) {
    printf("UNIMPLEMENTED %d\n",root->symbol);
    return -1;
}



attr_bitset switch_tok_block( astree* root ) {
    block_number++;
    block_stack.push_back(block_number);

    symbol_stack.push_back(nullptr);
    for( astree* child : root->children ) {
        invoke_switchboard(child);
    }

    block_stack.pop_back();
    return -1;
}



attr_bitset switch_tok_call( astree* root ) {
    printf("UNIMPLEMENTED %d\n",root->symbol);
    return -1;
}



attr_bitset switch_tok_ifelse( astree* root ) {
    printf("UNIMPLEMENTED %d\n",root->symbol);
    return -1;
}



attr_bitset switch_tok_declid( astree* root ) {
    printf("UNIMPLEMENTED %d\n",root->symbol);
    return -1;
}



attr_bitset switch_tok_paramlist( astree* root ) {
    printf("UNIMPLEMENTED %d\n",root->symbol);
    return -1;
}



attr_bitset switch_tok_pos( astree* root ) {
    printf("UNIMPLEMENTED %d\n",root->symbol);
    return -1;
}



attr_bitset switch_tok_neg( astree* root ) {
    printf("UNIMPLEMENTED %d\n",root->symbol);
    return -1;
}



attr_bitset switch_tok_newarray( astree* root ) {
    printf("UNIMPLEMENTED %d\n",root->symbol);
    return -1;
}



attr_bitset switch_tok_typeid( astree* root ) {
    printf("UNIMPLEMENTED %d\n",root->symbol);
    return -1;
}



attr_bitset switch_tok_field( astree* root ) {
    printf("UNIMPLEMENTED %d\n",root->symbol);
    return -1;
}



attr_bitset switch_tok_ord( astree* root ) {
    printf("UNIMPLEMENTED %d\n",root->symbol);
    return -1;
}



attr_bitset switch_tok_chr( astree* root ) {
    printf("UNIMPLEMENTED %d\n",root->symbol);
    return -1;
}



attr_bitset switch_tok_root( astree* root ) {
    printf("UNIMPLEMENTED %d\n",root->symbol);
    return -1;
}



attr_bitset switch_tok_function( astree* root ) {
    printf("UNIMPLEMENTED %d\n",root->symbol);
    return -1;
}



attr_bitset switch_tok_temp( astree* root ) {
    printf("UNIMPLEMENTED %d\n",root->symbol);
    return -1;
}



attr_bitset switch_tok_vardecl( astree* root ) {
    astree* lval = root->children[0];
    astree* rval = root->children[1];

    attr_bitset rtype = invoke_switchboard(rval);
    attr_bitset ltype = invoke_switchboard(lval->children[0]);

    int is_array = lval->children.size() - 1; 
    if (is_array) ltype |= attr_bitset(0x00200);

    attr_bitset bit_value = compare_types(ltype,rtype);

    if ( symbol_stack.back() == nullptr ) {
        symbol_table new_table;
        symbol_stack.push_back(&new_table);
    }
    
    symbol_table* symtable = symbol_stack.back();

    if (symtable->find(lval->lexinfo) != symtable->end()) {
        printf("FAILURE - SYMBOL ALREADY DECLARED!!!\n");

    } else {
        attr_bitset lval_var_bits = bit_value | attr_bitset(0x0088);


        symbol* new_node = (symbol*) malloc( sizeof(symbol) );
        new_node->attributes = lval_var_bits;
        new_node->fields = nullptr;
        new_node->filenr = lval->filenr;
        new_node->linenr = lval->linenr;
        new_node->offset = lval->offset;
        new_node->block_nr = block_stack.back();
        new_node->parameters = nullptr;
        symtable->emplace(lval->lexinfo,new_node);

        lval->block_nr = block_stack.back();
        lval->attributes = lval_var_bits;
    }

    attr_bitset vardecl_bits(0x00002);
    root->block_nr = block_stack.back();
    root->attributes = vardecl_bits | bit_value;

    return 0;
}



attr_bitset switch_tok_returnvoid( astree* root ) {
    printf("UNIMPLEMENTED %d\n",root->symbol);
    return -1;
}



attr_bitset switch_tok_newstring( astree* root ) {
    printf("UNIMPLEMENTED %d\n",root->symbol);
    return -1;
}



attr_bitset switch_tok_error( astree* root ) {
    printf("UNIMPLEMENTED %d\n",root->symbol);
    return -1;
}



attr_bitset switch_error( astree* root ) {
    printf("UNIMPLEMENTED %d: ERROR ENCOUNTERED!!!\n", root->symbol);
    return -1;
}
