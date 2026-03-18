// 04_Assembly/frontend/scan.h

#ifndef SCAN_H
#define SCAN_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define TEXTLEN 512

enum {
    T_EOF, T_PLUS, T_MINUS, T_STAR, T_SLASH, T_INTLIT, T_SEMI, T_EQUALS,
    T_IDENT,

    // Keywords
    T_PRINT, T_INT
};

struct token {
    int token;
    int intvalue;
};

#ifndef extern_
    #define extern_ extern
#endif

extern_ int Line;
extern_ int Putback;
extern_ FILE *Infile;
extern_ struct token Token;
extern_ char Text[TEXTLEN + 1];

#ifdef extern_
    #undef extern_
#endif

int scan(struct token *t);

#endif