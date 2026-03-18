// cg.c

#include <stdlib.h>
#include <stdio.h>

#include "../include/global.h"

#include "../midend/ast.h"

static int freereg[4];
static char *reglist[4] = {"%r8", "%r9", "%r10", "%r11"};

/**
 * @brief 初始化寄存器
 */
void initreg(void) {
    for(int i = 0; i < 4; i++) {
        freereg[i] = 1;
    }
}

/**
 * @brief 分配寄存器
 * @return 分配的寄存器索引
 */
static int alloc_reg(void) {
    for(int i = 0; i < 4; i++) {
        if(freereg[i]) {
            freereg[i] = 0;
            return i;
        }
    }
    fprintf(stderr, "alloc_reg: no free register\n");
    exit(1);
}

/**
 * @brief 释放寄存器
 * @param reg 要释放的寄存器索引
 */
static void free_reg(int reg) {
    if(freereg[reg]) {
        fprintf(stderr, "free_reg: register %d is not allocated\n", reg);
        exit(1);
    }
    freereg[reg] = 1;
}

/**
 * @brief 生成汇编代码的前导部分
 */
void cgpreamble()
{
  initreg();
  fputs(
	"\t.text\n"
	".LC0:\n"
	"\t.string\t\"%d\\n\"\n"
	"printint:\n"
	"\tpushq\t%rbp\n"
	"\tmovq\t%rsp, %rbp\n"
	"\tsubq\t$16, %rsp\n"
	"\tmovl\t%edi, -4(%rbp)\n"
	"\tmovl\t-4(%rbp), %eax\n"
	"\tmovl\t%eax, %esi\n"
	"\tleaq	.LC0(%rip), %rdi\n"
	"\tmovl	$0, %eax\n"
	"\tcall	printf@PLT\n"
	"\tnop\n"
	"\tleave\n"
	"\tret\n"
	"\n"
	"\t.globl\tmain\n"
	"\t.type\tmain, @function\n"
	"main:\n"
	"\tpushq\t%rbp\n"
	"\tmovq	%rsp, %rbp\n",
  Outfile);
}

/**
 * @brief 生成汇编代码的后缀部分
 */
void cgpostamble()
{
  fputs(
	"\tmovl	$0, %eax\n"
	"\tpopq	%rbp\n"
	"\tret\n",
  Outfile);
}

/**
 * @brief 加载立即数到寄存器
 * @param val 要加载的立即数
 * @return 分配的寄存器索引
 */
int cgload(int val) {
    int reg = alloc_reg();
    fprintf(Outfile, "\tmovq $%d, %s\n", val, reglist[reg]);
    return reg;
}

/**
 * @brief 加法指令
 * @param reg1 第一个操作数寄存器索引
 * @param reg2 第二个操作数寄存器索引
 * @return 结果寄存器索引 reg2
 */
int cgadd(int reg1, int reg2) {
    fprintf(Outfile, "\taddq %s, %s\n", reglist[reg1], reglist[reg2]);
    free_reg(reg1);
    return reg2;
}

/**
 * @brief 减法指令
 * @param reg1 第一个操作数寄存器索引
 * @param reg2 第二个操作数寄存器索引
 * @return 结果寄存器索引 reg1
 */
int cgsub(int reg1, int reg2) {
    fprintf(Outfile, "\tsubq %s, %s\n", reglist[reg2], reglist[reg1]);
    free_reg(reg2);
    return reg1;
}

/**
 * @brief 乘法指令
 * @param reg1 第一个操作数寄存器索引
 * @param reg2 第二个操作数寄存器索引
 * @return 结果寄存器索引 reg2
 */
int cgmul(int reg1, int reg2) {
    fprintf(Outfile, "\timulq %s, %s\n", reglist[reg1], reglist[reg2]);
    free_reg(reg1);
    return reg2;
}

/**
 * @brief 除法指令
 * @param reg1 第一个操作数寄存器索引
 * @param reg2 第二个操作数寄存器索引
 * @return 结果寄存器索引 reg1
 */
int cgdiv(int reg1, int reg2) {
  fprintf(Outfile, "\tmovq\t%s,%%rax\n", reglist[reg1]);
  fprintf(Outfile, "\tcqo\n");
  fprintf(Outfile, "\tidivq\t%s\n", reglist[reg2]);
  fprintf(Outfile, "\tmovq\t%%rax,%s\n", reglist[reg1]);
  free_reg(reg2);
  return(reg1);
}


/**
 * @brief 生成AST节点的汇编代码
 * @param n AST节点指针
 * @return 结果寄存器索引
 */
static int genAST(struct ASTnode *n) {
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
 * @brief 打印整数寄存器内容
 * @param reg 要打印的寄存器索引
 */
void cgprintint(int reg) {
    fprintf(Outfile, "\tmovq %s, %%rdi\n", reglist[reg]);
    fprintf(Outfile, "\tcall\tprintint\n");
    free_reg(reg);
}

/**
 * @brief 生成完整的汇编代码
 * @param n AST节点指针
 */
void cggen(struct ASTnode *n) {
    int reg;

    cgpreamble();
    reg = genAST(n);
    cgprintint(reg);
    cgpostamble();
}