// $Id: cppstrtok.cpp,v 1.4 2016-03-24 14:34:10-07 - - $
// Christopher Milke

// Use cpp to scan a file and print line numbers.
// Print out each input line read in, then strtok it for
// tokens.

#include <string>
#include <iostream>
#include <vector>
#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
using namespace std;

#include "astree.h"
#include "auxlib.h"
#include "yylex.h"
#include "lyutils.h"
#include "yyparse.h"
#include "stringset.h"
#include "symtable.h"
#include "oilgen.h"
#include "typechecking.h"



const string CPP = "/usr/bin/cpp";
constexpr size_t LINESIZE = 1024;



void deal_with_arguments (int argc, char** argv, vector<string> &files, string &cpp_opts) {
    yy_flex_debug = 0;
    yydebug = 0;

    int arg = -1;
    while ( (arg = getopt(argc, argv, "@:D:ly")) != -1 ){
        switch(arg) {
            case '@': set_debugflags(optarg); break;
            case 'D': cpp_opts = optarg; break;
            case 'l': yy_flex_debug = 1; break;
            case 'y': yydebug = 1; break;
        }
    }
    __debugprintf('b', "oc.cpp", -1, "deal_with_arguments",
            "l=%d, y=%d\n",yy_flex_debug,yydebug);

    for(int argi = optind; argi < argc; argi++) {
        files.push_back( string(argv[argi]) );
    }
}



void depth_first_print(FILE* tok_f, FILE* ast_f, FILE* sym_f, int depth, astree* root) {
    string attrs[] = { "void", "bool", "char", "int", "null",
                       "string",   "struct", "array",  "function",
                       "variable", "field",  "typeid", "param",
                       "lval",     "const",  "vreg",   "vaddr" };



    fprintf (tok_f, "%2ld  %02ld.%03ld  %3d  %-15s  (%s)\n",
            root->filenr, root->linenr, root->offset, root->symbol,
            get_yytname(root->symbol), root->lexinfo->c_str());


    for(int indentI = 0; indentI < depth; indentI++) fprintf(ast_f, "|    ");
    fprintf(ast_f, "%s  \"%s\"  %ld.%ld.%ld {%ld}",
            get_yytname(root->symbol), root->lexinfo->c_str(),
            root->filenr, root->linenr, root->offset,
            root->block_nr);

    for ( size_t i = 0; i < root->attributes.size(); i++ ) {
        if (root -> attributes[i]) {
            fprintf( ast_f, " %s", attrs[16-i].c_str() ); //FIXME:once you flip the bits around, remove the 16
        }
    } fprintf( ast_f, "\n");


    if ( root->is_symbol) {
        for(int indentI = 0; indentI < depth; indentI++) fprintf(sym_f, "   ");
        fprintf(sym_f, "%s (%ld.%ld.%ld) {%ld}",
                root->lexinfo->c_str(),
                root->filenr, root->linenr, root->offset,
                root->block_nr);

        for ( size_t i = 0; i < root->attributes.size(); i++ ) {
            if (root -> attributes[i]) {
                fprintf( sym_f, " %s", attrs[16-i].c_str() ); //FIXME:once you flip the bits around, remove the 16
            }
        } fprintf( sym_f, "\n");
    }

    for(auto tree : root->children) {
        depth_first_print(tok_f, ast_f, sym_f, depth+1, tree);
    }
}



FILE* make_output_files(string filename) {
    int delimit_i = filename.rfind(".");
    int delimit_j = filename.rfind("/");
    string output_base = filename.substr(0,delimit_i).substr(delimit_j+1);
    string output_name_str = output_base + ".str";
    string output_name_tok = output_base + ".tok";
    string output_name_ast = output_base + ".ast";
    string output_name_sym = output_base + ".sym";
    string output_name_oil = output_base + ".oil";


    FILE* str_f = fopen(output_name_str.c_str(), "w");
    dump_stringset(str_f);
    fclose(str_f);

    FILE* tok_f = fopen(output_name_tok.c_str(), "w");
    FILE* ast_f = fopen(output_name_ast.c_str(), "w");
    FILE* sym_f = fopen(output_name_sym.c_str(), "w");
    depth_first_print(tok_f, ast_f, sym_f, 0, yyparse_astree);
    fclose(tok_f);
    fclose(ast_f);

    FILE* oil_f = fopen(output_name_oil.c_str(), "w");
    return oil_f;
}



int main (int argc, char** argv) {
    set_execname (argv[0]);
    vector<string> files;
    string cpp_opts;
    deal_with_arguments(argc, argv, files, cpp_opts);

    string base_command = CPP + " "; 
    if( !cpp_opts.empty() ) base_command += "-D" + cpp_opts + " ";

    for (auto filename : files) {
        string command = base_command + filename;

        yyin = popen (command.c_str(), "r");
        if (yyin == NULL) {
            syserrprintf (command.c_str());
        } else {
            if ( yyparse() ) exit(1);

            int pclose_rc = pclose (yyin);
            eprint_status (command.c_str(), pclose_rc);
            if (pclose_rc != 0) exit(1);

            global_container* globals = create_symbol_table(yyparse_astree);
            if (ERROR_THROWN) {
                fprintf(stderr, "\nCOMPILATION FAILED\n");
                exit(1);
            }

            FILE* oil_f = make_output_files(filename);

            generate_oil_file(yyparse_astree,globals,oil_f);
        }

    }
    return get_exitstatus();
}

