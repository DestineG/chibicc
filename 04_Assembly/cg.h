// cg.h

#ifndef CG_H
#define CG_H

#include "ast.h"

void initreg(void);
void free_reg(int reg);
void cgpreamble(void);
void cgpostamble(void);
int cgload(int val);
int cgadd(int reg1, int reg2);
int cgsub(int reg1, int reg2);
int cgmul(int reg1, int reg2);
int cgdiv(int reg1, int reg2);
void cgprintint(int reg);
void cggen(struct ASTnode *n);

#endif