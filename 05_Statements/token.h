// token.h

#ifndef TOKEN_H
#define TOKEN_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define TEXTLEN 512

enum {
    T_EOF,
    T_PLUS,
    T_MINUS,
    T_STAR,
    T_SLASH,
    T_INTLIT,
    T_SEMI,
    T_PRINT,
};

struct token {
    int token;
    int intvalue;
};

#endif