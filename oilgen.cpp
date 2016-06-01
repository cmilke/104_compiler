#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
using namespace std;

#include "astree.h"
#include "oilgen.h"
#include "yyparse.h"

#include "oil_switch_functions.h"



string _oil_text = "";
int _s_const_reg = 0;

global_container::global_container() :
    structures   ( new vector<astree*> ),
    stringconsts ( new vector<astree*> ),
    variables    ( new vector<astree*> ),
    functions    ( new vector<astree*> ) {}




const string get_declaration_type( astree* declaration ) {
    string type_name = "";

    string base = *(declaration->children[0]->lexinfo);
    if      (base == "bool")   type_name += "char";
    else if (base == "char")   type_name += "char";
    else if (base == "int")    type_name += "int";
    else if (base == "string") type_name += "char*";
    else { 
        type_name += "struct s_";
        type_name += *(declaration->children[0]->lexinfo);
        type_name += "*";
    }

    if ( declaration->children.size() > 1 ) {
        type_name += "*";
    }

    return type_name;
}



void gen_structure( astree* struct_root ) {
    string struct_text = "";
    char buffer[1000];

    bool is_ident = true;
    const char* struct_name = struct_root->children[0]->lexinfo->c_str();
    for (astree* child : struct_root->children ) {
        if (is_ident) {
            sprintf( buffer, "struct s_%s {\n", struct_name );
            struct_text += string(buffer);
            is_ident = false;

        } else {
            const char* field_type = get_declaration_type(child).c_str();
            const char* field_name = child->lexinfo->c_str();
            sprintf( buffer, "\t%s f_%s_%s;\n",
                    field_type,struct_name,field_name );

            struct_text += string(buffer);
        }
    }
    struct_text += string("}\n\n");
    _oil_text += struct_text;
}



void gen_stringconst( astree* string_root ) {
    const char* string_val = string_root->lexinfo->c_str();
    char buffer[1000];
    sprintf( buffer, "char* __s%d = %s;\n", 
                _s_const_reg, string_val);

    _oil_text += string(buffer);

    _s_const_reg++;
}



void gen_variables( astree* var_root ) {
    char buffer[1000];
    const char* var_type = get_declaration_type(var_root).c_str();
    const char* var_name = var_root->lexinfo->c_str();
    sprintf( buffer, "%s __%s;\n",
            var_type,var_name );

    _oil_text += string(buffer);

}



void generate_oil_file(astree* root, global_container* globals) {

    _oil_text += "//BEGIN STRUCT DECLARATIONS\n";
    for ( astree* struct_root : *(globals->structures) ) {
        gen_structure(struct_root);
    }
    _oil_text += "//END STRUCT DECLARATIONS\n\n\n";

    _oil_text += "//BEGIN STRING CONSTANT DECLARATIONS\n";
    for ( astree* string_root : *(globals->stringconsts) ) {
        gen_stringconst(string_root);
    }
    _s_const_reg = 0;
    _oil_text += "//END STRING CONSTANT DECLARATIONS\n\n\n";

    _oil_text += "//BEGIN GLOBAL VARIABLE DECLARATIONS\n";
    for ( astree* struct_var : *(globals->variables) ) {
        gen_variables(struct_var);
    }
    _oil_text += "//END GLOBAL VARIABLE DECLARATIONS\n\n\n";

    //skipping functions for now...

    _oil_text += "//BEGIN MAIN PROGRAM\n";
    _oil_text += "void __ocmain (void)\n{";
    for ( astree* child :  root->children ) {
        _oil_text += *(child->lexinfo);
    }
    _oil_text += "}\n";
    _oil_text += "//END MAIN PROGRAM\n\n";

    

    cout << _oil_text;
}



