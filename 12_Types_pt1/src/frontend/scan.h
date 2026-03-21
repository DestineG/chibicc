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
    // + - * /
    T_PLUS, T_MINUS, T_STAR, T_SLASH,
    // == !=
    T_EQ, T_NE,
    // < > <= >=
    T_LT, T_GT, T_LE, T_GE,
    // int ; = identifier
    T_INTLIT, T_SEMI, T_ASSIGN, T_IDENT,
    // { } ( )
    T_LBRACE, T_RBRACE, T_LPAREN, T_RPAREN,

    // Keywords
    T_PRINT, T_INT, T_IF, T_ELSE, T_WHILE,
    T_FOR, T_VOID, T_CHAR
};

struct token {
    int token;
    int intvalue;
};

extern int Line;
extern int Putback;
extern FILE *Infile;
extern struct token Token;
extern char Text[TEXTLEN + 1];

int scan(struct token *t);

#endif