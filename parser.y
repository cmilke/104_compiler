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


astree* create_fielddecl(astree* fielddecl, astree* basetype, astree* field,
                         astree* semicolon) {

    free_ast(semicolon); 
    field->symbol = TOK_FIELD;
    adopt1(basetype,field);
    adopt1(fielddecl,basetype);
    return fielddecl;
}

astree* create_fielddecl_arr(astree* fielddecl, astree* basetype, astree* arr,
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


astree* create_vardecl(astree* identdecl, astree* equals, astree* expr, astree* semicolon) {
    free_ast(semicolon);
    equals->symbol = TOK_VARDECL;
    adopt2(equals,identdecl,expr); 
    return equals;
}


astree* create_ifelse(astree* tok_if, astree* lparen, astree* expr, astree* rparen,
            astree* statement1, astree* tok_else, astree* statement2) {
    
    free_ast3(lparen, rparen, tok_else);

    tok_if->symbol = TOK_IFELSE;
    adopt2(tok_if, expr, statement1);
    adopt1(tok_if, statement2);

    return tok_if;
}

astree* create_return(astree* tok_return, astree* semicolon) {
    free_ast(semicolon); 
    tok_return->symbol = TOK_RETURNVOID;
    return tok_return;
}



astree* create_allocator_ident(astree* tok_new, astree* tok_ident, astree* lparen, astree* rparen) {
    free_ast2(lparen,rparen);
    tok_ident->symbol = TOK_TYPEID;
    adopt1(tok_new,tok_ident);
    return tok_new;
}


astree* create_allocator_string(astree* tok_new, astree* tok_string, astree* lparen, astree* rparen) {
    free_ast2(lparen,rparen);
    tok_new->symbol = TOK_NEWSTRING;
    adopt1(tok_new,tok_string);
    return tok_new;
}


astree* create_allocator_array(astree* tok_new, astree* tok_basetype, astree* lbracket, 
                               astree* expr, astree* rbracket) {

    free_ast2(lbracket,rbracket);
    tok_new->symbol = TOK_NEWARRAY;
    adopt2(tok_new,tok_basetype,expr);
    return tok_new;
}


astree* create_call(astree* tok_ident, astree* lparen, astree* call_args, astree* rparen) {
    free_ast(rparen);
    lparen->symbol = TOK_CALL;
    adopt1(lparen,tok_ident);
    transfer_children_and_die(call_args,lparen);
    return lparen;
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
%token TOK_VARDECL TOK_RETURNVOID TOK_NEWSTRING

%right TOK_IF TOK_ELSE
%right '='
%left  TOK_EQ TOK_NE TOK_LT TOK_LE TOK_GT TOK_GE
%left  '+' '-'
%left  '*' '/' '%'
%right TOK_POS TOK_NEG '!' TOK_NEW TOK_ORD TOK_CHR

%start program

%%

program     : body                                              { yyparse_astree = $1; }
            ;
body        : body structdef                                    { $$ = adopt1($1, $2); }
            | body function                                     { $$ = adopt1($1, $2); }
            | body statement                                    { $$ = adopt1($1, $2); }
            | /*nothing*/                                       { $$ = new_treeroot(TOK_ROOT, "PROGRAM_ROOT"); }
            ;
structdef   : TOK_STRUCT TOK_IDENT '{' fielddecl '}'            { $$ = create_structdef($1,$2,$3,$4,$5); }
            ;
fielddecl   : fielddecl basetype TOK_IDENT ';'                  { $$ = create_fielddecl($1,$2,$3,$4); }
            | fielddecl basetype TOK_ARRAY TOK_IDENT ';'        { $$ = create_fielddecl_arr($1,$2,$3,$4,$5); }
            | /*nothing*/                                       { $$ = new_treeroot(TOK_TEMP,"FIELDDECL ROOT"); }
            ;
basetype    : TOK_VOID                                          { $$ = $1; }
            | TOK_BOOL                                          { $$ = $1; }
            | TOK_CHAR                                          { $$ = $1; }
            | TOK_INT                                           { $$ = $1; }
            | TOK_STRING                                        { $$ = $1; }
            | TOK_IDENT                                         { $1->symbol = TOK_TYPEID; $$ = $1; }
            ;
function    : identdecl '(' funargs ')' block                   { $$ = create_function($1,$2,$3,$4,$5); }
            ;
identdecl   : basetype TOK_IDENT                                { $$ = create_identdecl($1,$2); }
            | basetype TOK_ARRAY TOK_IDENT                      { $$ = create_identdecl_arr($1,$2,$3); }
            ;
funargs     : funargs identdecl                                 { $$ = adopt1($1,$2); }
            | funargs ',' identdecl                             { free_ast($2); $$ = adopt1($1,$3); }
            | /*nothing*/                                       { $$ = new_treeroot(TOK_TEMP,"TEMP FUNARGS ROOT"); }
            ;   
block       : '{' block_ops '}'                                 { $$ = create_block($1,$2,$3); }
            | ';'                                               { $1->symbol = TOK_BLOCK; $$ = $1; }
            ;
block_ops   : block_ops statement                               { $$ = adopt1($1,$2); }
            | /*nothing*/                                       { $$ = new_treeroot(TOK_TEMP,"TEMP BLOCK_OPS ROOT"); }
            ;
statement   : block                                             { $$ = $1; }
            | vardecl                                           { $$ = $1; }
            | while                                             { $$ = $1; }
            | ifelse                                            { $$ = $1; }
            | return                                            { $$ = $1; }
            | expr ';'                                          { free_ast($2); $$=$1; };
            ;
vardecl     : identdecl '=' expr ';'                            { $$ = create_vardecl($1,$2,$3,$4); }
            ;
while       : TOK_WHILE '(' expr ')' statement                  { free_ast2($2,$4); $$ = adopt2($1,$3,$5); }
            ;
ifelse      : TOK_IF '(' expr ')' statement                     { free_ast2($2,$4); $$ = adopt2($1,$3,$5); }
            | TOK_IF '(' expr ')' statement TOK_ELSE statement  { $$ = create_ifelse($1,$2,$3,$4,$5,$6,$7); }
            ;
return      : TOK_RETURN ';'                                    { $$ = create_return($1,$2); }
            | TOK_RETURN expr ';'                               { free_ast($3); $$ = adopt1($1,$2); }
            ;
binop       : expr '+' expr                                     { $$=adopt2($2,$1,$3); }
            | expr '-' expr                                     { $$=adopt2($2,$1,$3); }
            | expr '*' expr                                     { $$=adopt2($2,$1,$3); }
            | expr '/' expr                                     { $$=adopt2($2,$1,$3); }
            | expr '%' expr                                     { $$=adopt2($2,$1,$3); }
            | expr '=' expr                                     { $$=adopt2($2,$1,$3); }
            | expr TOK_EQ expr                                  { $$=adopt2($2,$1,$3); }
            | expr TOK_NE expr                                  { $$=adopt2($2,$1,$3); }
            | expr TOK_GT expr                                  { $$=adopt2($2,$1,$3); }
            | expr TOK_GE expr                                  { $$=adopt2($2,$1,$3); }
            | expr TOK_LT expr                                  { $$=adopt2($2,$1,$3); }
            | expr TOK_LE expr                                  { $$=adopt2($2,$1,$3); }
            ;
unop        : '+' expr                                          { $1->symbol=TOK_POS; $$=adopt1($1,$2); }
            | '-' expr                                          { $1->symbol=TOK_NEG; $$=adopt1($1,$2); }
            | '!' expr                                          { $$=adopt1($1,$2); }
            | TOK_ORD expr                                      { $$=adopt1($1,$2); }
            | TOK_CHR expr                                      { $$=adopt1($1,$2); }
            ;
expr        : binop                                             { $$=$1; }
            | unop                                              { $$=$1; }
            | allocator                                         { $$=$1; }
            | call                                              { $$=$1; }
            | '(' expr ')'                                      { free_ast2($1,$3); $$ = $2; }
            | variable                                          { $$=$1; }
            | constant                                          { $$=$1; }
            ;
allocator   : TOK_NEW TOK_IDENT '(' ')'                         { $$=create_allocator_ident($1,$2,$3,$4); }
            | TOK_NEW TOK_STRING '(' ')'                        { $$=create_allocator_string($1,$2,$3,$4); }
            | TOK_NEW basetype '[' expr ']'                     { $$=create_allocator_array($1,$2,$3,$4,$5); }
            ;
call_args   : call_args expr                                    { $$=adopt1($1,$2); }
            | call_args ',' expr                                { free_ast($2); $$=adopt1($1,$3); }
            | /*nothing*/                                       { $$=new_treeroot(TOK_TEMP,"TEMP CALL ROOT"); }
            ;
call        : TOK_IDENT '(' call_args ')'                       { $$=create_call($1,$2,$3,$4); }
            ;
variable    : TOK_IDENT                                         { $$=$1; }
            | expr '[' expr ']'                                 { free_ast($4); $$=adopt2($2,$1,$3); }
            | expr '.' TOK_IDENT                                { $3->symbol=TOK_FIELD; $$=adopt2($2,$1,$3); }
            ;
constant    : TOK_INTCON                                        { $$=$1; }
            | TOK_CHARCON                                       { $$=$1; }
            | TOK_STRINGCON                                     { $$=$1; }
            | TOK_TRUE                                          { $$=$1; }
            | TOK_FALSE                                         { $$=$1; }
            | TOK_NULL                                          { $$=$1; }
            ;
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