const string oil_invoke_switchboard(astree* root) {
    switch(root->symbol) {
        case '+':
        case '-':
        case '*':
        case '/':
        case '%':
        case '=':
        case TOK_EQ:
        case TOK_NE:         
        case TOK_LT:
        case TOK_LE:
        case TOK_GT:
        case TOK_GE:         return oil_switch_binary(root); break;
        case TOK_FALSE:      return "0"; break;
        case TOK_TRUE:       return "1"; break;
        case TOK_NULL:       return "null"; break;
        case TOK_INTCON:
        case TOK_CHARCON:    return *(root->lexinfo); break;
        case TOK_STRINGCON:  return oil_switch_stringcon(root); break;
        case '[':            return oil_switch_array(root); break;
        case '.':            return oil_switch_selector(root); break;
        case TOK_WHILE:      return oil_switch_while(root); break;
        case TOK_IF:         return oil_switch_if(root); break;
        case TOK_IFELSE:     return oil_switch_ifelse(root); break;
        case TOK_NEW:        return oil_switch_new(root); break;
        case TOK_BLOCK:      return oil_switch_block(root); break;
        case TOK_CALL:       return oil_switch_call(root); break;
        case TOK_NEG:
        case TOK_POS:        return oil_switch_unary(root); break;
        case TOK_NEWARRAY:   return oil_switch_newarray(root); break;
        case TOK_ORD:        return oil_switch_ord(root); break;
        case TOK_CHR:        return oil_switch_chr(root); break;
        case TOK_STRUCT:
        case TOK_PROTOTYPE:
        case TOK_FUNCTION:   return ""; break;
        case TOK_VARDECL:    return oil_switch_vardecl(root); break;
        case TOK_RETURN:     return oil_switch_return(root); break;
        case TOK_RETURNVOID: return oil_switch_returnvoid(root); break;
        case TOK_NEWSTRING:  return oil_switch_newstring(root); break;
        default:             return oil_compile_error(root); break;
    }
}



const string oil_switch_binary( astree* root ) {
    return "UNIMPLEMENTED: " + *(root->lexinfo);
}



const string oil_switch_stringcon( astree* root ) {
    return "UNIMPLEMENTED: " + *(root->lexinfo);
}



const string oil_switch_array( astree* root ) {
    return "UNIMPLEMENTED: " + *(root->lexinfo);
}



const string oil_switch_selector( astree* root ) {
    return "UNIMPLEMENTED: " + *(root->lexinfo);
}



const string oil_switch_while( astree* root ) {
    return "UNIMPLEMENTED: " + *(root->lexinfo);
}



const string oil_switch_if( astree* root ) {
    return "UNIMPLEMENTED: " + *(root->lexinfo);
}



const string oil_switch_ifelse( astree* root ) {
    return "UNIMPLEMENTED: " + *(root->lexinfo);
}



const string oil_switch_new( astree* root ) {
    return "UNIMPLEMENTED: " + *(root->lexinfo);
}



const string oil_switch_block( astree* root ) {
    return "UNIMPLEMENTED: " + *(root->lexinfo);
}



const string oil_switch_call( astree* root ) {
    return "UNIMPLEMENTED: " + *(root->lexinfo);
}



const string oil_switch_unary( astree* root ) {
    return "UNIMPLEMENTED: " + *(root->lexinfo);
}



const string oil_switch_newarray( astree* root ) {
    return "UNIMPLEMENTED: " + *(root->lexinfo);
}



const string oil_switch_ord( astree* root ) {
    return "UNIMPLEMENTED: " + *(root->lexinfo);
}



const string oil_switch_chr( astree* root ) {
    return "UNIMPLEMENTED: " + *(root->lexinfo);
}



const string oil_switch_vardecl( astree* root ) {
    return "UNIMPLEMENTED: " + *(root->lexinfo);
}



const string oil_switch_return( astree* root ) {
    return "UNIMPLEMENTED: " + *(root->lexinfo);
}



const string oil_switch_returnvoid( astree* root ) {
    return "UNIMPLEMENTED: " + *(root->lexinfo);
}



const string oil_switch_newstring( astree* root ) {
    return "UNIMPLEMENTED: " + *(root->lexinfo);
}



const string oil_compile_error( astree* root ) {
    return "UNIMPLEMENTED: " + *(root->lexinfo);
}



