#include "typechecking.h"

attr_bitset typemask = attr_bitset(0x1ff20);



attr_bitset compare_types(attr_bitset lval_type, attr_bitset rval_type) {
    if ( (lval_type&typemask) != (rval_type&typemask) ) {
        printf("FAILURE - TYPES DO NOT MATCH!!!\n");
        return -1;
    } else {
        return (lval_type&typemask);
    }
}
