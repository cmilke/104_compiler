#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
using namespace std;

#include "astree.h"
#include "oilgen.h"
#include "yyparse.h"
#include "typechecking.h"
#include "symtable.h"

#include "oil_switch_functions.h"



string _oil_text = "";
const string _indent = "        ";
int _s_const_reg = 0; //string constant regist
int _c_reg = 0; //char register
int _i_reg = 0; //int register
int _p_reg = 0; //direct data register
int _a_reg = 0; //indirect address pointer register



global_container::global_container() :
    structures   ( new vector<astree*> ),
    stringconsts ( new vector<astree*> ),
    variables    ( new vector<astree*> ),
    functions    ( new vector<astree*> ) {}




const string get_declaration_type( astree* root ) {
    string type_name = "";

    attr_bitset bits = root->attributes;
    if      ( bits.test(16) ) type_name += "void";
    else if ( bits.test(15) ) type_name += "char";
    else if ( bits.test(14) ) type_name += "char";
    else if ( bits.test(13) ) type_name += "int";
    else if ( bits.test(11) ) type_name += "char*";
    else { 
        type_name += "struct s_";
        type_name += *(root->children[0]->lexinfo);
        type_name += "*";
    }

    if ( root->children.size() > 1 ) {
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
            sprintf( buffer, "%s%s f_%s_%s;\n",
                    _indent.c_str(),field_type,struct_name,field_name );

            struct_text += string(buffer);
        }
    }
    struct_text += string("};\n\n");
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



void gen_functions( astree* fun_root ) {
    astree* ident  = fun_root->children[0];
    astree* params = fun_root->children[1];
    astree* block  = fun_root->children[2];

    const string ident_type = get_declaration_type(ident);
    const string ident_name = "__" + *(ident->lexinfo);

    _oil_text += ident_type + " " + ident_name + "(";
    
    int last_param = params->children.size();
    if ( last_param == 0 ) {
        _oil_text += ")";
    } else {
        _oil_text += "\n";
        for ( int paramI = 0; paramI < last_param; paramI++ ) {
            astree* param = params->children[paramI];

            const string param_type = get_declaration_type(param);
            const string param_name = oil_switch_ident(param);

            _oil_text += _indent + param_type + " " + param_name;
            if ( paramI == last_param-1 ) _oil_text += ")";
            else _oil_text += ",\n";
        }
    }

    if ( fun_root->symbol == TOK_PROTOTYPE ) {
        _oil_text += ";\n";

    } else {
        _oil_text += "\n{\n";
        oil_invoke_switchboard(block);
        _oil_text += "}\n";
    }
}



void generate_oil_file(astree* root, global_container* globals, FILE* oil_f) {

    //structures
    for ( astree* struct_root : *(globals->structures) ) {
        gen_structure(struct_root);
    }
    _oil_text += "\n";

    //string constants
    for ( astree* string_root : *(globals->stringconsts) ) {
        gen_stringconst(string_root);
    }
    _s_const_reg = 0;
    _oil_text += "\n";

    //global variables
    for ( astree* struct_var : *(globals->variables) ) {
        gen_variables(struct_var);
    }
    _oil_text += "\n";

    //functions
    for ( astree* struct_fun : *(globals->functions) ) {
        gen_functions(struct_fun);
    }
    _oil_text += "\n";

    //main function
    _oil_text += "void __ocmain (void)\n{\n";
    for ( astree* child : root->children ) {
        _oil_text += oil_invoke_switchboard(child);
    }
    _oil_text += "}\n";

    fprintf( oil_f, _oil_text.c_str() );
    fclose(oil_f);
}



const string oil_invoke_switchboard(astree* root) {
    switch(root->symbol) {
        case '+':
        case '-':
        case '*':
        case '/':
        case '%':
        case TOK_EQ:
        case TOK_NE:         
        case TOK_LT:
        case TOK_LE:
        case TOK_GT:
        case TOK_GE:            return oil_switch_binary(root); break;
        case TOK_VARDECL:
        case '=':               return oil_switch_assignment(root); break;
        case TOK_IDENT:         return oil_switch_ident(root); break;
        case TOK_FIELD:         return oil_switch_field(root); break;
        case TOK_DECLID:        return oil_switch_declid(root); break;
        case TOK_NULL:
        case TOK_FALSE:         return "0"; break;
        case TOK_TRUE:          return "1"; break;
        case TOK_VOID:          return "void"; break;
        case TOK_INTCON:
        case TOK_CHARCON:       return *(root->lexinfo); break;
        case TOK_STRINGCON:     return oil_switch_stringcon(); break;
        case '[':               return oil_switch_array(root); break;
        case '.':               return oil_switch_selector(root); break;
        case TOK_WHILE:         return oil_switch_while(root); break;
        case TOK_IF:            return oil_switch_if(root); break;
        case TOK_IFELSE:        return oil_switch_ifelse(root); break;
        case TOK_NEW:           return oil_switch_new(root); break;
        case TOK_NEWSTRING:
        case TOK_NEWARRAY:      return oil_switch_newarray(root); break;
        case TOK_BLOCK:         return oil_switch_block(root); break;
        case TOK_CALL:          return oil_switch_call(root); break;
        case '!':
        case TOK_NEG:
        case TOK_POS:           return oil_switch_unary(root); break;
        case TOK_ORD:           return oil_switch_ord(root); break;
        case TOK_CHR:           return oil_switch_chr(root); break;
        case TOK_STRUCT:
        case TOK_PROTOTYPE:
        case TOK_FUNCTION:      return ""; break;
        case TOK_RETURN:        return oil_switch_return(root); break;
        case TOK_RETURNVOID:    return oil_switch_returnvoid(); break;
        default:                return oil_compile_error(root); break;
    }
}



