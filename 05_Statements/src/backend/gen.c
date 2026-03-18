// 05_Statements/src/backend/gen.c

#include <stdlib.h>
#include <stdio.h>

#include "cgen.h"

/**
 * @brief 生成AST节点的汇编代码
 * @param n AST节点指针
 * @return 结果寄存器索引
 */
int genAST(struct ASTnode *n) {
    int leftreg, rightreg;

    if(n->left) {
        leftreg = genAST(n->left);
    }
    if(n->right) {
        rightreg = genAST(n->right);
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
        case A_INTLIT:
            return cgload(n->intvalue);
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