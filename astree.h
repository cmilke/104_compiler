#ifndef __ASTREE_H__
#define __ASTREE_H__

#include <inttypes.h>
#include <string>
#include <vector>

#include "typechecking.h"


struct astree {
   int symbol;               // token code
   size_t filenr;            // index into filename stack
   size_t linenr;            // line number from source code
   size_t offset;            // offset of token with current line
   size_t block_nr;          
   attr_bitset attributes;
   const std::string* lexinfo;    // pointer to lexical information
   std::vector<astree*> children; // children of this n-way node
   bool is_symbol;
   struct symbol* declid;
   astree (int symbol, int filenr, int linenr,
           int offset, size_t block_nr, 
           attr_bitset attributes, const char* clexinfo, 
           bool is_symbol, struct symbol* declid);
};

// Append one child to the vector of children.
astree* adopt1 (astree* root, astree* child);

// Append two children to the vector of children.
astree* adopt2 (astree* root, astree* left, astree* right);

// Dump an astree to a FILE.
void dump_astree (FILE* outfile, astree* root);

// Debug print an astree.
void yyprint (FILE* outfile, unsigned short toknum,
              astree* yyvaluep);

// Recursively free an astree.
void transfer_children_and_die (astree* oldparent, astree* newparent);

// Recursively free an astree.
void free_ast (astree* tree);

// Recursively free two astrees.
void free_ast2 (astree* tree1, astree* tree2);

void free_ast3 (astree* tree1, astree* tree2, astree* tree3);

#endif

