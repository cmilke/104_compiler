%{
// $Id: lexer.l,v 1.9 2014-10-09 18:39:47-07 - - $
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
{STRING}        { return yylval_token(TOK_STRINGCON); }
{CHAR}          { return yylval_token(TOK_CHARCON); }
"#".*           { lexer_include(); }
\n              { lexer_newline(); }
[ \t]+          { lexer_useraction(); }
"void"          { return yylval_token(TOK_VOID); }
"bool"          { return yylval_token(TOK_BOOL); }
"char"          { return yylval_token(TOK_CHAR); }
"int"           { return yylval_token(TOK_INT); }
"string"        { return yylval_token(TOK_STRING); }
"struct"        { return yylval_token(TOK_STRUCT); }
"if"            { return yylval_token(TOK_IF); }
"else"          { return yylval_token(TOK_ELSE); }
"while"         { return yylval_token(TOK_WHILE); }
"return"        { return yylval_token(TOK_RETURN); }
"false"         { return yylval_token(TOK_FALSE); }
"true"          { return yylval_token(TOK_TRUE); }
"null"          { return yylval_token(TOK_NULL); }
"new"           { return yylval_token(TOK_NEW); }
"ord"           { return yylval_token(TOK_ORD); }
"chr"           { return yylval_token(TOK_CHR); }
{IDENT}         { return yylval_token(TOK_IDENT); }
{NUMBER}        { return yylval_token(TOK_INTCON); }
"=="            { return yylval_token(TOK_EQ); }
"!="            { return yylval_token(TOK_NE); }
"<"             { return yylval_token(TOK_LT); }
"<="            { return yylval_token(TOK_LE); }
">"             { return yylval_token(TOK_GT); }
">="            { return yylval_token(TOK_GE); }
"[]"            { return yylval_token(TOK_ARRAY); }
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
.               { return yylval_token(-1); }

%%

