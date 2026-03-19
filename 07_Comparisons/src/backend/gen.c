// 07_Comparisons/src/backend/gen.c

#include <stdlib.h>
#include <stdio.h>

#include "../midend/ast.h"
#include "../midend/symbol.h"
#include "cgen.h"

/**
 * @brief 生成AST节点的汇编代码
 * @param n AST节点指针
 * @return 结果寄存器索引
 */
int translateAst(struct ASTnode *n, int reg) {
    int leftreg, rightreg;

    if(n->left) {
        leftreg = translateAst(n->left, -1);
    }
    if(n->right) {
        rightreg = translateAst(n->right, leftreg);
    }

    switch(n->op) {
        case A_ADD:
            return cgadd(leftreg, rightreg);
        case A_SUB:
            return cgsub(leftreg, rightreg);
        case A_MUL:
            return cgmul(leftreg, rightreg);
        case A_DIV:
            return cgdiv(leftreg, rightreg);
        case A_EQ:
            return cgequal(leftreg, rightreg);
        case A_NE:
            return cgnotequal(leftreg, rightreg);
        case A_LT:
            return cglessthan(leftreg, rightreg);
        case A_GT:
            return cggreaterthan(leftreg, rightreg);
        case A_LE:
            return cglessequal(leftreg, rightreg);
        case A_GE:
            return cggreaterequal(leftreg, rightreg);
        case A_INTLIT:
            return (cgloadint(n->v.intvalue));
        case A_IDENT:
            return (cgloadglob(Gsym[n->v.id].name));
        case A_LVIDENT:
            return (cgstorglob(reg, Gsym[n->v.id].name));
        case A_ASSIGN:
            return (rightreg);
        default:
            fprintf(stderr, "genAST: unexpected operator %d\n", n->op);
            exit(1);
    }
}

/**
 * @brief 生成程序的汇编代码前导部分
 */
void genpreamble() {
  cgpreamble();
}

/**
 * @brief 生成程序的汇编代码后缀部分
 */
void genpostamble() {
  cgpostamble();
}

/**
 * @brief 初始化寄存器
 */
void genfreeregs() {
  initreg();
}

/**
 * @brief 生成打印整数的汇编代码
 * @param reg 整数的寄存器索引
 */
void genprintint(int reg) {
  cgprintint(reg);
}