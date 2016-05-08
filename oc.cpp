// $Id: cppstrtok.cpp,v 1.4 2016-03-24 14:34:10-07 - - $
// Christopher Milke

// Use cpp to scan a file and print line numbers.
// Print out each input line read in, then strtok it for
// tokens.

#include <string>
#include <iostream>
#include <vector>
using namespace std;

#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

#include "auxlib.h"
#include "yylex.h"
#include "lyutils.h"
#include "yyparse.h"
#include "stringset.h"



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



// Run cpp against the lines of the file.
/*void gettokens (string filename, astree* parseroot) {
    __debugprintf('b', "oc.cpp", -1, "gettokens",
            "current file = %s\n",filename.c_str());

    for (;;) {
        int symbol = yylex();
        if (symbol == YYEOF) break;

        lexer_useraction();
        astree* newtree = get_newtree(symbol);
        intern_stringset(yytext);
        adopt1(parseroot, newtree);
    }
}*/

void depth_first_print(FILE* tok_f, FILE* ast_f, int depth, astree* root) {
    fprintf (tok_f, "%2ld  %02ld.%03ld  %3d  %-15s  (%s)\n",
            root->filenr, root->linenr, root->offset, root->symbol,
            get_yytname(root->symbol), root->lexinfo->c_str());


    for(int indentI = 0; indentI < depth; indentI++) fprintf(ast_f, "|    ");
    fprintf(ast_f, "%s  \"%s\"  %ld.%ld.%ld\n",
            get_yytname(root->symbol), root->lexinfo->c_str(),
            root->filenr, root->linenr, root->offset);

    for(auto tree : root->children) {
        depth_first_print(tok_f, ast_f, depth+1, tree);
    }
}

void make_output_files(string filename) {
    int delimit_i = filename.rfind(".");
    int delimit_j = filename.rfind("/");
    string output_base = filename.substr(0,delimit_i).substr(delimit_j+1);
    string output_name_str = output_base + ".str";
    string output_name_tok = output_base + ".tok";
    string output_name_ast = output_base + ".ast";


    FILE *str_f = fopen(output_name_str.c_str(), "w");
    dump_stringset(str_f);
    fclose(str_f);

    FILE *tok_f = fopen(output_name_tok.c_str(), "w");
    FILE *ast_f = fopen(output_name_ast.c_str(), "w");
    depth_first_print(tok_f, ast_f, 0, yyparse_astree);
    fclose(tok_f);
    fclose(ast_f);
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
        }else {
            yyparse();

            int pclose_rc = pclose (yyin);
            eprint_status (command.c_str(), pclose_rc);
            if (pclose_rc != 0) set_exitstatus (EXIT_FAILURE);

            make_output_files(filename);
        }

    }
    return get_exitstatus();
}

