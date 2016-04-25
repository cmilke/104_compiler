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
    int yflag = 0;

    int arg = -1;
    while ( (arg = getopt(argc, argv, "@:D:ly")) != -1 ){
        switch(arg) {
            case '@': set_debugflags(optarg); break;
            case 'D': cpp_opts = optarg; break;
            case 'l': yy_flex_debug = 1; break;
            case 'y': yflag = 1; break;
        }
    }
    __debugprintf('b', "oc.cpp", -1, "deal_with_arguments",
            "l=%d, y=%d\n",yy_flex_debug,yflag);

    for(int argi = optind; argi < argc; argi++) {
        files.push_back( string(argv[argi]) );
    }
}



// Run cpp against the lines of the file.
void gettokens (string filename, astree* parseroot) {
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
}


void make_str_file(string filename) {
    int delimit_i = filename.rfind(".");
    int delimit_j = filename.rfind("/");
    string output_orig = filename.substr(0,delimit_i) + ".str";
    string output_name = string(output_orig).substr(delimit_j+1);

    FILE *output_file;
    output_file = fopen(output_name.c_str(), "w");
    dump_stringset(output_file);
    fclose(output_file);
}


void make_tok_file(string filename, astree* parseroot) {
    int delimit_i = filename.rfind(".");
    int delimit_j = filename.rfind("/");
    string output_orig = filename.substr(0,delimit_i) + ".tok";
    string output_name = string(output_orig).substr(delimit_j+1);

    FILE *output_file;
    output_file = fopen(output_name.c_str(), "w");
    for (auto tree : parseroot->children) {
        fprintf (output_file, "%2ld  %02ld.%03ld  %3d  %-15s  (%s)\n",
                tree->filenr, tree->linenr, tree->offset, tree->symbol,
                get_yytname(tree->symbol), tree->lexinfo->c_str());
    }
    fclose(output_file);
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
            astree* parseroot = new_parseroot();
            gettokens(filename.c_str(), parseroot);

            int pclose_rc = pclose (yyin);
            eprint_status (command.c_str(), pclose_rc);
            if (pclose_rc != 0) set_exitstatus (EXIT_FAILURE);

            make_tok_file(filename, parseroot);
            make_str_file(filename);
        }

    }
    return get_exitstatus();
}

