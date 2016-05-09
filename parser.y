%{
// Dummy parser for scanner project.

#include <cassert>

#include "lyutils.h"
#include "astree.h"
#include "stdio.h"
#include "auxlib.h"





astree* create_structdef(astree* sstruct, astree* stypeid, astree* lbrace,
                         astree* fielddecl, astree* rbrace) {

    free_ast2(lbrace,rbrace); 
    stypeid->symbol = TOK_TYPEID;
    adopt1(sstruct,stypeid);
    transfer_children_and_die(fielddecl, sstruct);
    
    return sstruct;
}


astree* create_fielddecl_a(astree* fielddecl, astree* basetype, astree* field,
                         astree* semicolon) {

    free_ast(semicolon); 
    field->symbol = TOK_FIELD;
    adopt1(basetype,field);
    adopt1(fielddecl,basetype);
    return fielddecl;
}

astree* create_fielddecl_b(astree* fielddecl, astree* basetype, astree* arr,
                           astree* field, astree* semicolon) {

    free_ast(semicolon); 
    field->symbol = TOK_FIELD;
    adopt2(basetype,arr,field);
    adopt1(fielddecl,basetype);
    return fielddecl;
}


astree* create_function(astree* identifier, astree* lparen, 
            astree* arguments, astree* rparen, astree* block) {

    free_ast(rparen);
    lparen->symbol = TOK_PARAMLIST;
    transfer_children_and_die(arguments,lparen);

    astree* function_root = new_treeroot(TOK_FUNCTION,"FUNCTION ROOT");
    function_root->filenr = identifier->filenr;
    function_root->linenr = identifier->linenr;
    function_root->offset = identifier->offset;

    adopt1(function_root,identifier);
    adopt1(function_root,lparen);
    adopt1(function_root,block);

    return function_root;
}


astree* create_identdecl(astree* basetype, astree* declid) {
    declid->symbol = TOK_DECLID;
    adopt1(basetype,declid);
    return basetype;
}


astree* create_identdecl_arr(astree* basetype, astree* arr, astree* declid) {
    declid->symbol = TOK_DECLID;
    adopt2(basetype,arr,declid);
    return basetype;
}


astree* create_block(astree* lbrace, astree* block_ops, astree* rbrace) {
    free_ast(rbrace);
    lbrace->symbol = TOK_BLOCK;

    transfer_children_and_die(block_ops, lbrace);
    return lbrace;
}



%}

%debug
%defines
%error-verbose
%token-table
%verbose


%token TOK_VOID TOK_BOOL TOK_CHAR TOK_INT TOK_STRING
%token TOK_IF TOK_ELSE TOK_WHILE TOK_RETURN TOK_STRUCT
%token TOK_FALSE TOK_TRUE TOK_NULL TOK_NEW TOK_ARRAY
%token TOK_EQ TOK_NE TOK_LT TOK_LE TOK_GT TOK_GE
%token TOK_IDENT TOK_INTCON TOK_CHARCON TOK_STRINGCON

%token TOK_BLOCK TOK_CALL TOK_IFELSE TOK_DECLID TOK_PARAMLIST
%token TOK_POS TOK_NEG TOK_NEWARRAY TOK_TYPEID TOK_FIELD
%token TOK_ORD TOK_CHR TOK_ROOT TOK_FUNCTION TOK_TEMP

%left '+' '-'
%left '*' '/' '%'

%start program

%%

program     : body { yyparse_astree = $1; } ;

body        : body structdef { $$ = adopt1($1, $2); }
            | body function  { $$ = adopt1($1, $2); }
            | body statement { $$ = adopt1($1, $2); }
            | /*nothing*/    { $$ = new_treeroot(TOK_ROOT, "PROGRAM_ROOT"); }
            ;

structdef   : TOK_STRUCT TOK_IDENT '{' fielddecl '}' { $$ = create_structdef($1,$2,$3,$4,$5); }
            ;

fielddecl   : fielddecl basetype TOK_IDENT ';' { $$ = create_fielddecl_a($1,$2,$3,$4); }
            | fielddecl basetype TOK_ARRAY TOK_IDENT ';'{ $$ = create_fielddecl_b($1,$2,$3,$4,$5); }
            | /*nothing*/ { $$ = new_treeroot(TOK_TEMP,"FIELDDECL ROOT"); }
            ;

basetype    : TOK_VOID | TOK_BOOL | TOK_CHAR | TOK_INT | TOK_STRING
            | TOK_IDENT { $1->symbol = TOK_TYPEID; }
            ;

function    : identdecl '(' funargs ')' block { $$ = create_function($1,$2,$3,$4,$5); };

identdecl   : basetype TOK_IDENT { $$ = create_identdecl($1,$2); }
            | basetype TOK_ARRAY TOK_IDENT { $$ = create_identdecl_arr($1,$2,$3); }
            ;

funargs     : funargs identdecl { $$ = adopt1($1,$2); }
            | funargs ',' identdecl { free_ast($2); $$ = adopt1($1,$3); }
            | /*nothing*/ { $$ = new_treeroot(TOK_TEMP,"TEMP FUNARGS ROOT"); }
            ;   

block       : '{' block_ops '}' { $$ = create_block($1,$2,$3); }
            | ';' { $1->symbol = TOK_BLOCK; $$ = $1; }
            ;

block_ops   : block_ops statement { $$ = adopt1($1,$2); }
            | /*nothing*/ { $$ = new_treeroot(TOK_TEMP,"TEMP BLOCK_OPS ROOT"); }

binop       : expr '+' expr { $$=adopt2($2,$1,$3); }
            | expr '-' expr { $$=adopt2($2,$1,$3); }
            | expr '*' expr { $$=adopt2($2,$1,$3); }
            | expr '/' expr { $$=adopt2($2,$1,$3); }

statement   : block { $$ = $1; }
            | vardecl
            | while
            | ifelse
            | return
            | expr ';' { free_ast($2); $$=$1; };
            ;

vardecl     : ;

while       : ;

ifelse      : ;

return      : ;

expr        : binop { $$=$1; }
            | call 
            | '(' expr ')'
            | variable { $$=$1; }
            | constant { $$=$1; }
            ;

allocator   : ;

call_args        : ',' expr | ',' expr call_args ;

call        : TOK_IDENT '(' ')' 
            | TOK_IDENT '(' expr ')' 
            | TOK_IDENT '(' expr call_args ')'
            ;

variable    : TOK_IDENT         { $$=$1; }
            ;

constant    : TOK_INTCON        { $$=$1; }
            | TOK_CHARCON       { $$=$1; }
            | TOK_TRUE          { $$=$1; }
            | TOK_FALSE         { $$=$1; }
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

