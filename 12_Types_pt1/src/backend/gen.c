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


/*
label0
cond
jump label1
...
jump label0
label1
*/
static int translateWHILEAst(struct ASTnode *n) {
    int lstart, lend;
    // 获取标签
    lstart = label();
    lend = label();

    // 标签插入
    cglabel(lstart);

    // 生成 cond & jmp lend
    translateAst(n->left, lend, n->op);
    genfreeregs();

    // 生成 body
    translateAst(n->right, NOREG, n->op);
    genfreeregs();

    // 生成 jmp & label
    cgjump(lstart);
    cglabel(lend);
    
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
        case A_WHILE:
            return translateWHILEAst(n);
        case A_GLUE:
            translateAst(n->left, NOREG, n->op);
            genfreeregs();
            translateAst(n->right, NOREG, n->op);
            genfreeregs();
            return NOREG;
        case A_FUNCTION:
            cgfuncpreamble(Gsym[n->v.id].name);
            translateAst(n->left, NOREG, n->op);
            cgfuncpostamble();
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
            // NOTE: 当前 if while 条件表达式仅支持单个条件运算符
            // 且表达式默认将第一个运算符当作条件运算符，若不满足则会生成错误
            if(parentASTop == A_IF || parentASTop == A_WHILE) {
                return cgcompare_and_jump(n->op, leftreg, rightreg, reg);
            }
            else {
                return cgcompare_and_set(n->op, leftreg, rightreg);
            }
        case A_INTLIT:
            return (cgloadint(n->v.intvalue));
        // 加载变量值到 reg
        case A_IDENT:
            return (cgloadglob(n->v.id));
        // 将 reg 值存入变量
        case A_LVIDENT:
            return (cgstorglob(reg, n->v.id));
        case A_ASSIGN:
            return (rightreg);
        // NOTE: print AST 节点的表达式节点在 left
        case A_PRINT:
            genprintint(leftreg);
            genfreeregs();
            return (NOREG);
        case A_WIDEN:
          // 类型拓展 AST 节点
          return (cgwiden(leftreg, n->left->type, n->type));
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