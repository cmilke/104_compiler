#ifndef __OIL_SWITCH_FUNCTIONS_H__
#define __OIL_SWITCH_FUNCTIONS_H__

const string oil_invoke_switchboard(astree* root);

const string oil_switch_binary(astree* root);
const string oil_switch_assignment(astree* root); 
const string oil_switch_stringcon(astree* root);
const string oil_switch_array(astree* root);
const string oil_switch_selector(astree* root);
const string oil_switch_while(astree* root);
const string oil_switch_if(astree* root);
const string oil_switch_ifelse(astree* root);
const string oil_switch_new(astree* root);
const string oil_switch_block(astree* root);
const string oil_switch_call(astree* root);
const string oil_switch_unary(astree* root);
const string oil_switch_newarray(astree* root);
const string oil_switch_ord(astree* root);
const string oil_switch_chr(astree* root);
const string oil_switch_vardecl(astree* root);
const string oil_switch_ident( astree* root );
const string oil_switch_declid( astree* root );
const string oil_switch_return(astree* root);
const string oil_switch_returnvoid(astree* root);
const string oil_switch_newstring(astree* root);
const string oil_compile_error(astree* root);

#endif
