#ifndef __SYM_SWITCH_FUNCTIONS_H__
#define __SYM_SWITCH_FUNCTIONS_H__

attr_bitset invoke_switchboard(astree* root);
symbol_table* get_current_symtable();
symbol* retrieve_symbol(astree* root);
attr_bitset update_node( astree* root, attr_bitset new_bits );
attr_bitset update_node( astree* root, unsigned long val );
attr_bitset get_declid_type( astree* declid, attr_bitset mask, symbol** sympoint );
attr_bitset get_declid_type( astree* declid, unsigned long mask,
            symbol** sympoint );
attr_bitset get_declid_type( astree* declid, attr_bitset mask );
attr_bitset get_declid_type( astree* declid, unsigned long mask );
symbol* generate_symbol( astree* tree, attr_bitset node_type,
            symbol_table* new_fields, vector<symbol*>* new_parameters,
            symbol* ident );
bool check_parameters( vector<symbol*>* first, vector<symbol*>* second);
vector<symbol*>* generate_params( astree* paramlist );
attr_bitset update_binary( astree* root, attr_bitset ltype );
attr_bitset update_binary( astree* root );
attr_bitset equivalent_op( astree* root );
attr_bitset inequivalent_op( astree* root );
attr_bitset int_op( astree* root );
attr_bitset unary_op( astree* root );
attr_bitset branch_op( astree* root );
void activate_function(astree* root, vector<symbol*>* params);
attr_bitset switch_assignment( astree* root );
attr_bitset switch_tok_return( astree* root );
attr_bitset switch_tok_returnvoid( astree* root );
attr_bitset switch_tok_struct( astree* root );
attr_bitset switch_tok_new( astree* root );
attr_bitset switch_tok_ident( astree* root );
attr_bitset switch_tok_stringcon( astree* root );
attr_bitset switch_tok_block( astree* root );
attr_bitset switch_tok_call( astree* root );
attr_bitset switch_tok_ifelse( astree* root );
attr_bitset switch_tok_newarray( astree* root );
attr_bitset array_access( astree* root );
symbol* selector_recursion( astree* root );
attr_bitset selector_access( astree* root );
attr_bitset switch_tok_typeid( astree* root );
attr_bitset switch_tok_ord( astree* root );
attr_bitset switch_tok_chr( astree* root );
attr_bitset switch_tok_function( astree* root );
attr_bitset switch_tok_vardecl( astree* root );
attr_bitset compile_error(astree* root);

#endif
