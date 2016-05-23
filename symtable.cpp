#include <stdio.h>
#include <iostream>
#include "symtable.h"
#include "switch_functions.h"



vector<symbol_table*> symbol_stack;
int block_number = 0;
vector<int> block_stack;


attr_bitset typemask = attr_bitset(0x1ff20);


void create_symbol_table(astree* yyparse_astree) {
    block_stack.push_back(block_number);
    symbol_stack.push_back(new symbol_table);

    for( astree* child : yyparse_astree->children ) {
        invoke_switchboard(child);
    }
}



attr_bitset invoke_switchboard(astree* root) {
    switch(root->symbol) {
        case '+': //TODO: maybe needed for strings?
        case '-':
        case '*':
        case '/':
        case '%':            return int_op(root); break;
        case '=':            return update_binary(root); break;
        case TOK_VOID:       return update_node(root,0x10000); break;
        case TOK_BOOL:       return update_node(root,0x08000); break;
        case TOK_CHAR:       return update_node(root,0x04000); break;
        case TOK_INT:        return update_node(root,0x02000); break;
        case TOK_STRING:     return update_node(root,0x00800); break;
        case TOK_IF:         return switch_tok_if(root); break; //TODO
        case TOK_ELSE:       return switch_tok_else(root); break; //TODO
        case TOK_WHILE:      return switch_tok_while(root); break; //TODO
        case TOK_RETURN:     return switch_tok_return(root); break; //TODO
        case TOK_STRUCT:     return switch_tok_struct(root); break; //TODO
        case TOK_FALSE:      return update_node(root,0x08004); break;
        case TOK_TRUE:       return update_node(root,0x08004); break;
        case TOK_NULL:       return switch_tok_null(root); break; //TODO
        case TOK_NEW:        return switch_tok_new(root); break; //TODO
        case TOK_ARRAY:      return switch_tok_array(root); break; //TODO
        case TOK_EQ:
        case TOK_NE:
        case TOK_LT:
        case TOK_LE:
        case TOK_GT:
        case TOK_GE:         return bool_op(root); break;
        case TOK_IDENT:      return switch_tok_ident(root); break;
        case TOK_INTCON:     return update_node(root,0x02004); break;
        case TOK_CHARCON:    return update_node(root,0x04004); break;
        case TOK_STRINGCON:  return update_node(root,0x00804); break;
        case TOK_BLOCK:      return switch_tok_block(root); break;
        case TOK_CALL:       return switch_tok_call(root); break; //TODO
        case TOK_IFELSE:     return switch_tok_ifelse(root); break; //TODO
        case TOK_POS:        return switch_tok_pos(root); break; //TODO
        case TOK_NEG:        return switch_tok_neg(root); break; //TODO
        case TOK_NEWARRAY:   return switch_tok_newarray(root); break; //TODO
        case TOK_TYPEID:     return switch_tok_typeid(root); break; //TODO
        case TOK_FIELD:      return switch_tok_field(root); break; //TODO
        case TOK_ORD:        return switch_tok_ord(root); break; //TODO
        case TOK_CHR:        return switch_tok_chr(root); break; //TODO
        case TOK_FUNCTION:
        case TOK_PROTOTYPE:  return switch_tok_prototype(root); break;
        case TOK_VARDECL:    return switch_tok_vardecl(root); break;
        case TOK_RETURNVOID: return switch_tok_returnvoid(root); break; //TODO
        case TOK_NEWSTRING:  return switch_tok_newstring(root); break; //TODO
        default:             return switch_error(root); break;
    }
}



symbol_table* get_current_symtable() {
    if ( symbol_stack.back() == nullptr ) {
        symbol_table* new_table = new symbol_table;
        symbol_stack.back() = new_table;
    }

    symbol_table* symtable = symbol_stack.back();
    return symtable;
}



attr_bitset update_node( astree* root, attr_bitset new_bits ) {
    root->block_nr = block_stack.back();
    root->attributes = new_bits;
    return new_bits;
}



attr_bitset update_node( astree* root, unsigned long val ) {
    return update_node( root, attr_bitset(val) );
}



attr_bitset get_declid_type( astree* declid, attr_bitset mask ) {
    attr_bitset dectype = invoke_switchboard(declid->children[0]);
    int is_array = declid->children.size() - 1; 
    if (is_array) dectype |= attr_bitset(0x00200);

    dectype |= mask;
    return update_node(declid,dectype);
}



symbol* generate_symbol( astree* tree, attr_bitset node_type,
            symbol_table* new_fields, vector<symbol*>* new_parameters ) {

        symbol* new_node = (symbol*) malloc( sizeof(symbol) );

        new_node->key = tree->lexinfo;
        new_node->attributes = node_type;
        new_node->fields = new_fields;
        new_node->filenr = tree->filenr;
        new_node->linenr = tree->linenr;
        new_node->offset = tree->offset;
        new_node->block_nr = block_stack.back();
        new_node->parameters = new_parameters;

        return new_node;
}



bool check_parameters( vector<symbol*>* first, vector<symbol*>* second) {
    size_t len = first->size();

    if ( len != second->size() ) return false;

    for (size_t i = 0; i < len; i++) {
        symbol* param1 = first->at(i); 
        symbol* param2 = second->at(i); 

        attr_bitset bits2 = param2->attributes;
        attr_bitset bits1 = param1->attributes;

        if (bits1 != bits2) return false;
    }

    return true;
}



