// $Id: cppstrtok.cpp,v 1.4 2016-03-24 14:34:10-07 - - $

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
#include "stringset.h"



const string CPP = "/usr/bin/cpp";
constexpr size_t LINESIZE = 1024;



// Chomp the last character from a buffer if it is delim.
void chomp (char* string, char delim) {
    size_t len = strlen (string);
    if (len == 0) return;
    char* nlpos = string + len - 1;
    if (*nlpos == delim) *nlpos = '\0';
}



// Run cpp against the lines of the file.
void cpplines (FILE* pipe, string filename) {
    int linenr = 1;
    //char inputname[LINESIZE];
    //strcpy (inputname, filename);
    for (;;) {
        char buffer[LINESIZE];
        char* fgets_rc = fgets (buffer, LINESIZE, pipe);
        if (fgets_rc == NULL) break;
        chomp (buffer, '\n');
        __debugprintf('a', "oc.cpp", 50, "cpplines", 
                "%s:line %d: [%s]\n", filename.c_str(), linenr, buffer);
        // http://gcc.gnu.org/onlinedocs/cpp/Preprocessor-Output.html
        char* header_name = NULL;
        int sscanf_rc = sscanf (buffer, "# %d \"%[^\"]\"", &linenr, header_name);
        if (sscanf_rc == 2) {
            //printf ("DIRECTIVE: line %d file \"%s\"\n", linenr, header_name);
            continue;
        }
        char* savepos = NULL;
        char* bufptr = buffer;
        for (int tokenct = 1;; ++tokenct) {
            char* token = strtok_r (bufptr, " \t\n", &savepos);
            bufptr = NULL;
            if (token == NULL) break;
            //printf ("token %d.%d: [%s]\n", linenr, tokenct, token);
            intern_stringset(token);
        }
        ++linenr;
    }
}



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
        FILE* pipe = popen (command.c_str(), "r");
        if (pipe == NULL) {
            syserrprintf (command.c_str());
        }else {
            cpplines (pipe, filename.c_str());
            int pclose_rc = pclose (pipe);
            eprint_status (command.c_str(), pclose_rc);
            if (pclose_rc != 0) set_exitstatus (EXIT_FAILURE);
        }

        int delimit_i = filename.rfind(".");
        int delimit_j = filename.rfind("/");
        string output_orig = filename.substr(0,delimit_i) + ".str";
        string output_name = string(output_orig).substr(delimit_j+1);
        FILE *output_file;
        output_file = fopen(output_name.c_str(), "w");
        dump_stringset(output_file);
        fclose(output_file);
    }
    return get_exitstatus();
}

