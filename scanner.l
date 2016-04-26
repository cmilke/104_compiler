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
IDENT        {LETTER}({LETTER}|{DIGIT})*
NUMBER          {DIGIT}+
MISC            [ !#-&(-/:-@\[-`\{-~]
ESCAPE          \\[abfnrtv\\'\"?]
CHAR            '({LETTER}|{DIGIT}|{ESCAPE}|{MISC})'
STRING          \"({LETTER}|{DIGIT}|{ESCAPE}|{MISC})*\"

%%
{STRING}        { return TOKSTRING; }
{CHAR}          { return TOKCHAR; }
{IDENT}         { return TOKIDENT; }
"#".*           { lexer_include(); }
\n              { lexer_newline(); }
[ \t]+          { lexer_useraction(); }
"void"          { return TOKVOID; }
"bool"          { return TOKBOOL; }
{NUMBER}        { return TOKINT; }
"if"            { return TOKIF; }
"else"          { return TOKELSE; }
"while"         { return TOKWHILE; }
"return"        { return TOKRETURN; }
"struct"        { return TOKSTRUCT; }
"false"         { return TOKFALSE; }
"true"          { return TOKTRUE; }
"null"          { return TOKNULL; }
"new"           { return TOKNEW; }
"=="            { return TOKEQ; }
"!="            { return TOKNE; }
"<"             { return TOKLT; }
"<="            { return TOKLE; }
">"             { return TOKGT; }
">="            { return TOKGE; }
"+"             { return TOKPOS; }
"-"             { return TOKNEG; }
"[]"            { return TOKNEWARRAY; }
"ord"           { return TOKORD; }
"chr"           { return TOKCHR; }
"+"             { return '+'; }
"-"             { return '-'; }
"*"             { return '*'; }
"/"             { return '/'; }
"="             { return '='; }
";"             { return ';'; }
"{"             { return '{'; }
"}"             { return '}'; }
"["             { return '['; }
"]"             { return ']'; }
"("             { return '('; }
")"             { return ')'; }
","             { return ','; }
.               { return ERROR; }

%%

