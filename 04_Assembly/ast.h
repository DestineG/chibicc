// ast.h

#ifndef AST_H
#define AST_H

#include <ctype.h>

enum {
    A_ADD,
    A_SUB,
    A_MUL,
    A_DIV,
    A_INTLIT,
};

struct ASTnode {
    int op;
    struct ASTnode *left;
    struct ASTnode *right;
    int intvalue;
};

struct ASTnode *mkastnode(int op, struct ASTnode *left, struct ASTnode *right, int intvalue);
struct ASTnode *mkastleaf(int op, int intvalue);
struct ASTnode *mkastunary(int op, struct ASTnode *left, int intvalue);
struct ASTnode *binexpr(int ptp);
int interpretAST(struct ASTnode *n);
void dumpAST_level_tree(struct ASTnode *root);

#endif