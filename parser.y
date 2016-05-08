%{
// Dummy parser for scanner project.

#include <cassert>

#include "lyutils.h"
#include "astree.h"
#include "stdio.h"

%}

%debug
%defines
%error-verbose
%token-table
%verbose

%initial-action {
   yyparse_astree = new_parseroot();
}

%token TOK_VOID TOK_BOOL TOK_CHAR TOK_INT TOK_STRING
%token TOK_IF TOK_ELSE TOK_WHILE TOK_RETURN TOK_STRUCT
%token TOK_FALSE TOK_TRUE TOK_NULL TOK_NEW TOK_ARRAY
%token TOK_EQ TOK_NE TOK_LT TOK_LE TOK_GT TOK_GE
%token TOK_IDENT TOK_INTCON TOK_CHARCON TOK_STRINGCON

%token TOK_BLOCK TOK_CALL TOK_IFELSE TOK_INITDECL
%token TOK_POS TOK_NEG TOK_NEWARRAY TOK_TYPEID TOK_FIELD
%token TOK_ORD TOK_CHR TOK_ROOT

%left '+' '-'
%left '*' '/' '%'

%start program

%%

program     : op {dprint("PROGRAM",yyparse_astree);} ;

op          : op structdef  { $$ = adopt1(yyparse_astree, $2); }
            | op function   { $$ = adopt1(yyparse_astree, $2); }
            | op statement  { $$ = adopt1(yyparse_astree, $2); dprint("op",$2);}
            | /*nothing*/ 
            ;

structdef   : ;

basetype    : TOK_VOID | TOK_BOOL | TOK_CHAR | TOK_INT | TOK_STRING ;

function    : ;

binop       : expr '+' expr   { $$=adopt2($2,$1,$3); dprint("expr",$1,$2,$3); }
            | expr '-' expr   { $$=adopt2($2,$1,$3); dprint("expr",$1,$2,$3); }
            | expr '*' expr   { $$=adopt2($2,$1,$3); dprint("expr",$1,$2,$3); }
            | expr '/' expr   { $$=adopt2($2,$1,$3); dprint("expr",$1,$2,$3); }

statement   : expr ';' { free_ast($2); $$=$1; dprint("statement",$1); };  
            ;

expr        : binop             { $$=$1; dprint("expr",$1); }
            | call 
            | '(' expr ')'
            | variable          { $$=$1; dprint("expr",$1); }
            | constant          { $$=$1; dprint("expr",$1); }
            ;

args        : ',' expr | ',' expr args ;

call        : TOK_IDENT '(' ')' 
            | TOK_IDENT '(' expr ')' 
            | TOK_IDENT '(' expr args ')'
            ;

variable    : TOK_IDENT         { $$=$1; dprint("var",$1); }
            ;

constant    : TOK_INTCON        { $$=$1; dprint("const",$1); }
            | TOK_CHARCON       { $$=$1; dprint("const",$1); }
            | TOK_TRUE          { $$=$1; dprint("const",$1); }
            | TOK_FALSE         { $$=$1; dprint("const",$1); }
            ;
         


/*token   : '(' | ')' | '[' | ']' | '{' | '}' | ';' | ',' | '.'
        | '=' | '+' | '-' | '*' | '/' | '%' | '!'
        | TOK_VOID | TOK_BOOL | TOK_CHAR | TOK_INT | TOK_STRING
        | TOK_IF | TOK_ELSE | TOK_WHILE | TOK_RETURN | TOK_STRUCT
        | TOK_FALSE | TOK_TRUE | TOK_NULL | TOK_NEW | TOK_ARRAY | TOK_NEWARRAY
        | TOK_EQ | TOK_NE | TOK_LT | TOK_LE | TOK_GT | TOK_GE
        | TOK_IDENT | TOK_INTCON | TOK_CHARCON | TOK_STRINGCON
        | TOK_ORD | TOK_CHR | TOK_ROOT | TOK_NEG
        ;*/

%%


const char *get_yytname (int symbol) {
   return yytname [YYTRANSLATE (symbol)];
}


bool is_defined_token (int symbol) {
   return YYTRANSLATE (symbol) > YYUNDEFTOK;
}

/*
static void* yycalloc (size_t size) {
   void* result = calloc (1, size);
   assert (result != nullptr);
   return result;
}
*/

