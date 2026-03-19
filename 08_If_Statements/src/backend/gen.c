// 07_Comparisons/src/backend/gen.c

#include <stdlib.h>
#include <stdio.h>

#include "../midend/ast.h"
#include "../midend/symbol.h"
#include "cgen.h"
#include "gen.h"

static int label(void) {
  static int id = 1;
  return id++;
}

/**
 * @brief 生成IF语句的汇编代码
 * @param n IF语句的AST节点指针
 * @return 结果寄存器索引
 */
static int translateIFAst(struct ASTnode *n) {
    int lfalse, lend;
    lfalse = label();
    if(n->right) {
        lend = label();
    }

    // 处理条件表达式分支并插入 comp & jmp
    translateAst(n->left, lfalse, n->op);
    genfreeregs();

    // 处理 true 分支
    translateAst(n->mid, NOREG, n->op);
    genfreeregs();

    // 如果有 false 分支，就生成跳转指令
    if(n->right) {
        cgjump(lend);
    }

    // 创建 false 分支的标签
    cglabel(lfalse);

    // 处理 false 分支
    if(n->right) {
        translateAst(n->right, NOREG, n->op);
        genfreeregs();
        cglabel(lend);
    }

    return NOREG;
}

/**
 * @brief 生成AST节点的汇编代码
 * @param n AST节点指针
 * @return 结果寄存器索引
 */
int translateAst(struct ASTnode *n, int reg, int parentASTop) {
    // 控制 AST 处理
    switch(n->op) {
        case A_IF:
            return translateIFAst(n);
        case A_GLUE:
            translateAst(n->left, NOREG, n->op);
            genfreeregs();
            translateAst(n->right, NOREG, n->op);
            genfreeregs();
            return NOREG;
    }

    // 表达式 AST 处理
    int leftreg, rightreg;
    if(n->left) {
        leftreg = translateAst(n->left, NOREG, n->op);
    }
    if(n->right) {
        rightreg = translateAst(n->right, leftreg, n->op);
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
        case A_NE:
        case A_LT:
        case A_GT:
        case A_LE:
        case A_GE:
            if(parentASTop == A_IF) {
                return cgcompare_and_jump(n->op, leftreg, rightreg, reg);
            }
            else {
                return cgcompare_and_set(n->op, leftreg, rightreg);
            }
        case A_INTLIT:
            return (cgloadint(n->v.intvalue));
        // 加载变量值到 reg
        case A_IDENT:
            return (cgloadglob(Gsym[n->v.id].name));
        // 将 reg 值存入变量
        case A_LVIDENT:
            return (cgstorglob(reg, Gsym[n->v.id].name));
        case A_ASSIGN:
            return (rightreg);
        // NOTE: print AST 节点的表达式节点在 left
        case A_PRINT:
            genprintint(leftreg);
            genfreeregs();
            return (NOREG);
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