vector<symbol*>* generate_params( astree* paramlist ) {
    vector<symbol*>* params = new vector<symbol*>;
    attr_bitset var_mask = attr_bitset(0x00010);

    for ( astree* child : paramlist->children ) {
        attr_bitset bits = get_declid_type(child,var_mask);
        symbol* newsym = generate_symbol(child,bits,nullptr,nullptr);
        params->push_back(newsym);
    }

    return params;
}



attr_bitset update_binary( astree* root, attr_bitset ltype ) {
    astree* lval = root->children[0];
    astree* rval = root->children[1];
    attr_bitset rtype = invoke_switchboard(rval);

    if ( (ltype&typemask) != (rtype&typemask) ) {
        string error = "BINARY OPERATOR USED ACROSS DIFFERENT TYPES";
        throw_error(lval,rval,error);
    }

    attr_bitset bits = (ltype&typemask) | attr_bitset(0x00002);
    return update_node(root,bits);
}



attr_bitset update_binary( astree* root ) {
    return update_binary(root,invoke_switchboard(root->children[0]));
}



attr_bitset bool_op( astree* root ) {
    attr_bitset optype = update_binary(root);
    if ( (optype&typemask) != attr_bitset(0x08000) ) {
        string error = "BOOLEAN OPERATOR USED ON NON-BOOLEAN TYPES";
        throw_error ( root->children[0], root->children[1], error);
    }
    return update_node(root,0x08002);
}



attr_bitset int_op( astree* root ) {
    attr_bitset optype = update_binary(root);
    if ( (optype&typemask) != attr_bitset(0x02000) ) {
        string error = "INT OPERATOR USED ON NON-INT TYPES";
        throw_error ( root->children[0], root->children[1], error);
    }
    return update_node(root,0x02002);
}



void activate_function(astree* root, vector<symbol*>* params) {
    block_number++;
    block_stack.push_back(block_number);

    symbol_stack.push_back(nullptr);
    if ( params->size() > 0 ) {
        symbol_table* symtable = get_current_symtable();

        for( symbol* param : *params ) symtable->emplace(param->key,param);
    }

    for( astree* child : root->children ) {
        invoke_switchboard(child);
    }

    block_stack.pop_back();
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



attr_bitset switch_tok_ident( astree* root ) {
    const string* key = root->lexinfo;

    for ( symbol_table* table : symbol_stack ) {
        if ( table == nullptr ) continue;
        table->find(key);

        if ( table->find(key) != table->end()) {
            symbol* orig = table->at(key);
            attr_bitset new_bits = orig->attributes;
            root->block_nr = block_stack.back();
            root->attributes = new_bits;
            return new_bits;
        }
    }
    printf("VARIABLE NEVER DECLARED\n");
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
    return 0;
}



attr_bitset switch_tok_call( astree* root ) {
    printf("UNIMPLEMENTED %d\n",root->symbol);
    return -1;
}



attr_bitset switch_tok_ifelse( astree* root ) {
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



attr_bitset switch_tok_prototype( astree* root ) {
    bool is_function = (root->symbol == TOK_FUNCTION);

    astree* paramlist = root->children[1];
    vector<symbol*>* params = generate_params(paramlist);

    astree* declid = root->children[0];
    attr_bitset bitmask = attr_bitset(0x0);
    if (is_function) bitmask = attr_bitset(0x00100);
    attr_bitset dectype = get_declid_type( declid, bitmask);
    const string* key = declid->lexinfo;

    symbol* newsym = generate_symbol(declid,dectype,nullptr,params);
    symbol_table* symtable = symbol_stack[0];

    if (symtable->find(key) != symtable->end()) {
        symbol* previous = symtable->at(declid->lexinfo);
        attr_bitset prevbits = previous->attributes;

        attr_bitset protomask = attr_bitset(0x1feff);
        if ( (prevbits&protomask) == (dectype&protomask) ) {
            if ( check_parameters(params, previous->parameters) ) {
                if (is_function) {
                    if ( !prevbits.test(8) ) {
                        symtable->emplace(key,newsym);
                        activate_function(root->children[2],params);
                    } else {
                        string error = "FUNCTION PREVIOUSLY DEFINED";
                        throw_error(newsym,previous,error);
                    }
                }
            } else {
                string error = "PREVIOUS DECLARATION HAS DIFFERENT PARAMETERS";
                throw_error(newsym,previous,error);
            }
        } else {
            string error = "PREVIOUS DECLARATION OF NON-MATCHING TYPE";
            throw_error(newsym,previous,error);
        }
    } else {
        if (is_function) {
            symtable->emplace(key,newsym);
            activate_function(root->children[2],params);
        } else {
            symtable->emplace(key,newsym);
        }
    }

    return update_node(root,dectype);
}



attr_bitset switch_tok_temp( astree* root ) {
    printf("UNIMPLEMENTED %d\n",root->symbol);
    return -1;
}



attr_bitset switch_tok_vardecl( astree* root ) {
    astree* lval = root->children[0];
    attr_bitset ltype = get_declid_type( lval, attr_bitset(0x0088) );
    const string* key = lval->lexinfo;

    symbol* new_node = generate_symbol(lval,ltype,nullptr,nullptr);

    symbol_table* symtable = get_current_symtable();
    if (symtable->find(key) != symtable->end()) {
        string error = "SYMBOL ALREADY DECLARED IN SAME BLOCK";
        throw_error(new_node,symtable->at(key),error);
    } else {
        symtable->emplace(lval->lexinfo,new_node);
    }

    return update_binary(root,ltype);
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
    printf("UNIMPLEMENTED %d: ERROR ENCOUNTERED: SYMBOL NOT FOUND!!!\n", root->symbol);
    return -1;
}
