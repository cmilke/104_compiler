using namespace std;

#include "astree.h"
#include "symtable.h"
#include "typechecking.h"


bool ERROR_THROWN = false;



void throw_error( const string* lex1, attr_bitset type1, size_t filenr1,
            size_t linenr1, size_t offset1, size_t block_nr1, string error) {

    char errormessage[1000];

    sprintf(errormessage,
        "COMPILE ERROR: %s\nOCCURED ON SYMBOL:\n"
        "\t%s - type: %05lX f: %ld [%ld,%ld] {%ld}\n",
        error.c_str(), 
        lex1->c_str(), type1.to_ulong(), filenr1, linenr1, offset1, block_nr1);

    fprintf(stderr,errormessage);
    ERROR_THROWN = true;
}



void throw_error(astree* root1, string error) {
    const string* lex1 = root1->lexinfo;
    attr_bitset type1 = root1->attributes.to_ulong();
    size_t filenr1 = root1->filenr;
    size_t linenr1 = root1->linenr;
    size_t offset1 = root1->offset;
    size_t block_nr1 = root1->block_nr;

    throw_error( lex1,type1, filenr1, linenr1, offset1, block_nr1, error);
}



void throw_error( const string* lex1, attr_bitset type1, size_t filenr1,
            size_t linenr1, size_t offset1, size_t block_nr1,
            const string* lex2, attr_bitset type2, size_t filenr2,
            size_t linenr2, size_t offset2, size_t block_nr2, string error) {

    char errormessage[1000];

    sprintf(errormessage,
        "COMPILE ERROR: %s\nCONFLICT BETWEEN:\n"
        "\t%s - type: %05lX f: %ld [%ld,%ld] {%ld} AND\n"
        "\t%s - type: %05lX f: %ld [%ld,%ld] {%ld}\n",
        error.c_str(), 
        lex1->c_str(), type1.to_ulong(), filenr1, linenr1, offset1, block_nr1,
        lex2->c_str(), type2.to_ulong(), filenr2, linenr2, offset2, block_nr2);

    fprintf(stderr,errormessage);
    ERROR_THROWN = true;
}



void throw_error(astree* root1, astree* root2, string error) {
    const string* lex1 = root1->lexinfo;
    attr_bitset type1 = root1->attributes.to_ulong();
    size_t filenr1 = root1->filenr;
    size_t linenr1 = root1->linenr;
    size_t offset1 = root1->offset;
    size_t block_nr1 = root1->block_nr;

    const string* lex2 = root2->lexinfo;
    attr_bitset type2 = root2->attributes.to_ulong();
    size_t filenr2 = root2->filenr;
    size_t linenr2 = root2->linenr;
    size_t offset2 = root2->offset;
    size_t block_nr2 = root2->block_nr;


    throw_error( lex1,type1, filenr1, linenr1, offset1, block_nr1,
            lex2, type2, filenr2, linenr2, offset2, block_nr2, error);
}



void throw_error(astree* root1, symbol* sym2, string error) {
    const string* lex1 = root1->lexinfo;
    attr_bitset type1 = root1->attributes.to_ulong();
    size_t filenr1 = root1->filenr;
    size_t linenr1 = root1->linenr;
    size_t offset1 = root1->offset;
    size_t block_nr1 = root1->block_nr;

    const string* lex2 = sym2->key;
    attr_bitset type2 = sym2->attributes.to_ulong();
    size_t filenr2 = sym2->filenr;
    size_t linenr2 = sym2->linenr;
    size_t offset2 = sym2->offset;
    size_t block_nr2 = sym2->block_nr;


    throw_error( lex1,type1, filenr1, linenr1, offset1, block_nr1,
            lex2, type2, filenr2, linenr2, offset2, block_nr2, error);
}



void throw_error(symbol* sym1, symbol* sym2, string error) {

    const string* lex1 = sym1->key;
    attr_bitset type1 = sym1->attributes.to_ulong();
    size_t filenr1 = sym1->filenr;
    size_t linenr1 = sym1->linenr;
    size_t offset1 = sym1->offset;
    size_t block_nr1 = sym1->block_nr;

    const string* lex2 = sym2->key;
    attr_bitset type2 = sym2->attributes.to_ulong();
    size_t filenr2 = sym2->filenr;
    size_t linenr2 = sym2->linenr;
    size_t offset2 = sym2->offset;
    size_t block_nr2 = sym2->block_nr;

    throw_error( lex1,type1, filenr1, linenr1, offset1, block_nr1,
            lex2, type2, filenr2, linenr2, offset2, block_nr2, error);
}
