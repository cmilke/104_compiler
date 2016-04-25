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

%%

{IDENT}         { return TOK_IDENT; }
{NUMBER}        { return TOK_INT; }
"+"             { return '+'; }
"-"             { return '-'; }
"*"             { return '*'; }
"/"             { return '/'; }
"="             { return '='; }
";"             { return ';'; }
\n              { lexer_newline(); }
[ \t]+          { lexer_useraction(); }
"#".*           { lexer_include(); }
.               { return ERROR; }

%%