const string get_decl( astree* root ) {
    string type_name = "";

    attr_bitset bits = root->attributes;
    if      ( bits.test(16) ) type_name += "void";
    else if ( bits.test(15) ) type_name += "char";
    else if ( bits.test(14) ) type_name += "char";
    else if ( bits.test(13) ) type_name += "int";
    else if ( bits.test(11) ) type_name += "char*";
    else if ( bits.test(6)  ) {
        type_name += "struct s_";
        type_name += *(root->children[1]->declid->identifier->key);
        type_name += "*";

    } else { 
        type_name += "struct s_";
        type_name += *(root->children[0]->lexinfo);
        type_name += "*";
    }

    return type_name;
}



const string get_reg(astree* root) {
    attr_bitset reg_bits = root->attributes;

    string reg = "";
    if      ( reg_bits.test(15) ) reg += "c" + to_string(_c_reg++);
    else if ( reg_bits.test(14) ) reg += "c" + to_string(_c_reg++);
    else if ( reg_bits.test(13) ) reg += "i" + to_string(_i_reg++);
    //else if ( reg_bits.test(5) )   type_name += "?" + _reg_x++;
    else {  reg += "p" + to_string(_p_reg++);
        //type_name += "struct s_";
        //type_name += *(declaration->children[0]->lexinfo);
        //type_name += "*";
    }

    //if ( declaration->children.size() > 1 ) {
    //type_name += "*";
    //}

    return reg;
}



const string oil_switch_binary( astree* root ) {

    astree* lval = root->children[0];
    string lstring = oil_invoke_switchboard(lval);

    astree* rval = root->children[1];
    string rstring = oil_invoke_switchboard(rval);

    string reg = get_reg(root);
    string decl = get_decl(root);
    string op = *(root->lexinfo);

    _oil_text += _indent + decl + " " + reg + " = "; 
    _oil_text += lstring + " " + op + " " + rstring + ";\n";

    return reg;
}



const string oil_switch_assignment(astree* root) {
    astree* lval = root->children[0];
    astree* rval = root->children[1];

    string rstring = oil_invoke_switchboard(rval);
    string lstring = oil_invoke_switchboard(lval);

    _oil_text += _indent + lstring + " = " + rstring + ";\n";

    return "";
}



const string oil_switch_ident( astree* root ) {
    string base = *(root->lexinfo);

    size_t blk = root->block_nr;
    if ( root->declid != nullptr ) { 
        blk = root->declid->block_nr;
    }

    if (blk == 0) return ("__" + base);
    else return ( "_" + to_string(blk) + "_" + base);
}



const string oil_switch_declid( astree* root ) {
    string declid = "";
    if (root->block_nr != 0) declid += get_declaration_type(root) + " ";
    declid += oil_switch_ident(root);

    return declid;
}



const string oil_switch_field( astree* root ) {
    const string base = *(root->lexinfo);
    const string sname = *(root->declid->parent_structure->key);


    return ("f_" + sname + "_" + base);
}



const string oil_switch_stringcon() {
    return "__s" + to_string(_s_const_reg++);
}



const string oil_switch_array( astree* root ) {
    astree* array = root->children[0];
    astree* index = root->children[1];

    string a_string = oil_invoke_switchboard(array);
    string i_string = oil_invoke_switchboard(index);

    string reg = get_reg(root);
    string decl = get_decl(root);

    _oil_text += _indent + decl + " " + reg + " = ";
    _oil_text += a_string + "[" + i_string + "];\n";

    return reg;
}



const string oil_switch_selector( astree* root ) {
    astree* instance = root->children[0];
    astree* field = root->children[1];

    const string istring = oil_invoke_switchboard(instance);
    const string fstring = oil_invoke_switchboard(field);

    string decl = get_decl(root);
    string reg = get_reg(root);

    _oil_text += _indent + decl + "* " + reg + " = ";
    _oil_text += "&(" + istring + "->" + fstring + ");\n";

    return ("(*" + reg + ")");
}



