// global.h

#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdio.h>
#include "token.h"

#ifndef extern_
    #define extern_ extern
#endif

extern_ int Line;
extern_ int Putback;
extern_ FILE *Infile;
extern_ struct token Token;

#endif