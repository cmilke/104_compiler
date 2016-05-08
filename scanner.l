%{
// $Id: lexer.l,v 1.9 2014-10-09 18:39:47-07 - - $
#include "tokens.h"
#include "lyutils.h"

%}

%option 8bit
%option debug
%option nodefault
%option nounput
%option noyywrap
%option verbose
%option warn

LETTER          [a-zA-Z_]
DIGIT           [0-9]
IDENT           {LETTER}({LETTER}|{DIGIT})*
NUMBER          {DIGIT}+
MISC            [ !#-&(-/:-@\[-`\{-~]
ESCAPE          \\[abfnrtv\\'\"?]
CHAR            '({LETTER}|{DIGIT}|{ESCAPE}|{MISC})'
STRING          \"({LETTER}|{DIGIT}|{ESCAPE}|{MISC})*\"

%%
{STRING}        { return yylval_token(TOKSTRINGCON); }
{CHAR}          { return yylval_token(TOKCHARCON); }
"#".*           { lexer_include(); }
\n              { lexer_newline(); }
[ \t]+          { lexer_useraction(); }
"void"          { return yylval_token(TOKVOID); }
"bool"          { return yylval_token(TOKBOOL); }
"char"          { return yylval_token(TOKCHAR); }
"int"           { return yylval_token(TOKINT); }
"string"        { return yylval_token(TOKSTRING); }
"struct"        { return yylval_token(TOKSTRUCT); }
"if"            { return yylval_token(TOKIF); }
"else"          { return yylval_token(TOKELSE); }
"while"         { return yylval_token(TOKWHILE); }
"return"        { return yylval_token(TOKRETURN); }
"false"         { return yylval_token(TOKFALSE); }
"true"          { return yylval_token(TOKTRUE); }
"null"          { return yylval_token(TOKNULL); }
"new"           { return yylval_token(TOKNEW); }
"ord"           { return yylval_token(TOKORD); }
"chr"           { return yylval_token(TOKCHR); }
{IDENT}         { return yylval_token(TOKIDENT); }
{NUMBER}        { return yylval_token(TOKINTCON); }
"=="            { return yylval_token(TOKEQ); }
"!="            { return yylval_token(TOKNE); }
"<"             { return yylval_token(TOKLT); }
"<="            { return yylval_token(TOKLE); }
">"             { return yylval_token(TOKGT); }
">="            { return yylval_token(TOKGE); }
"[]"            { return yylval_token(TOKNEWARRAY); }
"+"             { return yylval_token('+'); }
"-"             { return yylval_token('-'); }
"*"             { return yylval_token('*'); }
"/"             { return yylval_token('/'); }
"="             { return yylval_token('='); }
";"             { return yylval_token(';'); }
"{"             { return yylval_token('{'); }
"}"             { return yylval_token('}'); }
"["             { return yylval_token('['); }
"]"             { return yylval_token(']'); }
"("             { return yylval_token('('); }
")"             { return yylval_token(')'); }
","             { return yylval_token(','); }
.               { return yylval_token(ERROR); }

%%

