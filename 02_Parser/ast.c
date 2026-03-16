// ast.c

#include <stdlib.h>
#include <stdio.h>
#include "ast.h"

/**
 * @brief 创建 AST 节点
 * @param op 操作符
 * @param left 左子节点
 * @param right 右子节点
 * @param intvalue 整数值
 * @return 新创建的 AST 节点
 */
struct ASTnode *mkastnode(int op, struct ASTnode *left, struct ASTnode *right, int intvalue) {
    struct ASTnode *n = malloc(sizeof(struct ASTnode));
    if(n == NULL) {
        fprintf(stderr, "Out of memory\n");
        exit(1);
    }
    n->op = op;
    n->left = left;
    n->right = right;
    n->intvalue = intvalue;
    return n;
}

/**
 * @brief 创建 AST 叶子节点
 * @param op 操作符
 * @param intvalue 整数值
 * @return 新创建的 AST 叶子节点
 */
struct ASTnode *mkastleaf(int op, int intvalue) {
    return mkastnode(op, NULL, NULL, intvalue);
}

/**
 * @brief 创建 AST 一元节点
 * @param op 操作符
 * @param left 左子节点
 * @param intvalue 整数值
 * @return 新创建的 AST 一元节点
 */
struct ASTnode *mkastunary(int op, struct ASTnode *left, int intvalue) {
    return mkastnode(op, left, NULL, intvalue);
}

static char *ASTop[] = { "+", "-", "*", "/" };

/**
 * @brief 解释 AST 节点
 * @param n AST 节点
 * @return 节点的值
 */
int interpretAST(struct ASTnode *n) {
    int leftval, rightval;

    if(n->left) {
        leftval = interpretAST(n->left);
    }

    if(n->right) {
        rightval = interpretAST(n->right);
    }

    if(n->op == A_INTLIT) {
        printf("int %d\n", n->intvalue);
    }
    else {
        printf("%d %s %d\n", leftval, ASTop[n->op], rightval);
    }
        
    switch(n->op) {
    case A_ADD:
        return leftval + rightval;
    case A_SUB:
        return leftval - rightval;
    case A_MUL:
        return leftval * rightval;
    case A_DIV:
        return leftval / rightval;
    case A_INTLIT:
        return n->intvalue;
    default:
        fprintf(stderr, "Unknown AST operator %d\n", n->op);
        exit(1);
    }
}