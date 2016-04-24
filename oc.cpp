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



const string CPP = "/usr/bin/cpp";
constexpr size_t LINESIZE = 1024;



void deal_with_arguments (int argc, char** argv, vector<string> &files, string &cpp_opts) {
    int lflag = 0;
    int yflag = 0;

    int arg = -1;
    while ( (arg = getopt(argc, argv, "@:D:ly")) != -1 ){
        switch(arg) {
            case '@': set_debugflags(optarg); break;
            case 'D': cpp_opts = optarg; break;
            case 'l': lflag = 1; break;
            case 'y': yflag = 1; break;
        }
    }
    __debugprintf('b', "oc.cpp", 87, "deal_with_arguments",
            "l=%d, y=%d\n",lflag,yflag);

    for(int argi = optind; argi < argc; argi++) {
        files.push_back( string(argv[argi]) );
    }
}



// Run cpp against the lines of the file.
void cpplines (string filename) {
    printf("\n\n");
    for (;;) {
        int token = yylex();
        if (token == YYEOF) break;

        __debugprintf('a', "oc.cpp", 50, "cpplines", 
                "%s: [%s]\n", filename.c_str(), token);

        printf("%d ", token);
    }
    printf("\n\n");
}



int main (int argc, char** argv) {
    set_execname (argv[0]);
    vector<string> files;
    string cpp_opts;
    deal_with_arguments(argc, argv, files, cpp_opts);

    for (auto filename : files) {
        string command;
        if ( cpp_opts.empty() ) command = CPP + " " + filename;
        else command = CPP + " " + "-D" + cpp_opts + " " + filename;
        printf ("command=\"%s\"\n", command.c_str());
        yyin = popen (command.c_str(), "r");
        if (yyin == NULL) {
            syserrprintf (command.c_str());
        }else {
            cpplines (filename.c_str());
            int pclose_rc = pclose (yyin);
            eprint_status (command.c_str(), pclose_rc);
            if (pclose_rc != 0) set_exitstatus (EXIT_FAILURE);
        }

        /*int delimit_i = filename.rfind(".");
        int delimit_j = filename.rfind("/");
        string output_orig = filename.substr(0,delimit_i) + ".str";
        string output_name = string(output_orig).substr(delimit_j+1);
        FILE *output_file;
        output_file = fopen(output_name.c_str(), "w");
        dump_stringset(output_file);
        fclose(output_file);*/
    }
    return get_exitstatus();
}

