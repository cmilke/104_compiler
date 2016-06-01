#ifndef __OILGEN_H__
#define __OILGEN_H__

#include <vector>

struct astree;

struct global_container {
    std::vector<astree*>* structures;
    std::vector<astree*>* stringconsts;
    std::vector<astree*>* variables;
    std::vector<astree*>* functions;

    global_container();
};


void generate_oil_file(astree* root, global_container* globals);

#endif
