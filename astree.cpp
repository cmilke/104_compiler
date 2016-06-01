#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

#include "astree.h"
#include "auxlib.h"
#include "lyutils.h"
#include "stringset.h"



astree::astree (int symbol, int filenr, int linenr, int offset,
                size_t block_nr, attr_bitset attributes, const char* clexinfo,
                bool is_symbol, struct symbol* declid):
        symbol (symbol), filenr (filenr), linenr (linenr),
        offset (offset), block_nr (block_nr),
        attributes( attributes ), lexinfo (intern_stringset (clexinfo) ),
        is_symbol( is_symbol ), declid( declid ) {

    DEBUGF ('f', "astree %p->{%d:%d.%d: %s: \"%s\"}\n",
            (void*) this, filenr, linenr, offset,
            get_yytname (symbol), lexinfo->c_str());
    }

astree* adopt1 (astree* root, astree* child) {
    root->children.push_back (child);
    DEBUGF ('a', "%p (%s) adopting %p (%s)\n",
            root, root->lexinfo->c_str(),
            child, child->lexinfo->c_str());
    return root;
}

astree* adopt2 (astree* root, astree* left, astree* right) {
    adopt1 (root, left);
    adopt1 (root, right);
    return root;
}


static void dump_node (FILE* outfile, astree* node) {
    fprintf (outfile, "%p->{%s(%d) %ld:%ld.%03ld \"%s\" [",
            node, get_yytname (node->symbol), node->symbol,
            node->filenr, node->linenr, node->offset,
            node->lexinfo->c_str());
    bool need_space = false;
    for (size_t child = 0; child < node->children.size();
            ++child) {
        if (need_space) fprintf (outfile, " ");
        need_space = true;
        fprintf (outfile, "%p", node->children.at(child));
    }
    fprintf (outfile, "]}");
}

static void dump_astree_rec (FILE* outfile, astree* root,
        int depth) {
    if (root == NULL) return;
    fprintf (outfile, "%*s%s ", depth * 3, "",
            root->lexinfo->c_str());
    dump_node (outfile, root);
    fprintf (outfile, "\n");
    for (size_t child = 0; child < root->children.size();
            ++child) {
        dump_astree_rec (outfile, root->children[child],
                depth + 1);
    }
}

void dump_astree (FILE* outfile, astree* root) {
    dump_astree_rec (outfile, root, 0);
    fflush (NULL);
}

void yyprint (FILE* outfile, unsigned short toknum,
        astree* yyvaluep) {
    if (is_defined_token (toknum)) {
        dump_node (outfile, yyvaluep);
    }else {
        fprintf (outfile, "%s(%d)\n",
                get_yytname (toknum), toknum);
    }
    fflush (NULL);
}

void transfer_children_and_die (astree* oldparent, astree* newparent) {
    for( auto child : oldparent->children ) {
        adopt1(newparent, child);
    }
    delete oldparent;
}

void free_ast (astree* root) {
    while (not root->children.empty()) {
        astree* child = root->children.back();
        root->children.pop_back();
        free_ast (child);
    }
    DEBUGF ('f', "free [%p]-> %d:%d.%d: %s: \"%s\")\n",
            root, root->filenr, root->linenr, root->offset,
            get_yytname (root->symbol), root->lexinfo->c_str());
    delete root;
}

void free_ast2 (astree* tree1, astree* tree2) {
    free_ast (tree1);
    free_ast (tree2);
}

void free_ast3 (astree* tree1, astree* tree2, astree* tree3) {
    free_ast (tree1);
    free_ast (tree2);
    free_ast (tree3);
}

