#ifndef __SWITCH_FUNCTIONS_H__
#define __SWITCH_FUNCTIONS_H__
attr_bitset invoke_switchboard(astree* root);

attr_bitset update_node( astree* root, attr_bitset new_bits );
attr_bitset update_node( astree* root, unsigned long val );
attr_bitset update_binary( astree* root );
attr_bitset equivalent_op( astree* root );
attr_bitset inequivalent_op( astree* root );
attr_bitset int_op( astree* root );
attr_bitset unary_op( astree* root );
attr_bitset branch_op( astree* root );

attr_bitset switch_assignment( astree* root );
attr_bitset switch_tok_void( astree* root );
attr_bitset switch_tok_bool( astree* root );
attr_bitset switch_tok_char( astree* root );
attr_bitset switch_tok_int( astree* root );
attr_bitset switch_tok_string( astree* root );
attr_bitset switch_tok_if( astree* root );
attr_bitset switch_tok_else( astree* root );
attr_bitset switch_tok_while( astree* root );
attr_bitset switch_tok_return( astree* root );
attr_bitset switch_tok_struct( astree* root );
attr_bitset switch_tok_false( astree* root );
attr_bitset switch_tok_true( astree* root );
attr_bitset switch_tok_null( astree* root );
attr_bitset switch_tok_new( astree* root );
attr_bitset switch_tok_array( astree* root );
attr_bitset switch_tok_ident( astree* root );
attr_bitset switch_tok_intcon( astree* root );
attr_bitset switch_tok_charcon( astree* root );
attr_bitset switch_tok_stringcon( astree* root );
attr_bitset switch_tok_block( astree* root );
attr_bitset switch_tok_call( astree* root );
attr_bitset switch_tok_ifelse( astree* root );
attr_bitset switch_tok_declid( astree* root );
attr_bitset switch_tok_paramlist( astree* root );
attr_bitset switch_tok_newarray( astree* root );
attr_bitset array_access( astree* root );
attr_bitset selector_access( astree* root );
attr_bitset switch_tok_typeid( astree* root );
attr_bitset switch_tok_field( astree* root );
attr_bitset switch_tok_ord( astree* root );
attr_bitset switch_tok_chr( astree* root );
attr_bitset switch_tok_root( astree* root );
attr_bitset switch_tok_function( astree* root );
attr_bitset switch_tok_prototype( astree* root );
attr_bitset switch_tok_temp( astree* root );
attr_bitset switch_tok_vardecl( astree* root );
attr_bitset switch_tok_returnvoid( astree* root );
attr_bitset switch_tok_newstring( astree* root );
attr_bitset switch_tok_error( astree* root );
attr_bitset switch_error( astree* root );
#endif
