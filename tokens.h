// $Id: tokens.h,v 1.7 2014-10-09 18:39:25-07 - - $

#ifndef __TOKENS_H__
#define __TOKENS_H__

#define YYEOF           0 
#define ERROR          -1
#define TOKVOID       258
#define TOKBOOL       259
#define TOKCHAR       260
#define TOKINT        261
#define TOKSTRING     262
#define TOKIF         263
#define TOKELSE       264
#define TOKWHILE      265
#define TOKRETURN     266
#define TOKSTRUCT     267
#define TOKFALSE      268
#define TOKTRUE       269
#define TOKNULL       270
#define TOKNEW        271
#define TOKARRAY      272
#define TOKEQ         273
#define TOKNE         274
#define TOKLT         275
#define TOKLE         276
#define TOKGT         277
#define TOKGE         278
#define TOKIDENT      279
#define TOKINTCON     280
#define TOKCHARCON    281
#define TOKSTRINGCON  282
#define TOKBLOCK      283
#define TOKCALL       284
#define TOKIFELSE     285
#define TOKINITDECL   286
#define TOKPOS        287
#define TOKNEG        288
#define TOKNEWARRAY   289
#define TOKTYPEID     290
#define TOKFIELD      291
#define TOKORD        292
#define TOKCHR        293
#define TOKROOT       294

extern int yy_flex_debug;
extern char* yytext;
extern int yylex (void); 

#endif

