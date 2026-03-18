// 04_Assembly/frontend/scan.h

#ifndef SCAN_H
#define SCAN_H

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

#ifndef extern_
    #define extern_ extern
#endif

extern_ struct token Token;
extern_ char Text[TEXTLEN + 1];

int scan(struct token *t);

#endif