#ifndef __LYUTILS_H__
#define __LYUTILS_H__

// Lex and Yacc interface utility.

#include <stdio.h>

#include "astree.h"
#include "auxlib.h"

#define YYEOF 0

extern FILE* yyin;
extern astree* yyparse_astree;
extern int yyin_linenr;
extern char* yytext;
extern int yy_flex_debug;
extern int yydebug;
//extern int yyleng;

int yylex (void);
int yyparse (void);
void yyerror (const char* message);
int yylex_destroy (void);
const char* get_yytname (int symbol);
bool is_defined_token (int symbol);

const string* lexer_filename (int filenr);
void lexer_newfilename (const char* filename);
void lexer_badchar (unsigned char bad);
void lexer_badtoken (char* lexeme);
void lexer_newline (void);
void lexer_setecho (bool echoflag);
void lexer_useraction (void);

astree* new_parseroot (void);
astree* new_treeroot (int symbol, string lexicalinfo);
int yylval_token (int symbol);
astree* get_newtree(int symbol);

void lexer_include (void);

typedef astree* astree_pointer;


void dprint (string dtype, astree* d1);
void dprint (string dtype, astree* d1, astree* d2);
void dprint (string dtype, astree* d1, astree* d2, astree* d3);
void dprint (string dtype, astree* d1, astree* d2, astree* d3, astree* d4);
void dprint (string dtype, astree* d1, astree* d2, astree* d3, astree* d4, astree* d5);


#define YYSTYPE astree_pointer
#include "yyparse.h"

#endif