const string oil_switch_while( astree* root ) {
    astree* expr = root->children[0];
    astree* block = root->children[1];

    string sym = "_" + to_string(root->filenr);
    sym += "_" + to_string(root->linenr);
    sym += "_" + to_string(root->offset);

    _oil_text += "while" + sym + ":;\n";

    const string estring = oil_invoke_switchboard(expr);
    _oil_text += _indent + "if (!" + estring + ") ";
    _oil_text += "goto break" + sym + ";\n";

    oil_invoke_switchboard(block);

    _oil_text += _indent + "goto while" + sym + ";\n";
    _oil_text += "break" + sym + ":;\n";
    return "";
}



const string oil_switch_if( astree* root ) {
    astree* expr = root->children[0];
    astree* block = root->children[1];

    const string estring = oil_invoke_switchboard(expr);
    string sym = "_" + to_string(root->filenr);
    sym += "_" + to_string(root->linenr);
    sym += "_" + to_string(root->offset);

    _oil_text += _indent + "if (!" + estring + ") ";
    _oil_text += "goto fi" + sym + ";\n"; 

    oil_invoke_switchboard(block);

    _oil_text += "fi" + sym + ":;\n";
    return "";
}



const string oil_switch_ifelse( astree* root ) {
    astree* expr = root->children[0];
    astree* ifblock = root->children[1];
    astree* elseblock = root->children[2];

    const string estring = oil_invoke_switchboard(expr);
    string sym = "_" + to_string(root->filenr);
    sym += "_" + to_string(root->linenr);
    sym += "_" + to_string(root->offset);

    _oil_text += _indent + "if (!" + estring + ") ";
    _oil_text += "goto else" + sym + ";\n"; 

    oil_invoke_switchboard(ifblock);
    _oil_text += _indent + "goto fi" + sym + ";\n";
    _oil_text += "else" + sym + ":;\n";
    oil_invoke_switchboard(elseblock);
    _oil_text += "fi" + sym + ":;\n";
    return "";
}



const string oil_switch_new( astree* root ) {
    astree* ident = root->children[0];
    string struct_name = *(ident->lexinfo);
    string reg = get_reg(ident);

    _oil_text += _indent + "struct s_" + struct_name + "* " + reg + " = ";
    _oil_text += "xcalloc (1, sizeof (struct s_" + struct_name + "));\n";
    return reg;
}



const string oil_switch_newarray( astree* root ) {
    astree* index  = root->children[1];
    string istring = oil_invoke_switchboard(index);

    astree* base = root->children[0];
    string reg = get_reg(base);
    string decl = get_decl(base);

    _oil_text += _indent + decl + "* " + reg + " = ";
    _oil_text += "xcalloc (" + istring + ", sizeof (" + decl + "));\n";
    return reg;
}



const string oil_switch_block( astree* root ) {
    for ( astree* child : root->children ) {
        oil_invoke_switchboard(child);
    }
    return "";
}



const string oil_switch_call( astree* root ) {

    const string ident = "__" + *(root->children[0]->lexinfo);
    string args = "(";
    
    int numkids = root->children.size();
    if (numkids > 1) {
        for ( int childI = 1; childI < numkids; childI++ ) {
            astree* child = root->children[childI];
            string arg = oil_invoke_switchboard(child);

            args += arg; 
            if ( childI < numkids-1 ) args += ",";
            else args += ")";
        }
    } else {
        args += ")";
    }

    if ( root->attributes.test(16) ) {
        _oil_text += _indent + ident + args + ";\n";
        return "";

    } else {
        string reg = get_reg(root);
        string decl = get_decl(root);

        _oil_text += _indent + decl + " " + reg + " = ";
        _oil_text += ident + args + ";\n";
        
        return reg; 
    }
}



const string generic_unary( astree* root, string op ) {
    astree* rval = root->children[0];
    string rstring = oil_invoke_switchboard(rval);

    string reg = get_reg(root);
    string decl = get_decl(root);

    _oil_text += _indent + decl + " " + reg + " = "; 
    _oil_text +=  op + " " + rstring + ";\n";

    return reg;
}



const string oil_switch_unary( astree* root ) {
    string op = *(root->lexinfo);
    return generic_unary(root,op);
}



const string oil_switch_ord( astree* root ) {
    return generic_unary(root,"(int)");
}



const string oil_switch_chr( astree* root ) {
    return generic_unary(root,"(char)");
}



const string oil_switch_return( astree* root ) {
    astree* child = root->children[0];
    string retval = oil_invoke_switchboard(child);
    _oil_text += _indent + "return " + retval + ";\n";
    return "";
}



const string oil_switch_returnvoid() {
    _oil_text += _indent + "return;\n";
    return "";
}



const string oil_compile_error( astree* root ) {
    return "\nERROR! " + *(root->lexinfo) + "\n";
}
