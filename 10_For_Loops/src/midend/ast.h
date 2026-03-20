// ast.h

#ifndef AST_H
#define AST_H

#include <ctype.h>

enum {
    // + - * /
    A_ADD=1, A_SUB, A_MUL, A_DIV,
    // == !=
    A_EQ, A_NE,
    // < > <= >=
    A_LT, A_GT, A_LE, A_GE,
    // int = identifier leftIdentifier
    A_INTLIT, A_ASSIGN, A_IDENT, A_LVIDENT,
    // print glue if while
    A_PRINT, A_GLUE, A_IF, A_WHILE
};

#define NOREG -1

struct ASTnode {
    int op;
    struct ASTnode *left;
    struct ASTnode *mid;
    struct ASTnode *right;
    union {
        int id;
        int intvalue;
    } v;
};

struct ASTnode *mkastnode(
    int op, 
    struct ASTnode *left,
    struct ASTnode *mid,
    struct ASTnode *right,
    int intvalue
);
struct ASTnode *mkastleaf(int op, int intvalue);
struct ASTnode *mkastunary(int op, struct ASTnode *left, int intvalue);
int interpretAST(struct ASTnode *n);
void dumpAST_level_tree(struct ASTnode *root);

#endif